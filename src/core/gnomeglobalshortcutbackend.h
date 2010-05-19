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

#ifndef GNOMEGLOBALSHORTCUTBACKEND_H
#define GNOMEGLOBALSHORTCUTBACKEND_H

#include "globalshortcutbackend.h"

class QDBusInterface;

class GnomeGlobalShortcutBackend : public GlobalShortcutBackend {
  Q_OBJECT

public:
  GnomeGlobalShortcutBackend(GlobalShortcuts* parent);

  static const char* kGsdService;
  static const char* kGsdPath;
  static const char* kGsdInterface;

protected:
  bool DoRegister();
  void DoUnregister();

private slots:
  void GnomeMediaKeyPressed(const QString& application, const QString& key);

private:
  QDBusInterface* interface_;
};

#endif // GNOMEGLOBALSHORTCUTBACKEND_H