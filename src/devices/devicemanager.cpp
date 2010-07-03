/* This file is part of Clementine.

   Clementine is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   Clementine is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Clementine.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "connecteddevice.h"
#include "devicedatabasebackend.h"
#include "devicemanager.h"
#include "devicekitlister.h"
#include "core/utilities.h"
#include "ui/iconloader.h"

#include <QIcon>


DeviceManager::DeviceInfo::DeviceInfo()
  : database_id_(-1),
    lister_(NULL)
{
}

DeviceDatabaseBackend::Device DeviceManager::DeviceInfo::SaveToDb() const {
  DeviceDatabaseBackend::Device ret;
  ret.friendly_name_ = friendly_name_;
  ret.size_ = size_;
  ret.unique_id_ = unique_id_;
  ret.id_ = database_id_;

  if (lister_)
    ret.icon_name_ = lister_->DeviceInfo(
        unique_id_, DeviceLister::Field_Icon).toString();

  return ret;
}

void DeviceManager::DeviceInfo::InitFromDb(const DeviceDatabaseBackend::Device &dev) {
  database_id_ = dev.id_;
  friendly_name_ = dev.friendly_name_;
  unique_id_ = dev.unique_id_;
  size_ = dev.size_;
  LoadIcon(dev.icon_name_);
}

void DeviceManager::DeviceInfo::LoadIcon(const QString &filename) {
  // Try to load the icon with that exact name first
  icon_ = IconLoader::Load(filename);

  // If that failed than try to guess if it's a phone or ipod.  Fall back on
  // a usb memory stick icon.
  if (icon_.isNull()) {
    if (filename.contains("phone"))
      icon_ = IconLoader::Load("phone");
    else if (filename.contains("ipod") || filename.contains("apple"))
      icon_ = IconLoader::Load("multimedia-player-ipod-standard-monochrome");
    else
      icon_ = IconLoader::Load("drive-removable-media-usb-pendrive");
  }
}


DeviceManager::DeviceManager(BackgroundThread<Database>* database,
                             TaskManager* task_manager, QObject *parent)
  : QAbstractListModel(parent),
    database_(database),
    task_manager_(task_manager)
{
  // Create the backend in the database thread
  backend_ = database_->CreateInThread<DeviceDatabaseBackend>();
  backend_->Init(database_->Worker());

  DeviceDatabaseBackend::DeviceList devices = backend_->GetAllDevices();
  foreach (const DeviceDatabaseBackend::Device& device, devices) {
    DeviceInfo info;
    info.InitFromDb(device);
    devices_ << info;
  }

#ifdef Q_WS_X11
  AddLister(new DeviceKitLister);
#endif
}

DeviceManager::~DeviceManager() {
  qDeleteAll(listers_);
  backend_->deleteLater();
}

int DeviceManager::rowCount(const QModelIndex&) const {
  return devices_.count();
}

QVariant DeviceManager::data(const QModelIndex& index, int role) const {
  if (!index.isValid() || index.column() != 0)
    return QVariant();

  const DeviceInfo& info = devices_[index.row()];

  switch (role) {
  case Qt::DisplayRole: {
    QString text = info.friendly_name_.isEmpty() ? info.unique_id_ : info.friendly_name_;
    if (info.size_)
      text = text + QString(" (%1)").arg(Utilities::PrettySize(info.size_));
    return text;
  }

  case Qt::DecorationRole: {
    bool connected = info.lister_;
    return info.icon_.pixmap(22, connected ? QIcon::Normal : QIcon::Disabled);
  }

  default:
    return QVariant();
  }
}

void DeviceManager::AddLister(DeviceLister *lister) {
  listers_ << lister;
  connect(lister, SIGNAL(DeviceAdded(QString)), SLOT(PhysicalDeviceAdded(QString)));
  connect(lister, SIGNAL(DeviceRemoved(QString)), SLOT(PhysicalDeviceRemoved(QString)));
  connect(lister, SIGNAL(DeviceChanged(QString)), SLOT(PhysicalDeviceChanged(QString)));

  lister->Start();
}

int DeviceManager::FindDeviceById(const QString &id) const {
  for (int i=0 ; i<devices_.count() ; ++i) {
    if (devices_[i].unique_id_ == id)
      return i;
  }
  return -1;
}

void DeviceManager::PhysicalDeviceAdded(const QString &id) {
  DeviceLister* lister = qobject_cast<DeviceLister*>(sender());

  qDebug() << "Device added:" << id;
  for (int i=0 ; i<DeviceKitLister::LastField ; ++i) {
    qDebug() << i << lister->DeviceInfo(id, i);
  }

  // Do we have this device already?
  int i = FindDeviceById(id);
  if (i == -1) {
    DeviceInfo info;
    info.lister_ = lister;
    info.unique_id_ = id;
    info.friendly_name_ = lister->DeviceInfo(id, DeviceLister::Field_FriendlyName).toString();
    info.size_ = lister->DeviceInfo(id, DeviceLister::Field_Capacity).toLongLong();
    info.LoadIcon(lister->DeviceInfo(id, DeviceLister::Field_Icon).toString());

    beginInsertRows(QModelIndex(), devices_.count(), devices_.count());
    devices_ << info;
    endInsertRows();
  } else {
    DeviceInfo& info = devices_[i];

    info.lister_ = lister;
    dataChanged(index(i, 0), index(i, 0));
  }
}

void DeviceManager::PhysicalDeviceRemoved(const QString &id) {
  DeviceLister* lister = qobject_cast<DeviceLister*>(sender());
  qDebug() << "Device removed:" << id;

  int i = FindDeviceById(id);
  if (i == -1) {
    // Shouldn't happen
    return;
  }

  DeviceInfo& info = devices_[i];

  if (info.database_id_ != -1) {
    // Keep the structure around, but just "disconnect" it
    info.lister_ = NULL;
    info.device_.reset();

    emit dataChanged(index(i, 0), index(i, 0));
  } else {
    // Remove the item from the model
    beginRemoveRows(QModelIndex(), i, i);
    devices_.removeAt(i);
    endRemoveRows();
  }
}

void DeviceManager::PhysicalDeviceChanged(const QString &id) {
  DeviceLister* lister = qobject_cast<DeviceLister*>(sender());

  int i = FindDeviceById(id);
  if (i == -1) {
    // Shouldn't happen
    return;
  }

  // TODO
}

boost::shared_ptr<ConnectedDevice> DeviceManager::Connect(int row) {
  DeviceInfo& info = devices_[row];
  if (info.device_) // Already connected
    return info.device_;

  if (!info.lister_) // Not physically connected
    return boost::shared_ptr<ConnectedDevice>();

  bool first_time = (info.database_id_ == -1);
  if (first_time) {
    // We haven't stored this device in the database before
    info.database_id_ = backend_->AddDevice(info.SaveToDb());
  }

  info.device_ = info.lister_->Connect(info.unique_id_, this, info.database_id_,
                                       first_time);
  return info.device_;
}

boost::shared_ptr<ConnectedDevice> DeviceManager::GetConnectedDevice(int row) const {
  return devices_[row].device_;
}