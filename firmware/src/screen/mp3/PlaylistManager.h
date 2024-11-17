#pragma once
#pragma GCC optimize("O3")

#include <Arduino.h>

#include "meow/manager/files/FileManager.h"

using namespace meow;

class PlaylistManager : public FileManager
{
public:
    bool updatePlaylists(std::function<void()> on_finish);
    bool updateTracklists(const char *playlist_name, std::function<void()> on_finish);
    bool removeTrack(const char *playlist_name, const char *track_name, std::function<void()> on_finish);

    uint16_t getPlaylistSize(const char *playlist_name);

    std::vector<String> getPlaylists();
    std::vector<String> getTracks(const char *playlist_name, uint8_t start_pos, uint8_t size);

    String getLibDbPath(const char *playlist_name);
    String getTrackPath(const char *playlist_name, const char *track_name);
    String getNextTrackName(const char *playlist_name, uint16_t track_pos);
    String getPrevTrackName(const char *playlist_name, uint16_t track_pos);
};
