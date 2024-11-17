#pragma GCC optimize("O3")
#include "PlaylistManager.h"

const char ROOT_PATH[] = "/music";
const char PLAYLIST_LIB_NAME[] = "/playlist.lib";
const char AUDIO_EXT[] = ".mp3";

bool PlaylistManager::updatePlaylists(std::function<void()> on_finish)
{
    String db_path = ROOT_PATH;
    db_path += PLAYLIST_LIB_NAME;

    return indexDirs(ROOT_PATH, db_path.c_str(), on_finish);
}

bool PlaylistManager::updateTracklists(const char *playlist_name, std::function<void()> on_finish)
{
    String dir_path = ROOT_PATH;
    dir_path += "/";
    dir_path += playlist_name;

    String db_path = getLibDbPath(playlist_name);

    return indexFilesExt(dir_path.c_str(), AUDIO_EXT, db_path.c_str(), on_finish);
}

uint16_t PlaylistManager::getPlaylistSize(const char *playlist_name)
{
    String db_path = getLibDbPath(playlist_name);

    return getDBSize(db_path.c_str());
}

bool PlaylistManager::removeTrack(const char *playlist_name, const char *track_name, std::function<void()> on_finish)
{
    String db_path = getTrackPath(playlist_name, track_name);

    return startRemoving(db_path.c_str(), on_finish);
}

std::vector<String> PlaylistManager::getPlaylists()
{
    String db_path = ROOT_PATH;
    db_path += PLAYLIST_LIB_NAME;

    return readFilesFromDB(db_path.c_str(), 0, 100);
}

std::vector<String> PlaylistManager::getTracks(const char *playlist_name, uint8_t start_pos, uint8_t size)
{
    String db_path = getLibDbPath(playlist_name);

    return readFilesFromDB(db_path.c_str(), start_pos, size);
}

String PlaylistManager::getNextTrackName(const char *playlist_name, uint16_t track_pos)
{
    String db_path = getLibDbPath(playlist_name);

    std::vector<String> track = readFilesFromDB(db_path.c_str(), track_pos + 1, 1);

    if (track.size() > 0)
        return track[0];

    return "";
}

String PlaylistManager::getPrevTrackName(const char *playlist_name, uint16_t track_pos)
{
    if (track_pos == 0)
        return "";

    String db_path = getLibDbPath(playlist_name);

    std::vector<String> track = readFilesFromDB(db_path.c_str(), track_pos - 1, 1);

    if (track.size() > 0)
        return track[0];

    return "";
}

String PlaylistManager::getLibDbPath(const char *playlist_name)
{
    String path = ROOT_PATH;
    path += "/";
    path += playlist_name;
    path += PLAYLIST_LIB_NAME;

    return path;
}

String PlaylistManager::getTrackPath(const char *playlist_name, const char *track_name)
{
    String path = ROOT_PATH;
    path += "/";
    path += playlist_name;
    path += "/";
    path += track_name;

    return path;
}
