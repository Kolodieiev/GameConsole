#pragma GCC optimize("O3")

#include "ReaderScreen.h"

#include "../WidgetCreator.h"

#include "../resources/string.h"
#include "../resources/color.h"
#include "../resources/const.h"

#include "meow/util/display/DisplayUtil.h"

const char STR_BOOK_DIR_PREF[] = "book_dir";
const char STR_BOOK_NAME_PREF[] = "book_name";
const char STR_READ_POS_PREF[] = "read_pos";
const char STR_BOOK_BRIGHT_PREF[] = "book_bright";

void ReaderScreen::loop()
{
}

std::vector<MenuItem *> ReaderScreen::loadPrev(uint8_t size, uint16_t current_ID)
{
    // Не отримали ID
    if (current_ID == 0)
        return std::vector<MenuItem *>();

    uint16_t item_pos = current_ID - 1;

    // Вже перший елемент
    if (item_pos == 0)
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

    return getBooksItems(size, item_pos);
}

std::vector<MenuItem *> ReaderScreen::loadNext(uint8_t size, uint16_t current_ID)
{
    if (current_ID == 0)
        return std::vector<MenuItem *>();

    return getBooksItems(size, current_ID);
}

void ReaderScreen::savePref()
{
    _settings.set(STR_BOOK_DIR_PREF, _book_dir_name.c_str());
    _settings.set(STR_BOOK_NAME_PREF, _book_name.c_str());
    _settings.set(STR_READ_POS_PREF, String(_read_pos).c_str());
    _settings.set(STR_BOOK_BRIGHT_PREF, String(_brightness).c_str());
}

//-------------------------------------------------------------------------------------------

void ReaderScreen::showBookMenu()
{
    _dynamic_menu->disable();

    WidgetCreator creator{_display};

    _pl_menu = new FixedMenu(ID_BOOK_MENU, _display);
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

    _mode = MODE_BOOK_MENU;
}

void ReaderScreen::showUpdating()
{
    WidgetCreator creator{_display};
    IWidgetContainer *layout = creator.getEmptyLayout();

    layout->addWidget(creator.getNavbar(ID_NAVBAR, "", "", STR_CANCEL));

    _msg_lbl = creator.getStatusMsgLable(ID_MSG_LBL, STR_UPDATING, 2);
    layout->addWidget(_msg_lbl);

    _mode = MODE_UPDATING;
    setLayout(layout);
}

void ReaderScreen::hideBookMenu()
{
    getLayout()->deleteWidgetByID(ID_BOOK_MENU);
    _dynamic_menu->enable();
    _mode = MODE_BOOK_SEL;
}

void ReaderScreen::showBookDirs()
{
    WidgetCreator creator{_display};
    IWidgetContainer *layout = creator.getEmptyLayout();

    layout->addWidget(creator.getNavbar(ID_NAVBAR, STR_SELECT, "", STR_BACK));

    _fixed_menu = new FixedMenu(ID_F_MENU, _display);
    layout->addWidget(_fixed_menu);
    _fixed_menu->setBackColor(COLOR_MENU_ITEM);
    _fixed_menu->setWidth(_display.width() - SCROLLBAR_WIDTH);
    _fixed_menu->setHeight(_display.height() - NAVBAR_HEIGHT - 2);
    _fixed_menu->setItemHeight((_display.height() - NAVBAR_HEIGHT - 1) / BOOK_DIR_ITEMS_NUM);

    _scrollbar = new ScrollBar(ID_SCROLL, _display);
    layout->addWidget(_scrollbar);
    _scrollbar->setWidth(SCROLLBAR_WIDTH);
    _scrollbar->setHeight(_display.height() - NAVBAR_HEIGHT);
    _scrollbar->setPos(_display.width() - SCROLLBAR_WIDTH, 0);

    if (!_book_name.isEmpty())
    {
        MenuItem *cont_item = creator.getMenuItem(1);
        _fixed_menu->addItem(cont_item);

        Label *cont_lbl = creator.getItemLabel(STR_CONTINUE, 4, 2);
        cont_item->setLbl(cont_lbl);
    }

    fillBookDirs(_fixed_menu, 3);

    MenuItem *upd_item = creator.getMenuItem(2);
    _fixed_menu->addItem(upd_item);

    Label *upd_lbl = creator.getItemLabel(STR_UPD_LISTS, 4, 2);
    upd_item->setLbl(upd_lbl);

    _scrollbar->setMax(_fixed_menu->getSize());

    _mode = MODE_BOOK_DIR_SEL;
    setLayout(layout);
}

void ReaderScreen::showBooks(uint16_t pos)
{
    WidgetCreator creator{_display};
    IWidgetContainer *layout = creator.getEmptyLayout();

    layout->addWidget(creator.getNavbar(ID_NAVBAR, STR_SELECT, "", STR_BACK));

    _dynamic_menu = creator.getDynamicMenu(ID_D_MENU, this);
    layout->addWidget(_dynamic_menu);
    _dynamic_menu->setItemHeight((_display.height() - NAVBAR_HEIGHT - 2) / BOOKS_ITEMS_NUM);
    _dynamic_menu->setWidth(_display.width() - SCROLLBAR_WIDTH);
    _dynamic_menu->setHeight(_display.height() - NAVBAR_HEIGHT - 1);

    _scrollbar = new ScrollBar(ID_SCROLL, _display);
    layout->addWidget(_scrollbar);
    _scrollbar->setWidth(SCROLLBAR_WIDTH);
    _scrollbar->setHeight(_display.height() - NAVBAR_HEIGHT);
    _scrollbar->setPos(_display.width() - SCROLLBAR_WIDTH, 0);

    uint16_t pl_sz = _bl_manager.getDirSize(_book_dir_name.c_str());

    if (pl_sz > 0 && pos == pl_sz)
        --pos;

    _scrollbar->setMax(pl_sz);
    _scrollbar->setValue(pos);

    std::vector<MenuItem *> items = getBooksItems(_dynamic_menu->getItemsNumOnScreen(), pos);

    for (size_t i = 0; i < items.size(); ++i)
        _dynamic_menu->addItem(items[i]);

    _mode = MODE_BOOK_SEL;
    setLayout(layout);
}

void ReaderScreen::showRead()
{
    DisplayUtil display;
    display.setBrightness(_brightness);

    WidgetCreator creator{_display};
    IWidgetContainer *layout = creator.getEmptyLayout();

    layout->setBackColor(TFT_BLACK);

    _page = new Label(ID_PAGE_LBL, _display);
    layout->addWidget(_page);
    _page->setMultiline(true);
    _page->setWidth(DWIDTH);
    _page->setHeight(DHEIGHT - NAVBAR_HEIGHT);
    _page->setBackColor(TFT_BLACK);
    _page->setTextColor(TFT_WHITE);

    _time_lbl = new Label(ID_UPD_LBL, _display);
    _time_lbl->setText(EMPTY_TIME);
    _time_lbl->setAlign(IWidget::ALIGN_CENTER);
    _time_lbl->setGravity(IWidget::GRAVITY_CENTER);
    _time_lbl->setTextColor(TFT_WHITE);
    _time_lbl->initWidthToFit();

    _progress_lbl = _time_lbl->clone(ID_PROGRESS_LBL);
    _progress_lbl->setText(EMPTY_READ_PROGRESS);
    _progress_lbl->initWidthToFit();

    _book_navbar = creator.getNavbar(ID_NAVBAR, "", "", "");
    layout->addWidget(_book_navbar);
    _book_navbar->setWidgets(_time_lbl, new Label(1, _display), _progress_lbl);

    updateTime();
    updateReadProgress();

    _mode = MODE_BOOK_READ;
    
    setLayout(layout);
}

void ReaderScreen::showSDErrTmpl()
{
    WidgetCreator creator{_display};
    IWidgetContainer *layout = creator.getEmptyLayout();

    layout->addWidget(creator.getNavbar(ID_NAVBAR, "", "", STR_EXIT));

    _msg_lbl = creator.getStatusMsgLable(ID_MSG_LBL, STR_SD_ERR);
    layout->addWidget(_msg_lbl);

    _mode = MODE_SD_UNCONN;
    setLayout(layout);
}

//-------------------------------------------------------------------------------------------

ReaderScreen::ReaderScreen(GraphicsDriver &display) : IScreen(display)
{
    WidgetCreator creator{_display};
    EmptyLayout *layout = creator.getEmptyLayout();
    setLayout(layout);

    if (!_bl_manager.hasConnection())
    {
        showSDErrTmpl();
        return;
    }

    _watch_inited = _watch.begin();

    if (!_watch_inited)
        log_e("Помилка ініціалізації RTC");

    _old_brightness = atoi(_settings.get(STR_PREF_BRIGHT).c_str());

    if (_old_brightness == 0)
        _old_brightness = 100;

    String book_br = _settings.get(STR_BOOK_BRIGHT_PREF);

    if (book_br.isEmpty())
        _brightness = _old_brightness;
    else
        _brightness = atoi(book_br.c_str());

    _book_dir_name = _settings.get(STR_BOOK_DIR_PREF);
    _book_name = _settings.get(STR_BOOK_NAME_PREF);
    _read_pos = atoi(_settings.get(STR_READ_POS_PREF).c_str());

    showBookDirs();
}

void ReaderScreen::update()
{
    if (_mode == MODE_SD_UNCONN)
    {
        if (_input.isReleased(KeyID::KEY_BACK))
        {
            _input.lock(KeyID::KEY_BACK, 500);
            setCpuFrequencyMhz(240);
            openScreenByID(ID_SCREEN_MENU);
        }

        return;
    }

    if (_input.isPressed(KeyID::KEY_OK))
    {
        _input.lock(KeyID::KEY_OK, 1000);
        if (_mode == MODE_BOOK_SEL)
            showBookMenu();
    }
    else if (_input.isPressed(KeyID::KEY_BACK))
    {
        _input.lock(KeyID::KEY_BACK, 2500);
        backPressed();
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

    if (_mode == MODE_BOOK_READ)
    {
        if (millis() - _upd_time_time > 10000)
        {
            updateTime();
            _upd_time_time = millis();
        }
    }
    else if (_mode == MODE_UPDATING)
    {
        if (_bl_manager.isWorking() && (millis() - _upd_msg_time) > 1000)
        {
            String upd_txt = STR_UPDATING;
            if (_upd_counter > 3)
                _upd_counter = 0;
            else
                ++_upd_counter;

            for (uint8_t i{0}; i < _upd_counter; ++i)
                upd_txt += ".";

            _msg_lbl->setText(upd_txt);
            _upd_msg_time = millis();
        }
    }
}

void ReaderScreen::ok()
{
    if (_mode == MODE_BOOK_DIR_SEL)
    {
        uint16_t item_ID = _fixed_menu->getCurrentItemID();
        if (item_ID == 1)
        {
            openBook(true);
        }
        else if (item_ID == 2)
        {
            if (_bl_manager.updateBookDirs([this]
                                           { showBookDirs(); }))
            {
                showUpdating();
            }
        }
        else
        {
            _book_dir_name = _fixed_menu->getCurrentItemText();
            _book_pos = 0;
            showBooks(_book_pos);
        }
    }
    else if (_mode == MODE_BOOK_SEL)
    {
        openBook(false);
    }
    else if (_mode == MODE_BOOK_READ)
    {
        _brightness_edit_en = !_brightness_edit_en;

        if (_brightness_edit_en)
        {
            _input.enablePin(KeyID::KEY_UP);
            _input.enablePin(KeyID::KEY_DOWN);
            _input.enablePin(KeyID::KEY_BACK);
            _book_navbar->setVisibility(IWidget::VISIBLE);
        }
        else
        {
            _input.disablePin(KeyID::KEY_UP);
            _input.disablePin(KeyID::KEY_DOWN);
            _input.disablePin(KeyID::KEY_BACK);
            _book_navbar->setVisibility(IWidget::INVISIBLE);
        }
    }
    else if (_mode == MODE_BOOK_MENU)
    {
        uint16_t id = _pl_menu->getCurrentItemID();

        if (id == ID_ITEM_UPD)
        {
            if (_bl_manager.updateBooklist(_book_dir_name.c_str(),
                                           [this]
                                           {
                                               updateBookPos();
                                               showBooks(_book_pos);
                                           }))
            {
                showUpdating();
            }
        }
        else if (id == ID_ITEM_DEL)
        {
            String book_name = _dynamic_menu->getCurrentItemText();
            if (book_name.isEmpty())
                return;

            _book_pos = _dynamic_menu->getCurrentItemID() - 1;

            if (_bl_manager.removeBook(_book_dir_name.c_str(), book_name.c_str(), [this]
                                       {
                                           if (!_bl_manager.updateBooklist(_book_dir_name.c_str(), [this] {
                                               updateBookPos();
                                               showBooks(_book_pos);
                                           }))
                                           {
                                                showBooks(_book_pos);
                                           } }))
            {
                showUpdating();
            }
        }
    }
}

void ReaderScreen::updateBookPos()
{
    if (_book_pos > 0)
    {
        uint16_t list_size = _bl_manager.getDirSize(_book_dir_name.c_str());

        if (_book_pos >= list_size)
            _book_pos = list_size;
    }
}

//-------------------------------------------------------------------------------------------

void ReaderScreen::up()
{
    if (_mode == MODE_BOOK_DIR_SEL)
    {
        _fixed_menu->focusUp();
        _scrollbar->scrollUp();
    }
    else if (_mode == MODE_BOOK_SEL)
    {
        _dynamic_menu->focusUp();
        _scrollbar->scrollUp();
    }
    else if (_mode == MODE_BOOK_READ)
    {
        if (_brightness_edit_en)
        {
            if (_brightness + 5 < 255)
            {
                _brightness += 5;

                DisplayUtil display;
                display.setBrightness(_brightness);
            }
        }
    }
    else if (_mode == MODE_BOOK_MENU)
    {
        _pl_menu->focusUp();
    }
}

void ReaderScreen::down()
{
    if (_mode == MODE_BOOK_DIR_SEL)
    {
        _fixed_menu->focusDown();
        _scrollbar->scrollDown();
    }
    else if (_mode == MODE_BOOK_SEL)
    {
        _dynamic_menu->focusDown();
        _scrollbar->scrollDown();
    }
    else if (_mode == MODE_BOOK_READ)
    {
        if (_brightness_edit_en)
        {
            if (_brightness > 10)
            {
                _brightness -= 5;

                DisplayUtil display;
                display.setBrightness(_brightness);
            }
        }
    }
    else if (_mode == MODE_BOOK_MENU)
    {
        _pl_menu->focusDown();
    }
}

void ReaderScreen::left()
{
    if (_mode == MODE_BOOK_READ)
        loadPrevTxt();
}

void ReaderScreen::right()
{
    if (_mode == MODE_BOOK_READ)
        loadNextTxt();
}

void ReaderScreen::back()
{
    if (_mode == MODE_BOOK_DIR_SEL)
    {
        openScreenByID(ID_SCREEN_MENU);
    }
    else if (_mode == MODE_BOOK_SEL)
    {
        showBookDirs();
    }
    else if (_mode == MODE_BOOK_MENU)
    {
        hideBookMenu();
    }
}

void ReaderScreen::backPressed()
{
    if (_mode == MODE_BOOK_READ)
    {
        setCpuFrequencyMhz(240);

        if (!_brightness_edit_en)
        {
            _input.enablePin(KeyID::KEY_UP);
            _input.enablePin(KeyID::KEY_DOWN);
            _brightness_edit_en = false;
        }

        DisplayUtil display;
        display.setBrightness(_old_brightness);
        _upd_time_time = 0;
        _temp_date_time.year = 0;
        savePref();
        showBooks(_book_pos);
    }
}

void ReaderScreen::fillBookDirs(Menu *menu_ptr, uint16_t from_id)
{
    std::vector<String> menu_list = _bl_manager.getBookDirs();

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

void ReaderScreen::openBook(bool contn)
{
    if (contn)
    {
        if ((_book_dir_name.isEmpty() || _book_name.isEmpty()))
            return;
    }
    else
    {
        if (_dynamic_menu->getSize() == 0)
            return;

        _book_name = _dynamic_menu->getCurrentItemText();
        _book_pos = _dynamic_menu->getCurrentItemID() - 1;
        _read_pos = 0;
        _bytes_read = 0;
    }

    _book_size = _bl_manager.getBookSize(_book_dir_name.c_str(), _book_name.c_str());

    if (_bl_manager.containCyrillic(_book_dir_name.c_str(), _book_name.c_str()))
        _num_char_to_read = KIR_NUM_CHARS_TO_READ;
    else
        _num_char_to_read = LAT_NUM_CHARS_TO_READ;

    if (_book_size > 0)
    {
        showRead();
        loadNextTxt();
    }
}

void ReaderScreen::loadNextTxt()
{
    setCpuFrequencyMhz(240);

    _read_pos += _bytes_read;

    bool is_eof;
    String txt = _bl_manager.readText(is_eof, _book_dir_name.c_str(), _book_name.c_str(), _read_pos, _num_char_to_read);

    if (!txt.isEmpty())
    {
        if (!is_eof)
            _bytes_read = txt.length();
        else
            _bytes_read = 0;

        _page->setText(txt);
        updateReadProgress();
    }

    setCpuFrequencyMhz(80);
}

void ReaderScreen::loadPrevTxt()
{
    if (_read_pos == 0)
        return;

    setCpuFrequencyMhz(240);

    if (_read_pos > _num_char_to_read)
        _read_pos -= _num_char_to_read;
    else
        _read_pos = 0;

    bool is_eof;
    String txt = _bl_manager.readText(is_eof, _book_dir_name.c_str(), _book_name.c_str(), _read_pos, _num_char_to_read);

    if (!txt.isEmpty())
    {
        if (!is_eof)
            _bytes_read = txt.length();
        else
            _bytes_read = 0;

        _page->setText(txt);
        updateReadProgress();
    }

    setCpuFrequencyMhz(80);
}

void ReaderScreen::updateTime()
{
    if (!_watch_inited)
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

void ReaderScreen::updateReadProgress()
{
    String prog_txt;
    prog_txt += _read_pos / _num_char_to_read;
    prog_txt += "/";
    prog_txt += _book_size / _num_char_to_read;

    _progress_lbl->setText(prog_txt);
    _progress_lbl->updateWidthToFit();
}

std::vector<MenuItem *> ReaderScreen::getBooksItems(uint8_t size, uint16_t from_id)
{
    std::vector<String> books = _bl_manager.getBooks(_book_dir_name.c_str(), from_id, size);

    WidgetCreator creator{_display};

    std::vector<MenuItem *> ret;
    ret.reserve(books.size());

    for (size_t i = 0; i < books.size(); ++i)
    {
        ++from_id;

        MenuItem *item = creator.getMenuItem(from_id);
        ret.push_back(item);

        Label *item_lbl = creator.getItemLabel(books[i].c_str(), 2, 1);
        item->setLbl(item_lbl);
    }

    return ret;
}
