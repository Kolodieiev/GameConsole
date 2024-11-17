#pragma once
#pragma GCC optimize("O3")

#include <Arduino.h>

#include "meow/lib/DS3231/DS3231.h"

#include "meow/ui/screen/IScreen.h"
#include "meow/manager/settings/SettingsManager.h"
#include "meow/ui/widget/scrollbar/ScrollBar.h"
#include "meow/ui/widget/navbar/NavBar.h"
#include "meow/ui/widget/menu/FixedMenu.h"
#include "meow/ui/widget/menu/DynamicMenu.h"

#include "./BooklistManager.h"

using namespace meow;

class ReaderScreen : public IScreen, public IItemsLoader
{

public:
    ReaderScreen(GraphicsDriver &display);
    virtual ~ReaderScreen() {}

protected:
    virtual void loop() override;
    virtual void update() override;

    virtual std::vector<MenuItem *> loadPrev(uint8_t size, uint16_t current_ID) override;
    virtual std::vector<MenuItem *> loadNext(uint8_t size, uint16_t current_ID) override;

private:
    enum Widget_ID : uint8_t
    {
        ID_NAVBAR = 1,
        ID_F_MENU,
        ID_D_MENU,
        ID_SCROLL,
        ID_BOOK_MENU,
        ID_PROGRESS,
        ID_TIME_LBL,
        ID_UPD_LBL,
        ID_PAGE_LBL,
        ID_PROGRESS_LBL,
        ID_MSG_LBL
    };

    enum Mode : uint8_t
    {
        MODE_BOOK_DIR_SEL = 0,
        MODE_UPDATING,
        MODE_BOOK_SEL,
        MODE_BOOK_READ,
        MODE_BOOK_MENU,
        MODE_SD_UNCONN
    };

    enum BlMenuItemsID : uint8_t
    {
        ID_ITEM_DEL = 1,
        ID_ITEM_UPD,
    };

    Mode _mode{MODE_BOOK_DIR_SEL};
    unsigned long _upd_msg_time{0};
    //
    BooklistManager _bl_manager;
    SettingsManager _settings;

    const uint8_t BOOK_DIR_ITEMS_NUM{6};
    const uint8_t BOOKS_ITEMS_NUM{10};
    const uint16_t LAT_NUM_CHARS_TO_READ{550};
    const uint16_t KIR_NUM_CHARS_TO_READ{950};
    uint16_t _num_char_to_read{0};
    //
    NavBar *_book_navbar;
    Label *_time_lbl;
    Label *_progress_lbl;
    //
    uint8_t _old_brightness;
    uint8_t _brightness;
    bool _brightness_edit_en{true};
    //
    Label *_page;
    FixedMenu *_fixed_menu;
    ScrollBar *_scrollbar;
    FixedMenu *_pl_menu;
    DynamicMenu *_dynamic_menu;
    //
    String _book_dir_name;
    String _book_name;
    uint16_t _book_pos{0};
    size_t _book_size{0};
    size_t _read_pos{0};
    size_t _bytes_read{0};

    //
    DS3231 _watch;
    bool _watch_inited{false};
    unsigned long _upd_time_time{0};
    DS3231DateTime _temp_date_time;
    //
    Label *_msg_lbl;
    uint8_t _upd_counter{0};
    //
    void savePref();
    //
    void ok();
    void up();
    void down();
    void left();
    void right();

    void back();
    void backPressed();

    void showBookMenu();
    void hideBookMenu();

    void showUpdating();
    void showBookDirs();
    void fillBookDirs(Menu *menu_ptr, uint16_t from_id);
    void showBooks(uint16_t pos = 0);
    std::vector<MenuItem *> getBooksItems(uint8_t size, uint16_t from_id);
    void openBook(bool contn = false);
    void loadNextTxt();
    void loadPrevTxt();

    void showRead();

    void updateTime();
    void updateReadProgress();

    void showSDErrTmpl();

    void updateBookPos();
};