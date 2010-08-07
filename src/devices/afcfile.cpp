#include "afcfile.h"
#include "imobiledeviceconnection.h"

#include <libimobiledevice/afc.h>

AfcFile::AfcFile(afc_client_t client, const QString& path, QObject* parent)
  : QIODevice(parent),
    client_(client),
    path_(path)
{
}

AfcFile::AfcFile(iMobileDeviceConnection* connection, const QString& path, QObject* parent)
  : QIODevice(parent),
    client_(connection->afc()),
    path_(path)
{
}

AfcFile::~AfcFile() {

}

bool AfcFile::open(QIODevice::OpenMode mode) {
  afc_file_mode_t afc_mode;
  switch (mode) {
    case ReadOnly:
      afc_mode = AFC_FOPEN_RDONLY;
      break;
    case WriteOnly:
      afc_mode = AFC_FOPEN_WRONLY;
      break;
    case ReadWrite:
      afc_mode = AFC_FOPEN_RW;
      break;

    default:
      afc_mode = AFC_FOPEN_RW;
  }
  afc_error_t err = afc_file_open(
      client_, path_.toUtf8().constData(), afc_mode, &handle_);
  if (err != AFC_E_SUCCESS) {
    return false;
  }

  return QIODevice::open(mode);
}

void AfcFile::close() {
  afc_file_close(client_, handle_);
  QIODevice::close();
}

bool AfcFile::seek(qint64 pos) {
  afc_error_t err = afc_file_seek(client_, handle_, pos, SEEK_SET);
  if (err != AFC_E_SUCCESS) {
    return false;
  }
  QIODevice::seek(pos);
  return true;
}

qint64 AfcFile::readData(char* data, qint64 max_size) {
  uint32_t bytes_read = 0;
  afc_error_t err = afc_file_read(client_, handle_, data, max_size, &bytes_read);
  if (err != AFC_E_SUCCESS) {
    return -1;
  }
  return bytes_read;
}

qint64 AfcFile::writeData(const char* data, qint64 max_size) {
  uint32_t bytes_written = 0;
  afc_error_t err = afc_file_write(client_, handle_, data, max_size, &bytes_written);
  if (err != AFC_E_SUCCESS) {
    return -1;
  }
  return bytes_written;
}