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

#ifndef LASTFMSETTINGSPAGE_H
#define LASTFMSETTINGSPAGE_H

#include "ui/settingspage.h"

class LastFMService;
class Ui_LastFMSettingsPage;

class LastFMSettingsPage : public SettingsPage {
  Q_OBJECT

public:
  LastFMSettingsPage(SettingsDialog* dialog);
  ~LastFMSettingsPage();

  void Load();
  void Save();

private slots:
  void Login();
  void AuthenticationComplete(bool success);
  void Logout();
  void UpdatedSubscriberStatus(bool is_subscriber);

private:
  LastFMService* service_;
  Ui_LastFMSettingsPage* ui_;
  QMovie* loading_icon_;

  bool waiting_for_auth_;

  void RefreshControls(bool authenticated);
};

#endif // LASTFMSETTINGSPAGE_H
