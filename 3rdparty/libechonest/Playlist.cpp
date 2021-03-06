/****************************************************************************************
 * Copyright (c) 2010 Leo Franchi <lfranchi@kde.org>                                    *
 *                                                                                      *
 * This program is free software; you can redistribute it and/or modify it under        *
 * the terms of the GNU General Public License as published by the Free Software        *
 * Foundation; either version 2 of the License, or (at your option) any later           *
 * version.                                                                             *
 *                                                                                      *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY      *
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A      *
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.             *
 *                                                                                      *
 * You should have received a copy of the GNU General Public License along with         *
 * this program.  If not, see <http://www.gnu.org/licenses/>.                           *
 ****************************************************************************************/

#include "Playlist.h"
#include "Playlist_p.h"
#include "Parsing_p.h"

Echonest::DynamicPlaylist::DynamicPlaylist()
    : d( new DynamicPlaylistData )
{

}

Echonest::DynamicPlaylist::DynamicPlaylist(const Echonest::DynamicPlaylist& other)
    : d( other.d )
{

}

Echonest::DynamicPlaylist::~DynamicPlaylist()
{

}


Echonest::DynamicPlaylist& Echonest::DynamicPlaylist::operator=(const Echonest::DynamicPlaylist& playlist)
{
    d = playlist.d;
    return *this;
}

QNetworkReply* Echonest::DynamicPlaylist::start(const Echonest::DynamicPlaylist::PlaylistParams& params)
{
    // params are the same, if user passes in format parsing will throw, but it should be expected..
    return generateInternal( params, "dynamic" );
}

Echonest::Song Echonest::DynamicPlaylist::parseStart(QNetworkReply* reply) throw( Echonest::ParseError )
{
    Echonest::Parser::checkForErrors( reply );
    
    QXmlStreamReader xml( reply->readAll() );
    
    Echonest::Parser::readStatus( xml );
    d->sessionId = Echonest::Parser::parsePlaylistSessionId( xml );
    Echonest::SongList songs = Echonest::Parser::parseSongList( xml );
    if( !songs.size() == 1 )
        throw new Echonest::ParseError( UnknownParseError );
    
    d->currentSong = songs.front();
    
    return d->currentSong;
}

QByteArray Echonest::DynamicPlaylist::sessionId() const
{
    return d->sessionId;
}

void Echonest::DynamicPlaylist::setSessionId(const QByteArray& id)
{
    d->sessionId = id;
}

Echonest::Song Echonest::DynamicPlaylist::currentSong() const
{
    return d->currentSong;
}

void Echonest::DynamicPlaylist::setCurrentSong(const Echonest::Song& song)
{
    d->currentSong = song;
}

QNetworkReply* Echonest::DynamicPlaylist::fetchNextSong(int rating)
{
    QUrl url = Echonest::baseGetQuery( "playlist", "dynamic" );
    url.addEncodedQueryItem( "session_id", d->sessionId );
    
    if( rating > 0 )
        url.addEncodedQueryItem( "rating", QByteArray::number( rating ) );
    
    return Echonest::Config::instance()->nam()->get( QNetworkRequest( url ) );
    
}


Echonest::Song Echonest::DynamicPlaylist::parseNextSong(QNetworkReply* reply)
{
    return parseStart( reply );
}

QNetworkReply* Echonest::DynamicPlaylist::staticPlaylist(const Echonest::DynamicPlaylist::PlaylistParams& params)
{
    return Echonest::DynamicPlaylist::generateInternal( params, "static" );
}

Echonest::SongList Echonest::DynamicPlaylist::parseStaticPlaylist(QNetworkReply* reply) throw( Echonest::ParseError )
{
    Echonest::Parser::checkForErrors( reply );
    
    QXmlStreamReader xml( reply->readAll() );
    
    Echonest::Parser::readStatus( xml );
    
    Echonest::SongList songs = Echonest::Parser::parseSongList( xml );
    return songs;
}

QNetworkReply* Echonest::DynamicPlaylist::generateInternal(const Echonest::DynamicPlaylist::PlaylistParams& params, const QByteArray& type)
{
    QUrl url = Echonest::baseGetQuery( "playlist", type );
    
    Echonest::DynamicPlaylist::PlaylistParams::const_iterator iter = params.constBegin();
    for( ; iter < params.constEnd(); ++iter ) {
        if( iter->first == Format ) // If it's a format, we have to remove the xml format we automatically specify
            url.removeEncodedQueryItem( "format" );
        
        if( iter->first == Type ) { // convert type enum to string
            switch( static_cast<Echonest::DynamicPlaylist::ArtistTypeEnum>( iter->second.toInt() ) ) 
            {
            case ArtistType:
                url.addEncodedQueryItem(  playlistParamToString( iter->first ), "artist" );
                break;
            case ArtistRadioType:
                url.addEncodedQueryItem(  playlistParamToString( iter->first ), "artist-radio" );
                break;
            case ArtistDescriptionType:
                url.addEncodedQueryItem(  playlistParamToString( iter->first ), "artist-description" );
                break;
            }
                
        } else if( iter->first == Sort ) {
            url.addEncodedQueryItem( playlistParamToString( iter->first ), playlistSortToString( static_cast<Echonest::DynamicPlaylist::SortingType>( iter->second.toInt() ) ) );
        } else if( iter->first == Pick ) {
            url.addEncodedQueryItem( playlistParamToString( iter->first ), playlistArtistPickToString( static_cast<Echonest::DynamicPlaylist::ArtistPick>( iter->second.toInt() ) ) );
        } else if( iter->first == SongInformation ){
            Echonest::Song::addQueryInformation( url, Echonest::Song::SongInformation( iter->second.value< Echonest::Song::SongInformation >() ) );
        } else {
            url.addQueryItem( QLatin1String( playlistParamToString( iter->first ) ), iter->second.toString().replace( QLatin1Char( ' ' ), QLatin1Char( '+' ) ) );
        }
    }
    
    qDebug() << "Creating playlist URL" << url;
    return Echonest::Config::instance()->nam()->get( QNetworkRequest( url ) );
}


QByteArray Echonest::DynamicPlaylist::playlistParamToString(Echonest::DynamicPlaylist::PlaylistParam param)
{
    switch( param )
    {
        case Echonest::DynamicPlaylist::Type :
            return "type";
        case Echonest::DynamicPlaylist::Format :
            return "format";
        case Echonest::DynamicPlaylist::Pick:
            return "artist_pick";
        case Echonest::DynamicPlaylist::Variety :
            return "variety";
        case Echonest::DynamicPlaylist::ArtistId :
            return "artist_id";
        case Echonest::DynamicPlaylist::Artist :
            return "artist";
        case Echonest::DynamicPlaylist::SongId :
            return "song_id";
        case Echonest::DynamicPlaylist::Description :
            return "description";
        case Echonest::DynamicPlaylist::Results :
            return "results";
        case Echonest::DynamicPlaylist::MaxTempo :
            return "max_tempo";
        case Echonest::DynamicPlaylist::MinTempo :
            return "min_tempo";
        case Echonest::DynamicPlaylist::MaxDuration :
            return "max_duration";
        case Echonest::DynamicPlaylist::MinDuration :
            return "min_duration";
        case Echonest::DynamicPlaylist::MaxLoudness :
            return "max_loudness";
        case Echonest::DynamicPlaylist::MinLoudness :
            return "min_loudness";
        case Echonest::DynamicPlaylist::ArtistMaxFamiliarity :
            return "artist_max_familiarity";
        case Echonest::DynamicPlaylist::ArtistMinFamiliarity :
            return "artist_min_familiarity";
        case Echonest::DynamicPlaylist::ArtistMaxHotttnesss :
            return "artist_max_hotttnesss";
        case Echonest::DynamicPlaylist::ArtistMinHotttnesss :
            return "artist_min_hotttnesss";
        case Echonest::DynamicPlaylist::SongMaxHotttnesss :
            return "song_max_hotttnesss";
        case Echonest::DynamicPlaylist::SongMinHotttnesss :
            return "song_min_hotttnesss";
        case Echonest::DynamicPlaylist::ArtistMinLongitude :
            return "artist_min_longitude";
        case Echonest::DynamicPlaylist::ArtistMaxLongitude :
            return "artist_max_longitude";
        case Echonest::DynamicPlaylist::ArtistMinLatitude  :
            return "artist_min_latitude";
        case Echonest::DynamicPlaylist::ArtistMaxLatitude :
            return "artist_max_latitude";
        case Echonest::DynamicPlaylist::Mode :
            return "mode";
        case Echonest::DynamicPlaylist::Key :
            return "key";
        case Echonest::DynamicPlaylist::SongInformation:
            return "bucket";
        case Echonest::DynamicPlaylist::Sort :
            return "sort";
        case Echonest::DynamicPlaylist::Limit :
            return "limit";
        case Echonest::DynamicPlaylist::Audio :
            return "audio";
        case Echonest::DynamicPlaylist::DMCA :
            return "dmca";
    }
    return QByteArray();
}

QByteArray Echonest::DynamicPlaylist::playlistArtistPickToString(Echonest::DynamicPlaylist::ArtistPick pick)
{
    switch( pick )
    {
        case PickSongHotttnesssAscending:
            return "song_hotttnesss-asc";
        case PickTempoAscending:
            return "tempo-asc";
        case PickDurationAscending:
            return "duration-asc";
        case PickLoudnessAscending:
            return "loudness-asc";
        case PickModeAscending:
            return "mode-asc";
        case PickKeyAscending:
            return "key-asc";
        case PickSongHotttnesssDescending:
            return "song_hotttnesss-desc";
        case PickTempoDescending:
            return "tempo-desc";
        case PickDurationDescending:
            return "duration-desc";
        case PickLoudnessDescending:
            return "loudness-desc";
        case PickModeDescending:
            return "mode-desc";
        case PickKeyDescending:
            return "key-desc";
    }
    return QByteArray();
}

QByteArray Echonest::DynamicPlaylist::playlistSortToString(Echonest::DynamicPlaylist::SortingType sorting)
{
    switch( sorting )
    {
        case SortTempoAscending:
            return "tempo-asc";
        case SortTempoDescending:
            return "tempo-desc";
        case SortDurationAscending:
            return "duration-asc";
        case SortDurationDescending:
            return "duration-desc";
        case SortArtistFamiliarityAscending:
            return "artist_familiarity-asc";
        case SortArtistFamiliarityDescending:
            return "artist_familiarity-desc";
        case SortArtistHotttnessAscending:
            return "artist_hotttnesss-asc";
        case SortArtistHotttnessDescending:
            return "artist_hotttnesss-desc";
        case SortSongHotttnesssAscending:
            return "song_hotttnesss-asc";
        case SortSongHotttnesssDescending:
            return "song_hotttnesss-desc";
        case SortLatitudeAscending:
            return "latitude-asc";
        case SortLatitudeDescending:
            return "latitude-desc";
        case SortLongitudeAscending:
            return "longitude-asc";
        case SortLongitudeDescending:
            return "longitude-desc";
        case SortModeAscending:
            return "mode-asc";
        case SortModeDescending:
            return "mode-desc";
        case SortKeyAscending:
            return "key-asc";
        case SortKeyDescending:
            return "key-desc";
    }
    return QByteArray();
}


QDebug Echonest::operator<<(QDebug d, const Echonest::DynamicPlaylist& playlist)
{
    d << QString::fromLatin1( "DynamicPlaylist(%1, %2)" ).arg( QLatin1String( playlist.sessionId() ), playlist.currentSong().toString() );
    return d.maybeSpace();
}
