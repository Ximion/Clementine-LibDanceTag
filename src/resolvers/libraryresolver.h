#ifndef LIBRARYRESOLVER_H
#define LIBRARYRESOLVER_H

#include <QMap>

#include "resolver.h"

class QNetworkReply;

class LibraryBackendInterface;
class LibraryQuery;

class LibraryResolver : public Resolver {
  Q_OBJECT
 public:
  LibraryResolver(LibraryBackendInterface* backend, QObject* parent = 0);
  int ResolveSong(const Song& song);

 signals:
  void ResolveFinished(int id, const SongList& songs);

 private slots:
  void QueryFinished();

 private:
  LibraryBackendInterface* backend_;
  QMap<LibraryQuery*, int> queries_;
  int next_id_;
};

#endif  // LIBRARYRESOLVER_H
