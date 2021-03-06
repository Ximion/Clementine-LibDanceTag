import clementine

from servicebase import DigitallyImportedServiceBase

from PythonQt.QtCore    import QSettings, QUrl
from PythonQt.QtNetwork import QNetworkCookie, QNetworkCookieJar, QNetworkRequest

import re

class SkyFmService(DigitallyImportedServiceBase):
  HOMEPAGE_URL = QUrl("http://www.sky.fm/")
  HOMEPAGE_NAME = "sky.fm"
  STREAM_LIST_URL = QUrl("http://listen.sky.fm/")
  ICON_FILENAME = "icon-sky.png"
  SERVICE_NAME = "SKY.fm"
  SERVICE_DESCRIPTION = "SKY.fm"
  URL_SCHEME = "skyfm"

  HASHKEY_RE = re.compile(r'hashKey\s*=\s*\'([0-9a-f]+)\'')

  # These have to be in the same order as in the settings dialog
  PLAYLISTS = [
    {"premium": False, "url": "http://listen.sky.fm/public3/%s.pls"},
    {"premium": True,  "url": "http://listen.sky.fm/premium_high/%s.pls?hash=%s"},
    {"premium": False, "url": "http://listen.sky.fm/public1/%s.pls"},
    {"premium": True,  "url": "http://listen.sky.fm/premium_medium/%s.pls?hash=%s"},
    {"premium": True,  "url": "http://listen.sky.fm/premium/%s.pls?hash=%s"},
    {"premium": False, "url": "http://listen.sky.fm/public5/%s.asx"},
    {"premium": True,  "url": "http://listen.sky.fm/premium_wma_low/%s.asx?hash=%s"},
    {"premium": True,  "url": "http://listen.sky.fm/premium_wma/%s.asx?hash=%s"},
  ]

  def __init__(self, model, settings_dialog_callback):
    DigitallyImportedServiceBase.Init(self, model, settings_dialog_callback)

    self.last_key = None
    self.load_station_reply = None

  def LoadStation(self, key):
    # Non-premium streams can just start loading straight away
    if not self.PLAYLISTS[self.audio_type]["premium"]:
      self.LoadPlaylist(key)
      return

    # Otherwise we have to get the user's hashKey
    request = QNetworkRequest(QUrl("http://www.sky.fm/configure_player.php"))
    postdata = "amember_login=%s&amember_pass=%s" % (
      QUrl.toPercentEncoding(self.username),
      QUrl.toPercentEncoding(self.password))

    self.load_station_reply = self.network.post(request, postdata)
    self.load_station_reply.connect("finished()", self.LoadHashKeyFinished)

    self.last_key = key

  def LoadHashKeyFinished(self):
    if self.load_station_reply is None:
      return

    # Parse the hashKey out of the reply
    data = self.load_station_reply.readAll().data()
    match = self.HASHKEY_RE.search(data)
    self.load_station_reply = None

    if match:
      hash_key = match.group(1)
    else:
      clementine.task_manager.SetTaskFinished(self.task_id)
      self.task_id = None
      self.StreamError(self.tr("Invalid SKY.fm username or password"))
      return

    # Now we can load the playlist
    self.LoadPlaylist(self.last_key, hash_key)

  def LoadPlaylist(self, key, hash_key=None):
    playlist_url = self.PLAYLISTS[self.audio_type]["url"]

    if hash_key:
      playlist_url = playlist_url % (key, hash_key)
    else:
      playlist_url = playlist_url % key

    # Start fetching the playlist.  Can't use a SongLoader to do this because
    # we have to use the cookies we set in ReloadSettings()
    self.load_station_reply = self.network.get(QNetworkRequest(QUrl(playlist_url)))
    self.load_station_reply.connect("finished()", self.LoadPlaylistFinished)
