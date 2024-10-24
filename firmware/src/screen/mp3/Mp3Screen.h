#pragma once
#pragma GCC optimize("Ofast")

#include <Arduino.h>

#include "meow/lib/DS3231/DS3231.h"
#include "meow/driver/audio/mp3/Audio.h"
#include "meow/util/preferences/PrefUtil.h"

#include "meow/ui/screen/IScreen.h"
#include "meow/ui/widget/scrollbar/ScrollBar.h"
#include "meow/ui/widget/menu/FixedMenu.h"
#include "meow/ui/widget/menu/DynamicMenu.h"
#include "meow/ui/widget/progress/ProgressBar.h"
#include "meow/ui/widget/image/Image.h"

#include "./PlaylistManager.h"

using namespace meow;

class Mp3Screen : public IScreen, public IItemsLoader
{
public:
    Mp3Screen(GraphicsDriver &display);
    virtual ~Mp3Screen();

protected:
    virtual void loop() override;
    virtual void update() override;

    virtual std::vector<MenuItem *> loadPrev(uint8_t size, uint16_t current_ID) override;
    virtual std::vector<MenuItem *> loadNext(uint8_t size, uint16_t current_ID) override;

private:
    enum Mode : uint8_t
    {
        MODE_PLST_SEL = 0,
        MODE_PLST_UPD,
        MODE_TRACK_SEL,
        MODE_AUDIO_PLAY,
        MODE_PLST_MENU
    };

    enum PlMenuItemsID : uint8_t
    {
        ID_ITEM_DEL = 1,
        ID_ITEM_UPD,
    };

    enum Widget_ID : uint8_t
    {
        ID_NAVBAR = 1,
        ID_F_MENU,
        ID_PL_MENU,
        ID_D_MENU,
        ID_SCROLL,
        ID_TRACK_NAME,
        ID_TRACK_POS,
        ID_TRACK_TIME,
        ID_PLAY_BTN,
        ID_UPD_LBL,
        ID_VOLUME_LBL,
        ID_VOLUME_IMG,
        ID_FORWARD_IMG,
        ID_REWIND_IMG,
        ID_PROGRESS,
        ID_TIME_LBL,
        ID_TIME_IMG,
    };

    const uint8_t PIN_I2S_BCLK{21};
    const uint8_t PIN_I2S_LRC{48};
    const uint8_t PIN_I2S_DOUT{47};

    const uint8_t PLAYLIST_ITEMS_NUM{6};
    const uint8_t TRACKS_ITEMS_NUM{10};

    PrefUtil _preferences;

    uint8_t _brightness;
    bool _is_locked{false};

    Mode _mode{MODE_PLST_SEL};
    bool _is_tracklist_upd{false};

    ScrollBar *_scrollbar;
    FixedMenu *_fixed_menu;
    FixedMenu *_pl_menu;
    DynamicMenu *_dynamic_menu;

    Label *_track_name_lbl;
    Image *_play_btn;
    Label *_track_pos_lbl;
    Label *_track_time_lbl;
    Label *_volume_lbl;
    ProgressBar *_progress;

    Label *_time_lbl;
    DS3231 _watch;
    bool _watch_inited{false};
    unsigned long _upd_time_time{0};
    DS3231DateTime _temp_date_time;

    Label *_upd_lbl;
    uint8_t _upd_counter{0};

    const uint16_t UPD_TRACK_INF_INTERVAL{1000};
    const uint16_t UPD_TIME_INTERVAL{10000};
    unsigned long _upd_inf_time{0};

    Audio _audio;
    PlaylistManager _pl_manager;

    String _playlist_name;
    String _track_name;
    uint8_t _volume;
    uint16_t _track_pos{0};
    int32_t _track_time{-1};

    //
    uint8_t _attempt_to_play_next_counter{0};
    bool _is_new_track{true};
    bool _is_playing{false};
    //
    void savePref();
    //
    void showPlaylists();
    void fillPlaylists(Menu *menu_ptr, uint16_t from_id);
    std::vector<MenuItem *> getTracksItems(uint8_t size, uint16_t from_id);

    void showTracks(uint16_t pos = 0);
    void showPlaying();
    void showUpdating();

    //
    bool playTrack(bool contn = false);
    bool playNext();
    bool playPrev();

    void volumeUp();
    void volumeDown();

    //
    void setStopState();

    bool updateTrackDuration();
    void updateTrackPos();

    void up();
    void down();

    void changeBackLight();

    void left();
    void leftPressed();

    void right();
    void rightPressed();

    void ok();

    void back();
    void backPressed();

    void showPlMenu();
    void hidePlMenu();
    //
    void updateTime();
};