#include "PlaylistManager.h"

const char ROOT_PATH[] = "/music";
const char PLAYLIST_LIB_NAME[] = "/playlist.lib";
const char AUDIO_EXT[] = ".mp3";

bool PlaylistManager::updatePlaylists()
{
    String db_path = ROOT_PATH;
    db_path += PLAYLIST_LIB_NAME;
    return indexDirs(ROOT_PATH, db_path.c_str());
}

bool PlaylistManager::updateTracklists(const char *playlist_name)
{
    String dir_path = ROOT_PATH;
    dir_path += "/";
    dir_path += playlist_name;

    String db_path = ROOT_PATH;
    db_path += "/";
    db_path += playlist_name;
    db_path += PLAYLIST_LIB_NAME;

    return indexFilesExt(dir_path.c_str(), AUDIO_EXT, db_path.c_str());
}

uint16_t PlaylistManager::getPlaylistSize(const char *playlist_name)
{
    String db_path = ROOT_PATH;
    db_path += "/";
    db_path += playlist_name;
    db_path += PLAYLIST_LIB_NAME;

    return getDBSize(db_path.c_str());
}

bool PlaylistManager::removeTrack(const char *playlist_name, const char *track_name)
{
    String db_path = ROOT_PATH;
    db_path += "/";
    db_path += playlist_name;
    db_path += "/";
    db_path += track_name;

    return SD.remove(db_path.c_str());
}

std::vector<String> PlaylistManager::getPlaylists()
{
    String db_path = ROOT_PATH;
    db_path += PLAYLIST_LIB_NAME;

    return readFilesFromDB(db_path.c_str(), 0, 0);
}

std::vector<String> PlaylistManager::getTracks(const char *playlist_name, uint8_t start_pos, uint8_t size)
{
    String db_path = ROOT_PATH;
    db_path += "/";
    db_path += playlist_name;
    db_path += PLAYLIST_LIB_NAME;

    return readFilesFromDB(db_path.c_str(), start_pos, size);
}

String PlaylistManager::getTrackPath(const char *playlist_name, const char *track_name)
{
    String path = ROOT_PATH;
    path += "/";
    path += playlist_name;
    path += "/";
    path += track_name;

    if (!SD.exists(path.c_str()))
        return "";

    return path;
}

String PlaylistManager::getNextTrackName(const char *playlist_name, uint16_t track_pos)
{
    String db_path = ROOT_PATH;
    db_path += "/";
    db_path += playlist_name;
    db_path += PLAYLIST_LIB_NAME;

    std::vector<String> track = readFilesFromDB(db_path.c_str(), track_pos + 1, 1);

    if (track.size() > 0)
        return track[0];

    return "";
}

String PlaylistManager::getPrevTrackName(const char *playlist_name, uint16_t track_pos)
{
    if (track_pos == 0)
        return "";

    String db_path = ROOT_PATH;
    db_path += "/";
    db_path += playlist_name;
    db_path += PLAYLIST_LIB_NAME;

    std::vector<String> track = readFilesFromDB(db_path.c_str(), track_pos - 1, 1);

    if (track.size() > 0)
        return track[0];

    return "";
}