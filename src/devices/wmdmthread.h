/* This file is part of Clementine.
   Copyright 2010, David Sansome <me@davidsansome.com>

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

#ifndef WMDMTHREAD_H
#define WMDMTHREAD_H

#include <QtGlobal>

#include <sac_shim.h>

struct IWMDeviceManager2;
struct IWMDMDevice;
struct IWMDMStorage;

class WmdmThread {
public:
  WmdmThread();
  ~WmdmThread();

  IWMDeviceManager2* manager() const { return device_manager_; }

  IWMDMDevice* GetDeviceByCanonicalName(const QString& device_name);
  IWMDMStorage* GetRootStorage(const QString& device_name);

private:
  Q_DISABLE_COPY(WmdmThread);

  IWMDeviceManager2* device_manager_;
  SacHandle sac_;
};

#endif // WMDMTHREAD_H
