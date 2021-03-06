
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Woverloaded-virtual -Wall -Wno-sign-compare")

option(BUILD_WERROR "Build with -Werror" ON)

if(BUILD_WERROR)
  if (LINUX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
  endif (LINUX)
endif(BUILD_WERROR)

include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(../3rdparty/gmock/gtest/include)
include_directories(../3rdparty/sha2)
if(WIN32)
  include_directories(../3rdparty/qtwin)
endif(WIN32)

include_directories(${CMAKE_BINARY_DIR})
include_directories(${GLIB_INCLUDE_DIRS})
include_directories(${LIBXML_INCLUDE_DIRS})
include_directories(${GOBJECT_INCLUDE_DIRS})
include_directories(${LIBPROJECTM_INCLUDE_DIRS})
include_directories(${QTSINGLEAPPLICATION_INCLUDE_DIRS})
include_directories(${QTIOCOMPRESSOR_INCLUDE_DIRS})
include_directories(${QXT_INCLUDE_DIRS})
include_directories(${ECHONEST_INCLUDE_DIRS})

if(HAVE_LIBINDICATE)
  link_directories(${INDICATEQT_LIBRARY_DIRS})
  include_directories(${INDICATEQT_INCLUDE_DIRS})
endif(HAVE_LIBINDICATE)

if(HAVE_LIBLASTFM)
  include_directories(${LASTFM_INCLUDE_DIRS})
endif(HAVE_LIBLASTFM)

if(HAVE_BREAKPAD)
  include_directories(../3rdparty/google-breakpad)
endif(HAVE_BREAKPAD)

if(HAVE_REMOTE)
  include_directories(${PJSIP_INCLUDE_DIRS})
endif(HAVE_REMOTE)

cmake_policy(SET CMP0011 NEW)
include(../cmake/ParseArguments.cmake)
include(../cmake/Translations.cmake)

set(SOURCES
  analyzers/analyzerbase.cpp
  analyzers/analyzercontainer.cpp
  analyzers/baranalyzer.cpp
  analyzers/blockanalyzer.cpp
  analyzers/boomanalyzer.cpp
  analyzers/nyancatanalyzer.cpp
  analyzers/sonogram.cpp
  analyzers/turbine.cpp

  core/backgroundstreams.cpp
  core/backgroundthread.cpp
  core/commandlineoptions.cpp
  core/crashreporting.cpp
  core/database.cpp
  core/deletefiles.cpp
  core/encoding.cpp
  core/filesystemmusicstorage.cpp
  core/fht.cpp
  core/fmpsparser.cpp
  core/globalshortcutbackend.cpp
  core/globalshortcuts.cpp
  core/gnomeglobalshortcutbackend.cpp
  core/logging.cpp
  core/mergedproxymodel.cpp
  core/multisortfilterproxy.cpp
  core/musicstorage.cpp
  core/network.cpp
  core/networkproxyfactory.cpp
  core/organise.cpp
  core/organiseformat.cpp
  core/player.cpp
  core/qxtglobalshortcutbackend.cpp
  core/scopedtransaction.cpp
  core/settingsprovider.cpp
  core/song.cpp
  core/songloader.cpp
  core/stylesheetloader.cpp
  core/taskmanager.cpp
  core/urlhandler.cpp
  core/utilities.cpp

  covers/albumcoverfetcher.cpp
  covers/albumcoverfetchersearch.cpp
  covers/albumcoverloader.cpp
  covers/amazoncoverprovider.cpp
  covers/artloader.cpp
  covers/coverprovider.cpp
  covers/coverproviders.cpp
  covers/coversearchstatistics.cpp
  covers/coversearchstatisticsdialog.cpp
  covers/kittenloader.cpp

  devices/connecteddevice.cpp
  devices/devicedatabasebackend.cpp
  devices/devicelister.cpp
  devices/devicemanager.cpp
  devices/deviceproperties.cpp
  devices/devicestatefiltermodel.cpp
  devices/deviceview.cpp
  devices/filesystemdevice.cpp

  engines/enginebase.cpp
  engines/gstengine.cpp
  engines/gstenginepipeline.cpp
  engines/gstelementdeleter.cpp

  globalsearch/globalsearch.cpp
  globalsearch/globalsearchitemdelegate.cpp
  globalsearch/globalsearchpopup.cpp
  globalsearch/globalsearchsortmodel.cpp
  globalsearch/globalsearchtooltip.cpp
  globalsearch/globalsearchwidget.cpp
  globalsearch/librarysearchprovider.cpp
  globalsearch/searchprovider.cpp
  globalsearch/tooltipactionwidget.cpp
  globalsearch/tooltipresultwidget.cpp

  internet/dancetagprovider.cpp
  internet/dancetagsettingspage.cpp
  internet/digitallyimportedservice.cpp
  internet/digitallyimportedservicebase.cpp
  internet/digitallyimportedsettingspage.cpp
  internet/digitallyimportedurlhandler.cpp
  internet/icecastbackend.cpp
  internet/icecastfilterwidget.cpp
  internet/icecastmodel.cpp
  internet/icecastservice.cpp
  internet/internetmodel.cpp
  internet/internetplaylistitem.cpp
  internet/internetservice.cpp
  internet/internetview.cpp
  internet/internetviewcontainer.cpp
  internet/jamendodynamicplaylist.cpp
  internet/jamendoplaylistitem.cpp
  internet/jamendoservice.cpp
  internet/magnatunedownloaddialog.cpp
  internet/magnatuneplaylistitem.cpp
  internet/magnatunesettingspage.cpp
  internet/magnatuneservice.cpp
  internet/magnatuneurlhandler.cpp
  internet/savedradio.cpp
  internet/skyfmservice.cpp
  internet/somafmservice.cpp
  internet/somafmurlhandler.cpp

  library/groupbydialog.cpp
  library/library.cpp
  library/librarybackend.cpp
  library/librarydirectorymodel.cpp
  library/libraryfilterwidget.cpp
  library/librarymodel.cpp
  library/libraryplaylistitem.cpp
  library/libraryquery.cpp
  library/librarysettingspage.cpp
  library/libraryview.cpp
  library/libraryviewcontainer.cpp
  library/librarywatcher.cpp
  library/sqlrow.cpp

  musicbrainz/echoprinter.cpp
  musicbrainz/fingerprinter.cpp
  musicbrainz/musicbrainzclient.cpp
  musicbrainz/musicdnsclient.cpp
  musicbrainz/tagfetcher.cpp

  playlist/dynamicplaylistcontrols.cpp
  playlist/playlist.cpp
  playlist/playlistbackend.cpp
  playlist/playlistcontainer.cpp
  playlist/playlistdelegates.cpp
  playlist/playlistfilter.cpp
  playlist/playlistheader.cpp
  playlist/playlistitem.cpp
  playlist/playlistmanager.cpp
  playlist/playlistsequence.cpp
  playlist/playlisttabbar.cpp
  playlist/playlistundocommands.cpp
  playlist/playlistview.cpp
  playlist/specialplaylisttype.cpp
  playlist/queue.cpp
  playlist/queuemanager.cpp
  playlist/songloaderinserter.cpp
  playlist/songplaylistitem.cpp

  playlistparsers/asxparser.cpp
  playlistparsers/asxiniparser.cpp
  playlistparsers/cueparser.cpp
  playlistparsers/m3uparser.cpp
  playlistparsers/parserbase.cpp
  playlistparsers/playlistparser.cpp
  playlistparsers/plsparser.cpp
  playlistparsers/xmlparser.cpp
  playlistparsers/xspfparser.cpp

  resolvers/libraryresolver.cpp
  resolvers/songresolver.cpp

  smartplaylists/generator.cpp
  smartplaylists/generatorinserter.cpp
  smartplaylists/querygenerator.cpp
  smartplaylists/querywizardplugin.cpp
  smartplaylists/search.cpp
  smartplaylists/searchpreview.cpp
  smartplaylists/searchterm.cpp
  smartplaylists/searchtermwidget.cpp
  smartplaylists/wizard.cpp
  smartplaylists/wizardplugin.cpp

  songinfo/artistinfoview.cpp
  songinfo/collapsibleinfoheader.cpp
  songinfo/collapsibleinfopane.cpp
  songinfo/echonestbiographies.cpp
  songinfo/echonestimages.cpp
  songinfo/songinfobase.cpp
  songinfo/songinfofetcher.cpp
  songinfo/songinfoprovider.cpp
  songinfo/songinfosettingspage.cpp
  songinfo/songinfotextview.cpp
  songinfo/songinfoview.cpp
  songinfo/songplaystats.cpp
  songinfo/ultimatelyricsprovider.cpp
  songinfo/ultimatelyricsreader.cpp

  transcoder/transcodedialog.cpp
  transcoder/transcoder.cpp
  transcoder/transcoderoptionsaac.cpp
  transcoder/transcoderoptionsdialog.cpp
  transcoder/transcoderoptionsflac.cpp
  transcoder/transcoderoptionsmp3.cpp
  transcoder/transcoderoptionsspeex.cpp
  transcoder/transcoderoptionsvorbis.cpp
  transcoder/transcoderoptionswma.cpp
  transcoder/transcodersettingspage.cpp

  ui/about.cpp
  ui/addstreamdialog.cpp
  ui/albumcoverchoicecontroller.cpp
  ui/albumcovermanager.cpp
  ui/albumcovermanagerlist.cpp
  ui/albumcoversearcher.cpp
  ui/backgroundstreamssettingspage.cpp
  ui/behavioursettingspage.cpp
  ui/coverfromurldialog.cpp
  ui/edittagdialog.cpp
  ui/equalizer.cpp
  ui/flowlayout.cpp
  ui/globalshortcutgrabber.cpp
  ui/globalshortcutssettingspage.cpp
  ui/iconloader.cpp
  ui/mainwindow.cpp
  ui/networkproxysettingspage.cpp
  ui/notificationssettingspage.cpp
  ui/organisedialog.cpp
  ui/organiseerrordialog.cpp
  ui/playbacksettingspage.cpp
  ui/qtsystemtrayicon.cpp
  ui/screensaver.cpp
  ui/settingsdialog.cpp
  ui/settingspage.cpp
  ui/systemtrayicon.cpp
  ui/trackselectiondialog.cpp
  ui/windows7thumbbar.cpp

  widgets/autoexpandingtreeview.cpp
  widgets/busyindicator.cpp
  widgets/clickablelabel.cpp
  widgets/didyoumean.cpp
  widgets/elidedlabel.cpp
  widgets/equalizerslider.cpp
  widgets/errordialog.cpp
  widgets/fancytabwidget.cpp
  widgets/fileview.cpp
  widgets/fileviewlist.cpp
  widgets/forcescrollperpixel.cpp
  widgets/freespacebar.cpp
  widgets/fullscreenhypnotoad.cpp
  widgets/groupediconview.cpp
  widgets/lineedit.cpp
  widgets/linetextedit.cpp
  widgets/loginstatewidget.cpp
  widgets/multiloadingindicator.cpp
  widgets/nowplayingwidget.cpp
  widgets/osd.cpp
  widgets/osdpretty.cpp
  widgets/prettyimage.cpp
  widgets/prettyimageview.cpp
  widgets/progressitemdelegate.cpp
  widgets/ratingwidget.cpp
  widgets/renametablineedit.cpp
  widgets/sliderwidget.cpp
  widgets/spinbox.cpp
  widgets/stickyslider.cpp
  widgets/stretchheaderview.cpp
  widgets/stylehelper.cpp
  widgets/trackslider.cpp
  widgets/tracksliderpopup.cpp
  widgets/tracksliderslider.cpp
  widgets/widgetfadehelper.cpp
)

set(HEADERS
  analyzers/analyzerbase.h
  analyzers/analyzercontainer.h
  analyzers/baranalyzer.h
  analyzers/blockanalyzer.h
  analyzers/boomanalyzer.h
  analyzers/nyancatanalyzer.h
  analyzers/sonogram.h
  analyzers/turbine.h

  core/backgroundstreams.h
  core/backgroundthread.h
  core/crashreporting.h
  core/database.h
  core/deletefiles.h
  core/globalshortcuts.h
  core/globalshortcutbackend.h
  core/gnomeglobalshortcutbackend.h
  core/mergedproxymodel.h
  core/mimedata.h
  core/network.h
  core/organise.h
  core/player.h
  core/songloader.h
  core/taskmanager.h
  core/urlhandler.h

  covers/albumcoverfetcher.h
  covers/albumcoverfetchersearch.h
  covers/albumcoverloader.h
  covers/amazoncoverprovider.h
  covers/artloader.h
  covers/coverprovider.h
  covers/coverproviders.h
  covers/coversearchstatisticsdialog.h
  covers/kittenloader.h

  devices/connecteddevice.h
  devices/devicedatabasebackend.h
  devices/devicelister.h
  devices/devicemanager.h
  devices/deviceproperties.h
  devices/devicestatefiltermodel.h
  devices/deviceview.h
  devices/filesystemdevice.h

  engines/enginebase.h
  engines/gstengine.h
  engines/gstenginepipeline.h
  engines/gstelementdeleter.h

  globalsearch/librarysearchprovider.h
  globalsearch/globalsearch.h
  globalsearch/globalsearchpopup.h
  globalsearch/globalsearchtooltip.h
  globalsearch/globalsearchwidget.h
  globalsearch/searchprovider.h
  globalsearch/tooltipactionwidget.h
  globalsearch/tooltipresultwidget.h

  internet/dancetagprovider.h
  internet/dancetagsettingspage.h
  internet/digitallyimportedservicebase.h
  internet/digitallyimportedsettingspage.h
  internet/icecastbackend.h
  internet/icecastfilterwidget.h
  internet/icecastmodel.h
  internet/icecastservice.h
  internet/internetmimedata.h
  internet/internetmodel.h
  internet/internetservice.h
  internet/internetview.h
  internet/internetviewcontainer.h
  internet/jamendodynamicplaylist.h
  internet/jamendoservice.h
  internet/magnatunedownloaddialog.h
  internet/magnatunesettingspage.h
  internet/magnatuneservice.h
  internet/savedradio.h
  internet/skyfmservice.h
  internet/somafmservice.h
  internet/somafmurlhandler.h

  library/groupbydialog.h
  library/library.h
  library/librarybackend.h
  library/librarydirectorymodel.h
  library/libraryfilterwidget.h
  library/librarymodel.h
  library/librarysettingspage.h
  library/libraryview.h
  library/libraryviewcontainer.h
  library/librarywatcher.h

  musicbrainz/musicbrainzclient.h
  musicbrainz/musicdnsclient.h
  musicbrainz/tagfetcher.h

  playlist/dynamicplaylistcontrols.h
  playlist/playlist.h
  playlist/playlistbackend.h
  playlist/playlistcontainer.h
  playlist/playlistdelegates.h
  playlist/playlistfilter.h
  playlist/playlistheader.h
  playlist/playlistitemmimedata.h
  playlist/playlistmanager.h
  playlist/playlistsequence.h
  playlist/playlisttabbar.h
  playlist/playlistview.h
  playlist/queue.h
  playlist/queuemanager.h
  playlist/songloaderinserter.h
  playlist/songmimedata.h

  playlistparsers/asxparser.h
  playlistparsers/asxiniparser.h
  playlistparsers/cueparser.h
  playlistparsers/m3uparser.h
  playlistparsers/parserbase.h
  playlistparsers/playlistparser.h
  playlistparsers/plsparser.h
  playlistparsers/xspfparser.h

  resolvers/libraryresolver.h
  resolvers/resolver.h
  resolvers/songresolver.h

  smartplaylists/generator.h
  smartplaylists/generatorinserter.h
  smartplaylists/generatormimedata.h
  smartplaylists/querywizardplugin.h
  smartplaylists/searchpreview.h
  smartplaylists/searchtermwidget.h
  smartplaylists/wizard.h
  smartplaylists/wizardplugin.h

  songinfo/artistinfoview.h
  songinfo/collapsibleinfoheader.h
  songinfo/collapsibleinfopane.h
  songinfo/echonestbiographies.h
  songinfo/echonestimages.h
  songinfo/songinfobase.h
  songinfo/songinfofetcher.h
  songinfo/songinfoprovider.h
  songinfo/songinfosettingspage.h
  songinfo/songinfotextview.h
  songinfo/songinfoview.h
  songinfo/songplaystats.h
  songinfo/ultimatelyricsprovider.h
  songinfo/ultimatelyricsreader.h

  transcoder/transcodedialog.h
  transcoder/transcoder.h
  transcoder/transcoderoptionsdialog.h
  transcoder/transcoderoptionsmp3.h
  transcoder/transcodersettingspage.h

  ui/about.h
  ui/addstreamdialog.h
  ui/albumcoverchoicecontroller.h
  ui/albumcovermanager.h
  ui/albumcovermanagerlist.h
  ui/albumcoversearcher.h
  ui/backgroundstreamssettingspage.h
  ui/behavioursettingspage.h
  ui/coverfromurldialog.h
  ui/edittagdialog.h
  ui/equalizer.h
  ui/globalshortcutgrabber.h
  ui/globalshortcutssettingspage.h
  ui/mainwindow.h
  ui/networkproxysettingspage.h
  ui/notificationssettingspage.h
  ui/organisedialog.h
  ui/organiseerrordialog.h
  ui/playbacksettingspage.h
  ui/qtsystemtrayicon.h
  ui/settingsdialog.h
  ui/settingspage.h
  ui/systemtrayicon.h
  ui/trackselectiondialog.h
  ui/windows7thumbbar.h

  widgets/autoexpandingtreeview.h
  widgets/busyindicator.h
  widgets/clickablelabel.h
  widgets/didyoumean.h
  widgets/elidedlabel.h
  widgets/equalizerslider.h
  widgets/errordialog.h
  widgets/fancytabwidget.h
  widgets/fileview.h
  widgets/fileviewlist.h
  widgets/freespacebar.h
  widgets/groupediconview.h
  widgets/lineedit.h
  widgets/linetextedit.h
  widgets/loginstatewidget.h
  widgets/multiloadingindicator.h
  widgets/nowplayingwidget.h
  widgets/osd.h
  widgets/osdpretty.h
  widgets/prettyimage.h
  widgets/prettyimageview.h
  widgets/progressitemdelegate.h
  widgets/ratingwidget.h
  widgets/renametablineedit.h
  widgets/sliderwidget.h
  widgets/spinbox.h
  widgets/stickyslider.h
  widgets/stretchheaderview.h
  widgets/trackslider.h
  widgets/tracksliderpopup.h
  widgets/tracksliderslider.h
  widgets/widgetfadehelper.h
)

set(UI
  covers/coversearchstatisticsdialog.ui

  devices/deviceproperties.ui

  globalsearch/globalsearchpopup.ui
  globalsearch/globalsearchwidget.ui

  internet/dancetagsettingspage.ui
  internet/digitallyimportedsettingspage.ui
  internet/icecastfilterwidget.ui
  internet/internetviewcontainer.ui
  internet/magnatunedownloaddialog.ui
  internet/magnatunesettingspage.ui
  internet/spotifysettingspage.ui

  library/groupbydialog.ui
  library/libraryfilterwidget.ui
  library/librarysettingspage.ui
  library/libraryviewcontainer.ui

  playlist/dynamicplaylistcontrols.ui
  playlist/playlistcontainer.ui
  playlist/playlistsequence.ui
  playlist/queuemanager.ui

  remote/remotesettingspage.ui

  smartplaylists/querysearchpage.ui
  smartplaylists/querysortpage.ui
  smartplaylists/searchpreview.ui
  smartplaylists/searchtermwidget.ui
  smartplaylists/wizardfinishpage.ui

  songinfo/songinfosettingspage.ui

  transcoder/transcodedialog.ui
  transcoder/transcodelogdialog.ui
  transcoder/transcoderoptionsaac.ui
  transcoder/transcoderoptionsdialog.ui
  transcoder/transcoderoptionsflac.ui
  transcoder/transcoderoptionsmp3.ui
  transcoder/transcoderoptionsspeex.ui
  transcoder/transcoderoptionsvorbis.ui
  transcoder/transcoderoptionswma.ui
  transcoder/transcodersettingspage.ui

  ui/about.ui
  ui/addstreamdialog.ui
  ui/albumcovermanager.ui
  ui/albumcoversearcher.ui
  ui/backgroundstreamssettingspage.ui
  ui/behavioursettingspage.ui
  ui/coverfromurldialog.ui
  ui/edittagdialog.ui
  ui/equalizer.ui
  ui/globalshortcutgrabber.ui
  ui/globalshortcutssettingspage.ui
  ui/mainwindow.ui
  ui/networkproxysettingspage.ui
  ui/notificationssettingspage.ui
  ui/organisedialog.ui
  ui/organiseerrordialog.ui
  ui/playbacksettingspage.ui
  ui/settingsdialog.ui
  ui/trackselectiondialog.ui

  widgets/equalizerslider.ui
  widgets/errordialog.ui
  widgets/fileview.ui
  widgets/loginstatewidget.ui
  widgets/osdpretty.ui
  widgets/trackslider.ui

  wiimotedev/wiimotesettingspage.ui
  wiimotedev/wiimoteshortcutgrabber.ui
)

set(RESOURCES
  ../data/data.qrc
)

set(LINGUAS "All" CACHE STRING "A space-seperated list of translations to compile in to Clementine, or \"None\".")
if (LINGUAS STREQUAL "All")
  # build LANGUAGES from all existing .po files
  file(GLOB pofiles translations/*.po)
  foreach(pofile ${pofiles})
    get_filename_component(lang ${pofile} NAME_WE)
    list(APPEND LANGUAGES ${lang})
  endforeach(pofile)
else (LINGUAS STREQUAL "All")
  if (NOT LINGUAS OR LINGUAS STREQUAL "None")
    set (LANGUAGES "")
  else (NOT LINGUAS OR LINGUAS STREQUAL "None")
    string(REGEX MATCHALL [a-zA-Z_]+
           LANGUAGES ${LINGUAS})
  endif (NOT LINGUAS OR LINGUAS STREQUAL "None")
endif (LINGUAS STREQUAL "All")

option(USE_INSTALL_PREFIX "Look for data in CMAKE_INSTALL_PREFIX" ON)

# Visualisations
if(ENABLE_VISUALISATIONS)

  list(APPEND SOURCES
    visualisations/projectmpresetmodel.cpp
    visualisations/projectmvisualisation.cpp
    visualisations/visualisationcontainer.cpp
    visualisations/visualisationoverlay.cpp
    visualisations/visualisationselector.cpp
  )
  list(APPEND HEADERS
    visualisations/projectmpresetmodel.h
    visualisations/projectmvisualisation.h
    visualisations/visualisationcontainer.h
    visualisations/visualisationoverlay.h
    visualisations/visualisationselector.h
  )
  list(APPEND UI
    visualisations/visualisationoverlay.ui
    visualisations/visualisationselector.ui
  )
endif(ENABLE_VISUALISATIONS)

# Lastfm
if(HAVE_LIBLASTFM)
  list(APPEND SOURCES
    covers/lastfmcoverprovider.cpp
    internet/fixlastfm.cpp
    internet/lastfmservice.cpp
    internet/lastfmsettingspage.cpp
    internet/lastfmstationdialog.cpp
    internet/lastfmurlhandler.cpp
    songinfo/echonestsimilarartists.cpp
    songinfo/echonesttags.cpp
    songinfo/lastfmtrackinfoprovider.cpp
    songinfo/tagwidget.cpp
    suggesters/lastfmsuggester.cpp
  )
  list(APPEND HEADERS
    covers/lastfmcoverprovider.h
    internet/lastfmservice.h
    internet/lastfmsettingspage.h
    internet/lastfmstationdialog.h
    songinfo/echonestsimilarartists.h
    songinfo/echonesttags.h
    songinfo/lastfmtrackinfoprovider.h
    songinfo/tagwidget.h
    suggesters/lastfmsuggester.h
  )
  list(APPEND UI
    internet/lastfmsettingspage.ui
    internet/lastfmstationdialog.ui
  )
endif(HAVE_LIBLASTFM)

if(HAVE_SPOTIFY)
  list(APPEND SOURCES
    globalsearch/spotifysearchprovider.cpp
    internet/spotifyblobdownloader.cpp
    internet/spotifysearchplaylisttype.cpp
    internet/spotifyserver.cpp
    internet/spotifyservice.cpp
    internet/spotifysettingspage.cpp
    internet/spotifyurlhandler.cpp
    resolvers/spotifyresolver.cpp
  )
  list(APPEND HEADERS
    globalsearch/spotifysearchprovider.h
    internet/spotifyblobdownloader.h
    internet/spotifyserver.h
    internet/spotifyservice.h
    internet/spotifysettingspage.h
    resolvers/spotifyresolver.h
  )
endif(HAVE_SPOTIFY)

if(APPLE)
  list(APPEND HEADERS core/macglobalshortcutbackend.h)
  list(APPEND HEADERS devices/macdevicelister.h)
  list(APPEND HEADERS ui/macscreensaver.h)
  list(APPEND HEADERS ui/macsystemtrayicon.h)
  list(APPEND HEADERS widgets/maclineedit.h)
  list(APPEND SOURCES core/macglobalshortcutbackend.mm)
  list(APPEND SOURCES devices/macdevicelister.mm)
  list(APPEND SOURCES globalsearch/globalsearchpopup.mm)
  list(APPEND SOURCES ui/globalshortcutgrabber.mm)
  list(APPEND SOURCES ui/macscreensaver.cpp)
  list(APPEND SOURCES ui/macsystemtrayicon.mm)
  list(APPEND SOURCES widgets/maclineedit.mm)
  list(APPEND SOURCES widgets/osd_mac.mm)
  include_directories(${GROWL}/Headers)
else(APPLE)
  if(WIN32)
    list(APPEND SOURCES widgets/osd_win.cpp)
  else(WIN32)
    list(APPEND SOURCES widgets/osd_x11.cpp)
  endif(WIN32)
endif(APPLE)

# DBUS and MPRIS - Linux specific
if(HAVE_DBUS)
  file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/dbus)

  # Hack to get it to generate interfaces without namespaces - required
  # because otherwise org::freedesktop::UDisks and
  # org::freedesktop::UDisks::Device conflict.
  list(APPEND QT_DBUSXML2CPP_EXECUTABLE -N)

  # MPRIS DBUS interfaces
  qt4_add_dbus_adaptor(SOURCES
      dbus/org.freedesktop.MediaPlayer.player.xml
      core/mpris1.h mpris::Mpris1Player core/mpris_player MprisPlayer)
  qt4_add_dbus_adaptor(SOURCES
      dbus/org.freedesktop.MediaPlayer.root.xml
      core/mpris1.h mpris::Mpris1Root core/mpris_root MprisRoot)
  qt4_add_dbus_adaptor(SOURCES
      dbus/org.freedesktop.MediaPlayer.tracklist.xml
      core/mpris1.h mpris::Mpris1TrackList core/mpris_tracklist MprisTrackList)

  # MPRIS 2.0 DBUS interfaces
  qt4_add_dbus_adaptor(SOURCES
      dbus/org.mpris.MediaPlayer2.Player.xml
      core/mpris2.h mpris::Mpris2 core/mpris2_player Mpris2Player)
  qt4_add_dbus_adaptor(SOURCES
      dbus/org.mpris.MediaPlayer2.xml
      core/mpris2.h mpris::Mpris2 core/mpris2_root Mpris2Root)
  qt4_add_dbus_adaptor(SOURCES
      dbus/org.mpris.MediaPlayer2.TrackList.xml
      core/mpris2.h mpris::Mpris2 core/mpris2_tracklist Mpris2TrackList)

  # org.freedesktop.Notifications DBUS interface
  qt4_add_dbus_interface(SOURCES
      dbus/org.freedesktop.Notifications.xml
      dbus/notification)

  # org.gnome.SettingsDaemon interface
  qt4_add_dbus_interface(SOURCES
      dbus/org.gnome.SettingsDaemon.MediaKeys.xml
      dbus/gnomesettingsdaemon)


  # DeviceKit DBUS interfaces
  if(HAVE_DEVICEKIT)
    qt4_add_dbus_interface(SOURCES
        dbus/org.freedesktop.UDisks.xml
        dbus/udisks)
    qt4_add_dbus_interface(SOURCES
        dbus/org.freedesktop.UDisks.Device.xml
        dbus/udisksdevice)
  endif(HAVE_DEVICEKIT)

  # MPRIS source
  list(APPEND SOURCES core/mpris.cpp core/mpris1.cpp core/mpris2.cpp)
  list(APPEND HEADERS core/mpris.h   core/mpris1.h   core/mpris2.h)

  # Wiimotedev interface classes
  if(ENABLE_WIIMOTEDEV)
    qt4_add_dbus_interface(SOURCES
        dbus/org.wiimotedev.deviceEvents.xml
        dbus/wiimotedev)

    list(APPEND SOURCES
      wiimotedev/shortcuts.cpp
      wiimotedev/wiimotesettingspage.cpp
      wiimotedev/wiimoteshortcutgrabber.cpp
    )
    list(APPEND HEADERS
      wiimotedev/shortcuts.h
      wiimotedev/wiimotesettingspage.h
      wiimotedev/wiimoteshortcutgrabber.h
    )
  endif(ENABLE_WIIMOTEDEV)

  # DeviceKit lister source
  if(HAVE_DEVICEKIT)
    list(APPEND SOURCES devices/devicekitlister.cpp)
    list(APPEND HEADERS devices/devicekitlister.h)
  endif(HAVE_DEVICEKIT)

  # Gnome Screensaver DBus interface
  list(APPEND SOURCES ui/dbusscreensaver.cpp)
endif(HAVE_DBUS)

# Libgpod device backend
if(HAVE_LIBGPOD)
  include_directories(${LIBGPOD_INCLUDE_DIRS})

  list(APPEND SOURCES devices/gpoddevice.cpp devices/gpodloader.cpp)
  list(APPEND HEADERS devices/gpoddevice.h devices/gpodloader.h)
endif(HAVE_LIBGPOD)

# GIO device backend
if(HAVE_GIO)
  include_directories(${GIO_INCLUDE_DIRS})

  list(APPEND SOURCES devices/giolister.cpp)
  list(APPEND HEADERS devices/giolister.h)
endif(HAVE_GIO)

# CDIO backend and device
if(HAVE_AUDIOCD)
  list(APPEND SOURCES devices/cddadevice.cpp)
  list(APPEND SOURCES devices/cddalister.cpp)

  list(APPEND HEADERS devices/cddadevice.h)
  list(APPEND HEADERS devices/cddalister.h)
endif(HAVE_AUDIOCD)

# libimobiledevice backend and device
if(HAVE_IMOBILEDEVICE)
  include_directories(${IMOBILEDEVICE_INCLUDE_DIRS})
  include_directories(${PLIST_INCLUDE_DIRS})
  include_directories(${PLISTPP_INCLUDE_DIRS})

  list(APPEND SOURCES devices/afcdevice.cpp)
  list(APPEND SOURCES devices/afcfile.cpp)
  list(APPEND SOURCES devices/afctransfer.cpp)
  list(APPEND SOURCES devices/ilister.cpp)
  list(APPEND SOURCES devices/imobiledeviceconnection.cpp)

  list(APPEND HEADERS devices/afcdevice.h)
  list(APPEND HEADERS devices/afcfile.h)
  list(APPEND HEADERS devices/afctransfer.h)
  list(APPEND HEADERS devices/ilister.h)
endif(HAVE_IMOBILEDEVICE)

# mtp device
if(HAVE_LIBMTP)
  include_directories(${LIBMTP_INCLUDE_DIRS})

  list(APPEND SOURCES devices/mtpconnection.cpp)
  list(APPEND SOURCES devices/mtpdevice.cpp)
  list(APPEND SOURCES devices/mtploader.cpp)

  list(APPEND SOURCES devices/mtpconnection.h)
  list(APPEND HEADERS devices/mtpdevice.h)
  list(APPEND HEADERS devices/mtploader.h)
endif(HAVE_LIBMTP)

# Windows media lister
IF(WIN32)
  list(APPEND SOURCES devices/wmdmdevice.cpp)
  list(APPEND SOURCES devices/wmdmlister.cpp)
  list(APPEND SOURCES devices/wmdmloader.cpp)
  list(APPEND SOURCES devices/wmdmprogress.cpp)
  list(APPEND SOURCES devices/wmdmthread.cpp)

  list(APPEND HEADERS devices/wmdmdevice.h)
  list(APPEND HEADERS devices/wmdmlister.h)
  list(APPEND HEADERS devices/wmdmloader.h)
ENDIF(WIN32)

# Mac specific startup stuff
if(APPLE)
  include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/google-breakpad/client/mac/build/Release/Breakpad.framework)
  list(APPEND SOURCES core/mac_startup.mm)
endif(APPLE)

if(HAVE_REMOTE)
  list(APPEND HEADERS
    remote/icesession.h
    remote/portforwarder.h
    remote/remote.h
    remote/remotesettingspage.h
    remote/streampipeline.h
  )
  list(APPEND SOURCES
    remote/icesession.cpp
    remote/portforwarder.h
    remote/remote.cpp
    remote/remotesettingspage.cpp
    remote/streampipeline.cpp
  )
endif(HAVE_REMOTE)

if(LINUX)
  # Hack to add Clementine to the Unity system tray whitelist
  list(APPEND SOURCES core/ubuntuunityhack.cpp)
  list(APPEND HEADERS core/ubuntuunityhack.h)
endif(LINUX)

# OS-specific sources that should be searched for translatable strings even
# if they're not compiled
list(APPEND OTHER_SOURCES
  core/macglobalshortcutbackend.h
  core/macglobalshortcutbackend.mm
  core/modelfuturewatcher.h
  core/mpris.cpp
  core/mpris.h
  core/mpris1.cpp
  core/mpris1.h
  core/mpris2.cpp
  core/mpris2.h
  core/ubuntuunityhack.cpp
  core/ubuntuunityhack.h
  devices/afcdevice.cpp
  devices/afcdevice.h
  devices/afcfile.cpp
  devices/afcfile.h
  devices/afctransfer.cpp
  devices/afctransfer.h
  devices/cddadevice.cpp
  devices/cddadevice.h
  devices/cddalister.cpp
  devices/cddalister.h
  devices/devicekitlister.h
  devices/devicekitlister.cpp
  devices/gpoddevice.cpp
  devices/gpoddevice.h
  devices/gpodloader.cpp
  devices/gpodloader.h
  devices/ilister.cpp
  devices/ilister.h
  devices/imobiledeviceconnection.cpp
  devices/imobiledeviceconnection.h
  devices/mtpconnection.cpp
  devices/mtpconnection.h
  devices/mtpdevice.cpp
  devices/mtpdevice.h
  devices/mtploader.cpp
  devices/mtploader.h
  devices/wmdmlister.cpp
  devices/wmdmlister.h
  devices/wmdmloader.h
  devices/wmdmloader.cpp
  internet/spotifyblobdownloader.cpp
  internet/spotifyblobdownloader.h
  internet/spotifysettingspage.cpp
  internet/spotifysettingspage.h
  ${CMAKE_CURRENT_BINARY_DIR}/ui_spotifysettingspage.h
  internet/spotifysearchplaylisttype.cpp
  internet/spotifysearchplaylisttype.h
  internet/spotifyserver.cpp
  internet/spotifyserver.h
  internet/spotifyservice.cpp
  internet/spotifyservice.h
  internet/spotifyurlhandler.cpp
  internet/spotifyurlhandler.h
  remote/remote.cpp
  remote/remote.h
  remote/remotesettingspage.cpp
  remote/remotesettingspage.h
  ${CMAKE_CURRENT_BINARY_DIR}/ui_remotesettingspage.h
  suggesters/lastfmsuggester.cpp
  suggesters/lastfmsuggester.h
  ui/macsystemtrayicon.h
  ui/macsystemtrayicon.mm
  wiimotedev/wiimotesettingspage.cpp
  wiimotedev/wiimotesettingspage.h
  wiimotedev/wiimoteshortcutgrabber.cpp
  wiimotedev/wiimoteshortcutgrabber.h
  ${CMAKE_CURRENT_BINARY_DIR}/ui_wiimotesettingspage.h
  ${CMAKE_CURRENT_BINARY_DIR}/ui_wiimoteshortcutgrabber.h
  widgets/osd_mac.mm
  widgets/osd_win.cpp
  widgets/osd_x11.cpp
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
               ${CMAKE_CURRENT_BINARY_DIR}/config.h)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/version.h.in
               ${CMAKE_CURRENT_BINARY_DIR}/version.h)

qt4_wrap_cpp(MOC ${HEADERS})
qt4_wrap_ui(UIC ${UI})
qt4_add_resources(QRC ${RESOURCES})

add_pot(
  ${CMAKE_CURRENT_SOURCE_DIR}/translations/header
  ${CMAKE_CURRENT_SOURCE_DIR}/translations/translations.pot
  ${SOURCES} ${MOC} ${UIC} ${OTHER_SOURCES}
)
add_po(PO clementine_
  POT ${CMAKE_CURRENT_SOURCE_DIR}/translations/translations.pot
  LANGUAGES ${LANGUAGES}
  DIRECTORY translations
)

add_library(clementine_lib STATIC
  ${SOURCES}
  ${MOC}
  ${UIC}
  ${QRC}
  ${PO}
)

add_dependencies(clementine_lib pot)


target_link_libraries(clementine_lib
  chardet
  echoprint
  sha2
  ${ECHONEST_LIBRARIES}
  ${GOBJECT_LIBRARIES}
  ${GLIB_LIBRARIES}
  ${TAGLIB_LIBRARIES}
  ${QT_LIBRARIES}
  ${GSTREAMER_BASE_LIBRARIES}
  ${GSTREAMER_LIBRARIES}
  ${GSTREAMER_APP_LIBRARIES}
  ${GSTREAMER_TAG_LIBRARIES}
  ${QTSINGLEAPPLICATION_LIBRARIES}
  ${QTSINGLECOREAPPLICATION_LIBRARIES}
  ${QTIOCOMPRESSOR_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
  dl
  z
)

if(ENABLE_VISUALISATIONS)
  target_link_libraries(clementine_lib ${LIBPROJECTM_LIBRARIES})
endif(ENABLE_VISUALISATIONS)

if(HAVE_LIBLASTFM)
  target_link_libraries(clementine_lib ${LASTFM_LIBRARIES})
endif(HAVE_LIBLASTFM)

if(HAVE_LIBGPOD)
  target_link_libraries(clementine_lib ${LIBGPOD_LIBRARIES})
endif(HAVE_LIBGPOD)

if(HAVE_GIO)
  target_link_libraries(clementine_lib ${GIO_LIBRARIES})
endif(HAVE_GIO)

if(HAVE_AUDIOCD)
  target_link_libraries(clementine_lib ${CDIO_LIBRARIES})
  target_link_libraries(clementine_lib ${GSTREAMER_CDDA_LIBRARIES})
endif(HAVE_AUDIOCD)

if(HAVE_IMOBILEDEVICE)
  target_link_libraries(clementine_lib
    ${IMOBILEDEVICE_LIBRARIES}
    ${PLIST_LIBRARIES}
    ${USBMUXD_LIBRARIES}
    gstafcsrc
  )
  link_directories(${USBMUXD_LIBRARY_DIRS})
endif(HAVE_IMOBILEDEVICE)

if(HAVE_LIBMTP)
  target_link_libraries(clementine_lib ${LIBMTP_LIBRARIES})
endif(HAVE_LIBMTP)

if(HAVE_LIBINDICATE)
  target_link_libraries(clementine_lib ${INDICATEQT_LIBRARIES})
endif(HAVE_LIBINDICATE)

if(HAVE_REMOTE)
  target_link_libraries(clementine_lib ${GLOOX_LIBRARIES})
  link_directories(${GLOOX_LIBRARY_DIRS})
  target_link_libraries(clementine_lib xrme)
  target_link_libraries(clementine_lib portfwd)
  target_link_libraries(clementine_lib qjson)
  target_link_libraries(clementine_lib ${PJSIP_LIBRARIES})
  link_directories(${PJSIP_LIBRARY_DIRS})
endif(HAVE_REMOTE)

if(HAVE_BREAKPAD)
  if (LINUX)
    target_link_libraries(clementine_lib breakpad)
  elseif (APPLE)
    add_dependencies(clementine_lib breakpad)
    target_link_libraries(clementine_lib
        "-F${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/google-breakpad/client/mac/build/Release"
        "-framework Breakpad")
  endif (LINUX)
endif(HAVE_BREAKPAD)

if(HAVE_SPOTIFY)
  target_link_libraries(clementine_lib clementine-spotifyblob-messages)
endif(HAVE_SPOTIFY)

if (APPLE)
  target_link_libraries(clementine_lib
    ${GROWL}
    /System/Library/Frameworks/AppKit.framework
    /System/Library/Frameworks/Carbon.framework
    /System/Library/Frameworks/DiskArbitration.framework
    /System/Library/Frameworks/Foundation.framework
    /System/Library/Frameworks/IOKit.framework
  )
  target_link_libraries(clementine_lib ${SPMEDIAKEYTAP_LIBRARIES})
  if (HAVE_SPARKLE)
    include_directories(${SPARKLE}/Headers)
    target_link_libraries(clementine_lib ${SPARKLE})
  endif (HAVE_SPARKLE)
else (APPLE)
  target_link_libraries(clementine_lib ${QXT_LIBRARIES})
endif (APPLE)

# Link against the qsqlite plugin on windows and mac
if(HAVE_STATIC_SQLITE)
  set(3RDPARTY_SQLITE_LIBRARY qsqlite)
  target_link_libraries(clementine_lib qsqlite)
endif(HAVE_STATIC_SQLITE)

if (WIN32)
  target_link_libraries(clementine_lib
    ${ZLIB_LIBRARIES}
    ${MSWMDM_LIBRARIES}
    ${SAC_SHIM_LIBRARIES}
    ${QTSPARKLE_LIBRARIES}
    qtwin
  )
endif (WIN32)

if (LINUX)
  # Hack: the Gold linker pays attention to the order that libraries are
  # specified on the link line.  -lX11 and -ldl are provided earlier in the link
  # command but they're actually used by libraries that appear after them, so
  # they end up getting ignored.  This appends them to the very end of the link
  # line, ensuring they're always used.
  target_link_libraries(clementine_lib -lX11 -ldl)
endif (LINUX)

add_dependencies(clementine_lib qtsingleapplication)


###############################################################################

set(EXECUTABLE_OUTPUT_PATH ..)

# Show the console window in debug mode on Windows
if (NOT CMAKE_BUILD_TYPE STREQUAL "Debug" AND NOT ENABLE_WIN32_CONSOLE)
  set(CLEMENTINE-WIN32-FLAG WIN32)
endif (NOT CMAKE_BUILD_TYPE STREQUAL "Debug" AND NOT ENABLE_WIN32_CONSOLE)

# resource file for windows
if(WIN32)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../dist/windows/windres.rc.in
                 ${CMAKE_CURRENT_BINARY_DIR}/windres.rc)
  set(CLEMENTINE-WIN32-RESOURCES windres.rc)
endif(WIN32)

add_executable(clementine
  MACOSX_BUNDLE
  ${CLEMENTINE-WIN32-FLAG}
  ${CLEMENTINE-WIN32-RESOURCES}
  main.cpp
)
target_link_libraries(clementine
  clementine_lib
)

# macdeploy.py relies on the blob being built first.
if(HAVE_SPOTIFY_BLOB)
  add_dependencies(clementine clementine-spotifyblob)
endif(HAVE_SPOTIFY_BLOB)

set_target_properties(clementine PROPERTIES
  MACOSX_BUNDLE_INFO_PLIST "../dist/Info.plist"
)

if (APPLE)
  install(FILES ../dist/clementine.icns
    DESTINATION "${CMAKE_BINARY_DIR}/clementine.app/Contents/Resources")
  install(FILES ../dist/qt.conf
    DESTINATION "${CMAKE_BINARY_DIR}/clementine.app/Contents/Resources")
  install(FILES ../dist/sparkle_pub.pem
    DESTINATION "${CMAKE_BINARY_DIR}/clementine.app/Contents/Resources")

  install(DIRECTORY "${QT_QTGUI_LIBRARY_RELEASE}/Versions/Current/Resources/"
    DESTINATION "${CMAKE_BINARY_DIR}/clementine.app/Contents/Resources")

  if (HAVE_SPARKLE)
    install(DIRECTORY "${SPARKLE}/Versions/Current/Resources"
      DESTINATION "${CMAKE_BINARY_DIR}/clementine.app/Contents/Frameworks/Sparkle.framework")
  endif (HAVE_SPARKLE)

  install(DIRECTORY "${GROWL}/Versions/Current/Resources"
    DESTINATION "${CMAKE_BINARY_DIR}/clementine.app/Contents/Frameworks/Growl.framework")

  if (HAVE_BREAKPAD)
    install(DIRECTORY
        ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/google-breakpad/client/mac/build/Release/Breakpad.framework
        DESTINATION "${CMAKE_BINARY_DIR}/clementine.app/Contents/Frameworks"
        PATTERN "*.h" EXCLUDE
        PATTERN "Inspector" PERMISSIONS OWNER_EXECUTE OWNER_READ
                                        GROUP_EXECUTE GROUP_READ
                                        WORLD_READ WORLD_EXECUTE
        PATTERN "crash_report_sender" PERMISSIONS OWNER_EXECUTE OWNER_READ
                                                  GROUP_EXECUTE GROUP_READ
                                                  WORLD_READ WORLD_EXECUTE)
  endif (HAVE_BREAKPAD)

  add_custom_command(TARGET clementine
    POST_BUILD
    COMMAND
    ${CMAKE_CURRENT_SOURCE_DIR}/../dist/macdeploy.py ${PROJECT_BINARY_DIR}/clementine.app -f
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
  )

  add_custom_command(OUTPUT ${PROJECT_BINARY_DIR}/clementine.breakpad
    COMMAND
    ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/google-breakpad/tools/mac/dump_syms/build/Release/dump_syms
    -a i386 ${PROJECT_BINARY_DIR}/clementine.app/Contents/MacOS/clementine
    > ${PROJECT_BINARY_DIR}/clementine.breakpad
    DEPENDS clementine)
  add_custom_target(clementine-breakpad
    DEPENDS ${PROJECT_BINARY_DIR}/clementine.breakpad)


  add_custom_command(
    OUTPUT ${PROJECT_BINARY_DIR}/clementine-${CLEMENTINE_VERSION_RPM}.dmg
    ${CMAKE_COMMAND} -E remove -f ${PROJECT_BINARY_DIR}/clementine-${CLEMENTINE_VERSION_RPM}.dmg
    COMMAND macdeployqt clementine.app -no-plugins -dmg
    COMMAND ${CMAKE_COMMAND} -E rename
      ${PROJECT_BINARY_DIR}/clementine.dmg
      ${PROJECT_BINARY_DIR}/clementine-${CLEMENTINE_VERSION_RPM}.dmg
    DEPENDS clementine
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
  )
  add_custom_target(dmg
    DEPENDS ${PROJECT_BINARY_DIR}/clementine-${CLEMENTINE_VERSION_RPM}.dmg)
else (APPLE)
  install(TARGETS clementine
    RUNTIME DESTINATION bin
  )
endif (APPLE)
