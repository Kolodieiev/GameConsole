#pragma once
#include <Arduino.h>

#include "meow/util/files/FileManager.h"

using namespace meow;

class PlaylistManager : public FileManager
{
public:
    bool updatePlaylists();
    bool updateTracklists(const char *playlist_name);
    uint16_t getPlaylistSize(const char *playlist_name);
    bool removeTrack(const char *playlist_name, const char *track_name);

    std::vector<String> getPlaylists();
    std::vector<String> getTracks(const char *playlist_name, uint8_t start_pos, uint8_t size);

    String getTrackPath(const char *playlist_name, const char *track_name);
    String getNextTrackName(const char *playlist_name, uint16_t track_pos);
    String getPrevTrackName(const char *playlist_name, uint16_t track_pos);
};
