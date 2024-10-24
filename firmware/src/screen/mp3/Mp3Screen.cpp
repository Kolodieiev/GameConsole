#pragma GCC optimize("Ofast")

#include "Mp3Screen.h"

#include "../resources/color.h"
#include "../resources/string.h"
#include "../resources/const.h"

#include "./res/play.h"
#include "./res/pause.h"
#include "./res/speaker.h"
#include "./res/forward.h"
#include "./res/rewind.h"
#include "./res/clock.h"

#include "../WidgetCreator.h"
#include "meow/ui/widget/layout/EmptyLayout.h"
#include "meow/ui/widget/navbar/NavBar.h"

#include "meow/util/sd/SdUtil.h"
#include "meow/util/display/DisplayUtil.h"

const char STR_PLAYLIST_PREF[] = "Playlist";
const char STR_TRACK_NAME_PREF[] = "TrackName";
const char STR_VOLUME_PREF[] = "Volume";
const char STR_TRACK_POS_PREF[] = "TrackPos";
const char STR_TRACK_TIME_PREF[] = "TrackTime";

Mp3Screen::Mp3Screen(GraphicsDriver &display) : IScreen(display)
{
    SdUtil sd;
    sd.begin();

    _watch_inited = _watch.begin();

    if (!_watch_inited)
        log_e("Помилка ініціалізації RTC");

    String bright = _preferences.get(STR_PREF_BRIGHT);
    if (bright.isEmpty())
    {
        _brightness = 100;
    }
    else
    {
        _brightness = atoi(bright.c_str());

        if (_brightness == 0)
            _brightness = 100;
    }

    _volume = atoi(_preferences.get(STR_VOLUME_PREF).c_str());
    _track_pos = atoi(_preferences.get(STR_TRACK_POS_PREF).c_str());
    _track_time = atoi(_preferences.get(STR_TRACK_TIME_PREF).c_str());
    _playlist_name = _preferences.get(STR_PLAYLIST_PREF);
    _track_name = _preferences.get(STR_TRACK_NAME_PREF);

    if (_volume == 0)
        _volume = 5;

    _audio.setVolumeSteps(31);
    _audio.setVolume(_volume);

    String mono_mode = _preferences.get(STR_PREF_MONO_AUDIO);
    if (mono_mode == "1")
        _audio.forceMono(true);

    _audio.setPinout(PIN_I2S_BCLK, PIN_I2S_LRC, PIN_I2S_DOUT);

    showPlaylists();
}

Mp3Screen::~Mp3Screen()
{
    SdUtil sd;
    sd.end();
}

void Mp3Screen::loop()
{
    _audio.loop();
}

void Mp3Screen::update()
{
    if (_input.isPressed(KeyID::KEY_OK))
    {
        _input.lock(KeyID::KEY_OK, 1000);
        changeBackLight();
    }
    else if (_input.isPressed(KeyID::KEY_RIGHT))
    {
        _input.lock(KeyID::KEY_RIGHT, 1000);
        rightPressed();
    }
    else if (_input.isPressed(KeyID::KEY_LEFT))
    {
        _input.lock(KeyID::KEY_LEFT, 1000);
        leftPressed();
    }
    else if (_input.isHolded(KeyID::KEY_UP))
    {
        _input.lock(KeyID::KEY_UP, 150);
        up();
    }
    else if (_input.isHolded(KeyID::KEY_DOWN))
    {
        _input.lock(KeyID::KEY_DOWN, 150);
        down();
    }
    else if (_input.isReleased(KeyID::KEY_RIGHT))
    {
        _input.lock(KeyID::KEY_RIGHT, 100);
        right();
    }
    else if (_input.isReleased(KeyID::KEY_LEFT))
    {
        _input.lock(KeyID::KEY_LEFT, 100);
        left();
    }
    else if (_input.isReleased(KeyID::KEY_OK))
    {
        _input.lock(KeyID::KEY_OK, 100);
        ok();
    }
    else if (_input.isReleased(KeyID::KEY_BACK))
    {
        _input.lock(KeyID::KEY_BACK, 100);
        back();
    }
    else if (_input.isPressed(KeyID::KEY_OK))
    {
        _input.lock(KeyID::KEY_OK, 1000);
        showPlMenu();
    }
    else if (_input.isPressed(KeyID::KEY_BACK))
    {
        _input.lock(KeyID::KEY_BACK, 2500);
        backPressed();
    }

    if (_mode == MODE_AUDIO_PLAY)
    {
        if (_audio.isRunning())
        {
            if (_is_locked)
                return;

            if (_is_new_track && updateTrackDuration())
            {
                _track_name_lbl->setText(_track_name);
                _is_new_track = false;
            }

            if (millis() - _upd_inf_time > UPD_TRACK_INF_INTERVAL)
            {
                updateTrackPos();
                _upd_inf_time = millis();
            }
        }
        // Якщо трек скінчився самостійно
        else if (_is_playing)
        {
            // Намагаємося перемкнути
            if (playNext())
            {
                _is_new_track = true;
            }
            // Якщо не вдалося змінити трек з 3х спроб, зупинити плеєр
            else if (_attempt_to_play_next_counter == 3)
            {
                setStopState();
            }
            else
                ++_attempt_to_play_next_counter;
        }

        if (!_is_locked && millis() - _upd_time_time > UPD_TIME_INTERVAL)
        {
            updateTime();
            _upd_time_time = millis();
        }
    }
    else if (_mode == MODE_PLST_UPD)
    {
        if (_pl_manager.isTaskDone())
        {
            if (_is_tracklist_upd)
            {
                if (_track_pos > 0)
                {
                    uint16_t list_size = _pl_manager.getPlaylistSize(_playlist_name.c_str());

                    if (_track_pos >= list_size)
                        _track_pos = list_size;
                }

                showTracks(_track_pos);
            }
            else
                showPlaylists();
        }
        else if ((millis() - _upd_inf_time) > UPD_TRACK_INF_INTERVAL)
        {
            String upd_txt = STR_UPDATING;
            if (_upd_counter > 3)
                _upd_counter = 0;
            else
                ++_upd_counter;

            for (uint8_t i{0}; i < _upd_counter; ++i)
                upd_txt += ".";

            _upd_lbl->setText(upd_txt);
            _upd_inf_time = millis();
        }
    }
}

std::vector<MenuItem *> Mp3Screen::getTracksItems(uint8_t size, uint16_t current_ID)
{
    std::vector<String> tracks = _pl_manager.getTracks(_playlist_name.c_str(), current_ID, size);

    WidgetCreator creator{_display};

    std::vector<MenuItem *> ret;
    ret.reserve(tracks.size());

    for (size_t i = 0; i < tracks.size(); ++i)
    {
        ++current_ID;

        MenuItem *item = creator.getMenuItem(current_ID);
        ret.push_back(item);

        Label *item_lbl = creator.getItemLabel(tracks[i].c_str(), 2, 1);
        item->setLbl(item_lbl);
    }

    return ret;
}

std::vector<MenuItem *> Mp3Screen::loadPrev(uint8_t size, uint16_t current_ID)
{
    // Не отримали ID
    if (!current_ID)
        return std::vector<MenuItem *>();

    uint16_t item_pos = current_ID - 1;

    // Вже перший елемент
    if (!item_pos)
        return std::vector<MenuItem *>();

    // Вирахувати першу позицію звідки потрібно читати файл.
    if (current_ID > size)
        item_pos = current_ID - size - 1;
    else
    {
        item_pos = 0;
        // Вирівняти скролбар, якщо меню було завантажене не з першого елемента
        _scrollbar->setValue(current_ID);
    }

    return getTracksItems(size, item_pos);
}

std::vector<MenuItem *> Mp3Screen::loadNext(uint8_t size, uint16_t current_ID)
{
    if (!current_ID)
        return std::vector<MenuItem *>();

    return getTracksItems(size, current_ID);
}

void Mp3Screen::savePref()
{
    _preferences.set(STR_VOLUME_PREF, String(_volume).c_str());
    _preferences.set(STR_TRACK_POS_PREF, String(_track_pos).c_str());
    _preferences.set(STR_TRACK_TIME_PREF, String(_track_time).c_str());
    _preferences.set(STR_PLAYLIST_PREF, _playlist_name.c_str());
    _preferences.set(STR_TRACK_NAME_PREF, _track_name.c_str());
}

void Mp3Screen::showPlaylists()
{
    _mode = MODE_PLST_SEL;

    WidgetCreator creator{_display};

    EmptyLayout *layout = creator.getEmptyLayout();
    setLayout(layout);

    NavBar *navbar = creator.getNavbar(ID_NAVBAR, STR_SELECT, "", STR_BACK);
    layout->addWidget(navbar);

    _fixed_menu = new FixedMenu(ID_F_MENU, _display);
    layout->addWidget(_fixed_menu);
    _fixed_menu->setBackColor(COLOR_MENU_ITEM);
    _fixed_menu->setWidth(_display.width() - SCROLLBAR_WIDTH);
    _fixed_menu->setHeight(_display.height() - NAVBAR_HEIGHT - 2);
    _fixed_menu->setItemHeight((_display.height() - NAVBAR_HEIGHT - 1) / PLAYLIST_ITEMS_NUM);

    _scrollbar = new ScrollBar(ID_SCROLL, _display);
    layout->addWidget(_scrollbar);
    _scrollbar->setWidth(SCROLLBAR_WIDTH);
    _scrollbar->setHeight(_display.height() - NAVBAR_HEIGHT);
    _scrollbar->setPos(_display.width() - SCROLLBAR_WIDTH, 0);

    if (!_track_name.isEmpty())
    {
        MenuItem *cont_item = creator.getMenuItem(1);
        _fixed_menu->addItem(cont_item);

        Label *cont_lbl = creator.getItemLabel(STR_CONTINUE, 4, 2);
        cont_item->setLbl(cont_lbl);
    }

    fillPlaylists(_fixed_menu, 3);

    MenuItem *upd_item = creator.getMenuItem(2);
    _fixed_menu->addItem(upd_item);

    Label *upd_lbl = creator.getItemLabel(STR_UPD_LISTS, 4, 2);
    upd_item->setLbl(upd_lbl);

    _scrollbar->setMax(_fixed_menu->getSize());
}

void Mp3Screen::fillPlaylists(Menu *menu_ptr, uint16_t from_id)
{
    std::vector<String> menu_list = _pl_manager.getPlaylists();

    WidgetCreator creator{_display};

    for (uint8_t i = 0; i < menu_list.size(); ++i)
    {
        MenuItem *upd_item = creator.getMenuItem(from_id);
        menu_ptr->addItem(upd_item);

        Label *upd_lbl = creator.getItemLabel(menu_list[i].c_str(), 4, 2);
        upd_item->setLbl(upd_lbl);

        ++from_id;
    }
}

void Mp3Screen::showTracks(uint16_t pos)
{
    _mode = MODE_TRACK_SEL;

    WidgetCreator creator{_display};

    EmptyLayout *layout = creator.getEmptyLayout();
    setLayout(layout);

    layout->addWidget(creator.getNavbar(ID_NAVBAR, STR_SELECT, "", STR_BACK));

    _dynamic_menu = creator.getDynamicMenu(ID_D_MENU, this);
    layout->addWidget(_dynamic_menu);
    _dynamic_menu->setItemHeight((_display.height() - NAVBAR_HEIGHT - 2) / TRACKS_ITEMS_NUM);
    _dynamic_menu->setWidth(_display.width() - SCROLLBAR_WIDTH);
    _dynamic_menu->setHeight(_display.height() - NAVBAR_HEIGHT - 1);

    _scrollbar = new ScrollBar(ID_SCROLL, _display);
    layout->addWidget(_scrollbar);
    _scrollbar->setWidth(SCROLLBAR_WIDTH);
    _scrollbar->setHeight(_display.height() - NAVBAR_HEIGHT);
    _scrollbar->setPos(_display.width() - SCROLLBAR_WIDTH, 0);

    uint16_t pl_sz = _pl_manager.getPlaylistSize(_playlist_name.c_str());

    if (pl_sz > 0 && pos == pl_sz)
        --pos;

    _scrollbar->setMax(pl_sz);
    _scrollbar->setValue(pos);

    std::vector<MenuItem *> items = getTracksItems(_dynamic_menu->getItemsNumOnScreen(), pos);

    for (size_t i = 0; i < items.size(); ++i)
        _dynamic_menu->addItem(items[i]);
}

void Mp3Screen::showPlaying()
{
    const uint8_t DISPLAY_PADDING{10};

    WidgetCreator creator{_display};

    EmptyLayout *layout = creator.getEmptyLayout();
    setLayout(layout);

    NavBar *navbar = creator.getNavbar(ID_NAVBAR, STR_PAUSE, "", STR_EXIT);
    layout->addWidget(navbar);

    _track_name_lbl = new Label(ID_TRACK_NAME, _display);
    layout->addWidget(_track_name_lbl);
    _track_name_lbl->setText(_track_name);
    _track_name_lbl->setFontID(4);
    _track_name_lbl->setAlign(IWidget::ALIGN_CENTER);
    _track_name_lbl->setGravity(IWidget::GRAVITY_CENTER);
    _track_name_lbl->setBackColor(TFT_BLACK);
    _track_name_lbl->setTextColor(TFT_ORANGE);
    _track_name_lbl->setWidth(_display.width());
    _track_name_lbl->setHeight(20);
    _track_name_lbl->setTicker(true);

    _progress = new ProgressBar(ID_PROGRESS, _display);
    layout->addWidget(_progress);
    _progress->setBackColor(TFT_BLACK);
    _progress->setProgressColor(TFT_ORANGE);
    _progress->setBorderColor(TFT_WHITE);
    _progress->setMax(9999);
    _progress->setWidth(_display.width() - 20);
    _progress->setHeight(10);
    _progress->setProgress(1);
    _progress->setPos(DISPLAY_PADDING, _display.height() - NAVBAR_HEIGHT - _progress->getHeight() - 5);

    _track_pos_lbl = _track_name_lbl->clone(ID_TRACK_POS);
    layout->addWidget(_track_pos_lbl);
    _track_pos_lbl->setText(STR_POS_TIME);
    _track_pos_lbl->setAlign(IWidget::ALIGN_END);
    _track_pos_lbl->initWidthToFit();
    _track_pos_lbl->setBackColor(COLOR_MAIN_BACK);
    _track_pos_lbl->setPos(DISPLAY_PADDING, _progress->getYPos() - 25);
    _track_pos_lbl->setTicker(false);

    _track_time_lbl = _track_pos_lbl->clone(ID_TRACK_TIME);
    layout->addWidget(_track_time_lbl);
    _track_time_lbl->setText(STR_TRACK_TIME);
    _track_time_lbl->initWidthToFit();
    _track_time_lbl->setPos(_display.width() - _track_time_lbl->getWidth() - DISPLAY_PADDING, _track_pos_lbl->getYPos());

    _play_btn = new Image(ID_PLAY_BTN, _display);
    layout->addWidget(_play_btn);
    _play_btn->init(32, 32);
    _play_btn->setSrc(PAUSE_IMG);
    _play_btn->setBackColor(COLOR_MAIN_BACK);
    _play_btn->setTransparentColor(_play_btn->TRANSPARENT_COLOR);
    _play_btn->setPos((_display.width() - 32) / 2, (_display.height() - 32) / 2);

    _volume_lbl = new Label(ID_VOLUME_LBL, _display);
    layout->addWidget(_volume_lbl);
    _volume_lbl->setText(String(_volume));
    _volume_lbl->setAlign(IWidget::ALIGN_END);
    _volume_lbl->setGravity(IWidget::GRAVITY_CENTER);
    _volume_lbl->setHeight(15);
    _volume_lbl->setWidth(20);
    _volume_lbl->setBackColor(COLOR_MAIN_BACK);
    _volume_lbl->setTextColor(TFT_ORANGE);
    _volume_lbl->setPos((_display.width() - _volume_lbl->getWidth()) / 2 + 12, _display.height() - 60);

    Image *volume_img = new Image(ID_VOLUME_IMG, _display);
    layout->addWidget(volume_img);
    volume_img->init(16, 16);
    volume_img->setSrc(SPEAKER_IMG);
    volume_img->setBackColor(COLOR_MAIN_BACK);
    volume_img->setTransparentColor(volume_img->TRANSPARENT_COLOR);
    volume_img->setPos(_volume_lbl->getXPos() - 16 - 2, _volume_lbl->getYPos());

    Image *forward_img = new Image(ID_FORWARD_IMG, _display);
    layout->addWidget(forward_img);
    forward_img->init(24, 24);
    forward_img->setSrc(FORWARD_IMG);
    forward_img->setBackColor(COLOR_MAIN_BACK);
    forward_img->setTransparentColor(forward_img->TRANSPARENT_COLOR);
    forward_img->setPos(_play_btn->getXPos() + 32 * 2, _play_btn->getYPos() + 4);

    Image *rewind_img = new Image(ID_REWIND_IMG, _display);
    layout->addWidget(rewind_img);
    rewind_img->init(24, 24);
    rewind_img->setSrc(REWIND_IMG);
    rewind_img->setBackColor(COLOR_MAIN_BACK);
    rewind_img->setTransparentColor(rewind_img->TRANSPARENT_COLOR);
    rewind_img->setPos(_play_btn->getXPos() - 32 - 24, _play_btn->getYPos() + 4);

    Image *clock_img = new Image(ID_TIME_IMG, _display);
    layout->addWidget(clock_img);
    clock_img->init(16, 16);
    clock_img->setSrc(CLOCK_IMG);
    clock_img->setBackColor(COLOR_MAIN_BACK);
    clock_img->setTransparentColor(clock_img->TRANSPARENT_COLOR);
    clock_img->setPos(5, _track_name_lbl->getHeight() + 5);

    _time_lbl = new Label(ID_TIME_LBL, _display);
    layout->addWidget(_time_lbl);
    _time_lbl->setText(EMPTY_TIME);
    _time_lbl->setGravity(IWidget::GRAVITY_CENTER);
    _time_lbl->setHeight(18);
    _time_lbl->setWidth(50);
    _time_lbl->setBackColor(COLOR_MAIN_BACK);
    _time_lbl->setTextColor(TFT_ORANGE);
    _time_lbl->setPos(clock_img->getXPos() + clock_img->getWidth() + 2, clock_img->getYPos());

    _upd_time_time = 0;

    _temp_date_time.hour = 0;
    _temp_date_time.minute = 0;
    _temp_date_time.year = 0;

    updateTime();
}

void Mp3Screen::showUpdating()
{
    _mode = MODE_PLST_UPD;

    WidgetCreator creator{_display};

    EmptyLayout *layout = creator.getEmptyLayout();
    setLayout(layout);

    NavBar *navbar = creator.getNavbar(ID_NAVBAR, "", "", STR_CANCEL);
    layout->addWidget(navbar);

    _upd_lbl = new Label(ID_UPD_LBL, _display);
    layout->addWidget(_upd_lbl);
    _upd_lbl->setAlign(IWidget::ALIGN_CENTER);
    _upd_lbl->setGravity(IWidget::GRAVITY_CENTER);
    _upd_lbl->setBackColor(COLOR_MAIN_BACK);
    _upd_lbl->setTextColor(TFT_WHITE);
    _upd_lbl->setText(STR_UPDATING);
    _upd_lbl->setHeight(16);
    _upd_lbl->setWidth(_display.width());
    _upd_lbl->setPos(0, (float)(_display.height() - NAVBAR_HEIGHT - _upd_lbl->getHeight()) / 2);
}

void Mp3Screen::showPlMenu()
{
    if (_mode != MODE_TRACK_SEL)
        return;

    _mode = MODE_PLST_MENU;

    _dynamic_menu->disable();

    WidgetCreator creator{_display};
    _pl_menu = new FixedMenu(ID_PL_MENU, _display);
    getLayout()->addWidget(_pl_menu);
    _pl_menu->setBackColor(COLOR_MENU_ITEM);
    _pl_menu->setBorderColor(TFT_ORANGE);
    _pl_menu->setBorder(true);
    _pl_menu->setItemHeight(20);
    _pl_menu->setWidth(120);
    _pl_menu->setHeight(44);
    _pl_menu->setPos(_display.width() - _pl_menu->getWidth(), _display.height() - _pl_menu->getHeight() - NAVBAR_HEIGHT);

    MenuItem *upd_item = creator.getMenuItem(ID_ITEM_UPD);
    _pl_menu->addItem(upd_item);

    Label *upd_lbl = creator.getItemLabel(STR_UPDATE, 4, 2);
    upd_item->setLbl(upd_lbl);

    if (_dynamic_menu->getCurrentItemID() != 0)
    {
        MenuItem *del_item = creator.getMenuItem(ID_ITEM_DEL);
        _pl_menu->addItem(del_item);

        Label *upd_lbl = creator.getItemLabel(STR_DELETE, 4, 2);
        del_item->setLbl(upd_lbl);
    }
}

void Mp3Screen::hidePlMenu()
{
    _mode = MODE_TRACK_SEL;

    getLayout()->deleteWidgetByID(ID_PL_MENU);

    _dynamic_menu->enable();
}

void Mp3Screen::updateTime()
{
    if (!_watch_inited || _is_locked)
        return;

    DS3231DateTime date_time = _watch.getDateTime();

    if (date_time == _temp_date_time)
        return;

    _temp_date_time = date_time;

    String temp_str;

    // Час
    if (date_time.hour < 10)
    {
        temp_str = "0";
        temp_str += String(date_time.hour);
        temp_str += ":";
    }
    else
    {
        temp_str = String(date_time.hour);
        temp_str += ":";
    }

    if (date_time.minute < 10)
    {
        temp_str += "0";
        temp_str += String(date_time.minute);
    }
    else
    {
        temp_str += String(date_time.minute);
    }

    _time_lbl->setText(temp_str);
    _time_lbl->updateWidthToFit();
}

bool Mp3Screen::playTrack(bool contn)
{
    if ((_playlist_name.isEmpty() || _track_name.isEmpty()))
        return false;

    if (!contn)
        _track_time = 0;

    String track_path = _pl_manager.getTrackPath(_playlist_name.c_str(), _track_name.c_str());
    if (track_path.isEmpty())
        return false;

    if (!_audio.connecttoFS(SD, track_path.c_str(), _track_time))
    {
        setStopState();
        return false;
    }

    showPlaying();
    _mode = MODE_AUDIO_PLAY;
    _is_new_track = true;
    _is_playing = true;
    return true;
}

bool Mp3Screen::playNext()
{
    _track_name = _pl_manager.getNextTrackName(_playlist_name.c_str(), _track_pos);
    if (_track_name.isEmpty())
        return false;

    if (!playTrack(false))
        return false;

    ++_track_pos;
    return true;
}

bool Mp3Screen::playPrev()
{
    _track_name = _pl_manager.getPrevTrackName(_playlist_name.c_str(), _track_pos);
    if (_track_name.isEmpty())
        return false;

    if (!playTrack(false))
        return false;

    --_track_pos;
    return true;
}

void Mp3Screen::volumeUp()
{
    if (_volume < _audio.maxVolume() - 2)
    {
        if (_volume < 10)
            _volume += 1;
        else
            _volume += 2;

        _audio.setVolume(_volume);
    }

    _volume_lbl->setText(String(_volume));
}

void Mp3Screen::volumeDown()
{
    if (_volume > 1)
    {
        if (_volume > 10)
            _volume -= 2;
        else
            _volume -= 1;

        _audio.setVolume(_volume);
    }

    _volume_lbl->setText(String(_volume));
}

void Mp3Screen::setStopState()
{
    _audio.stopSong();

    _is_playing = false;
    _track_pos = 0;
    _track_name = "";

    _play_btn->setSrc(PLAY_IMG);
    _track_name_lbl->setText(STR_STOPPED);
    _track_pos_lbl->setText(STR_POS_TIME);
    _track_time_lbl->setText(STR_TRACK_TIME);
    _progress->reset();

    if (_is_locked)
        setCpuFrequencyMhz(10);
}

bool Mp3Screen::updateTrackDuration()
{
    uint32_t duration = _audio.getAudioFileDuration();

    if (duration == 0)
        return false;

    uint32_t minutes = floor((float)duration / 60);
    uint32_t sec = duration - minutes * 60;

    String track_time{""};

    if (minutes < 100)
        track_time += "0";

    if (minutes < 10)
        track_time += "0";

    track_time += minutes;

    track_time += ":";

    sec = duration - minutes * 60;

    if (sec < 10)
        track_time += "0";

    track_time += sec;

    _track_time_lbl->setText(track_time);

    _progress->setProgress(0);
    _progress->setMax(duration);

    return true;
}

void Mp3Screen::updateTrackPos()
{
    _track_time = _audio.getAudioCurrentTime();

    uint32_t minutes = floor((float)_track_time / 60);
    uint32_t sec = _track_time - minutes * 60;

    String track_pos;

    if (minutes < 100)
        track_pos += "0";

    if (minutes < 10)
        track_pos += "0";

    track_pos += String(minutes);

    track_pos += ":";

    sec = _track_time - minutes * 60;

    if (sec < 10)
        track_pos += "0";

    track_pos += String(sec);

    _track_pos_lbl->setText(track_pos);

    _progress->setProgress(_track_time);
}

void Mp3Screen::up()
{
    if (_mode == MODE_PLST_SEL)
    {
        _fixed_menu->focusUp();
        _scrollbar->scrollUp();
    }
    else if (_mode == MODE_TRACK_SEL)
    {
        _dynamic_menu->focusUp();
        _scrollbar->scrollUp();
    }
    else if (_mode == MODE_AUDIO_PLAY)
    {
        volumeUp();
    }
    else if (_mode == MODE_PLST_MENU)
    {
        _pl_menu->focusUp();
    }
}

void Mp3Screen::changeBackLight()
{
    if (_mode != MODE_AUDIO_PLAY)
        return;

    if (_is_locked)
    {
        setCpuFrequencyMhz(240);

        DisplayUtil display;
        display.setBrightness(_brightness);
        _screen_enabled = true;

        _input.enablePin(KeyID::KEY_BACK);
        _input.enablePin(KeyID::KEY_LEFT);
        _input.enablePin(KeyID::KEY_RIGHT);
    }
    else
    {
        DisplayUtil display;
        display.setBrightness(0);
        _screen_enabled = false;

        _input.disablePin(KeyID::KEY_BACK);
        _input.disablePin(KeyID::KEY_LEFT);
        _input.disablePin(KeyID::KEY_RIGHT);

        setCpuFrequencyMhz(160);
    }

    _is_locked = !_is_locked;
}

void Mp3Screen::down()
{
    if (_mode == MODE_PLST_SEL)
    {
        _fixed_menu->focusDown();
        _scrollbar->scrollDown();
    }
    else if (_mode == MODE_TRACK_SEL)
    {
        _dynamic_menu->focusDown();
        _scrollbar->scrollDown();
    }
    else if (_mode == MODE_AUDIO_PLAY)
    {
        volumeDown();
    }
    else if (_mode == MODE_PLST_MENU)
    {
        _pl_menu->focusDown();
    }
}

void Mp3Screen::left()
{
    if (_mode == MODE_AUDIO_PLAY)
        playPrev();
}

void Mp3Screen::right()
{
    if (_mode == MODE_AUDIO_PLAY)
        playNext();
}

void Mp3Screen::leftPressed()
{
    _audio.setTimeOffset(-20);
}

void Mp3Screen::rightPressed()
{
    _audio.setTimeOffset(20);
}

void Mp3Screen::ok()
{
    if (_mode == MODE_PLST_SEL)
    {
        uint16_t item_ID = _fixed_menu->getCurrentItemID();
        if (item_ID == 1)
        {
            playTrack(true);
        }
        else if (item_ID == 2)
        {
            if (_pl_manager.updatePlaylists())
            {
                _is_tracklist_upd = false;
                showUpdating();
            }
        }
        else
        {
            _playlist_name = _fixed_menu->getCurrentItemText();
            _track_pos = 0;
            showTracks(_track_pos);
        }
    }
    else if (_mode == MODE_TRACK_SEL)
    {
        if (_dynamic_menu->getSize() > 0)
        {
            _track_name = _dynamic_menu->getCurrentItemText();
            _track_pos = _dynamic_menu->getCurrentItemID() - 1;
            playTrack(false);
        }
    }
    else if (_mode == MODE_AUDIO_PLAY)
    {
        if (_track_name != "")
        {
            _audio.pauseResume();

            if (_audio.isRunning())
            {
                _play_btn->setSrc(PAUSE_IMG);
                _is_playing = true;
            }
            else
            {
                _play_btn->setSrc(PLAY_IMG);
                _is_playing = false;
            }
        }
    }
    else if (_mode == MODE_PLST_MENU)
    {
        uint16_t id = _pl_menu->getCurrentItemID();

        if (id == ID_ITEM_UPD)
        {
            if (_pl_manager.updateTracklists(_playlist_name.c_str()))
            {
                _is_tracklist_upd = true;
                showUpdating();
            }
        }
        else if (id == ID_ITEM_DEL)
        {
            String track_name = _dynamic_menu->getCurrentItemText();
            if (track_name.isEmpty())
                return;

            _track_pos = _dynamic_menu->getCurrentItemID();

            --_track_pos;

            if (_pl_manager.removeTrack(_playlist_name.c_str(), track_name.c_str()))
            {
                if (_pl_manager.updateTracklists(_playlist_name.c_str()))
                {
                    _is_tracklist_upd = true;
                    showUpdating();
                }
                else
                    showTracks(_track_pos);
            }
        }
    }
}

void Mp3Screen::back()
{
    if (_mode == MODE_PLST_SEL)
    {
        openScreenByID(ID_SCREEN_MENU);
    }
    else if (_mode == MODE_TRACK_SEL)
    {
        showPlaylists();
    }
    else if (_mode == MODE_PLST_MENU)
    {
        hidePlMenu();
    }
}

void Mp3Screen::backPressed()
{
    if (_mode == MODE_AUDIO_PLAY)
    {
        _audio.stopSong();
        savePref();
        showTracks(_track_pos);
    }
}
