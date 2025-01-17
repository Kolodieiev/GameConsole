#pragma GCC optimize("O3")
#include "ReaderContext.h"

#include "../WidgetCreator.h"

#include "meow/util/display/DisplayUtil.h"

#include "../resources/string.h"
#include "../resources/color.h"
#include "../resources/const.h"

const char STR_BOOK_DIR_PREF[] = "book_dir";
const char STR_BOOK_NAME_PREF[] = "book_name";
const char STR_READ_POS_PREF[] = "read_pos";
const char STR_BOOK_BRIGHT_PREF[] = "book_bright";

const char ROOT_PATH[] = "/books";
const char BOOK_EXT[] = ".txt";

void ReaderContext::loop()
{
}

void ReaderContext::savePref()
{
    _settings.set(STR_BOOK_DIR_PREF, _dir_name.c_str());
    _settings.set(STR_BOOK_NAME_PREF, _book_name.c_str());
    _settings.set(STR_READ_POS_PREF, String(_read_pos).c_str());
}

//-------------------------------------------------------------------------------------------

void ReaderContext::showContextMenuTmpl()
{
    IWidgetContainer *layout = getLayout();
    layout->disable();

    _books_list_menu->disable();

    WidgetCreator creator{_display};

    _context_menu = new FixedMenu(ID_BOOK_MENU, _display);
    layout->addWidget(_context_menu);
    _context_menu->setBackColor(COLOR_MENU_ITEM);
    _context_menu->setBorderColor(TFT_ORANGE);
    _context_menu->setBorder(true);
    _context_menu->setItemHeight(20);
    _context_menu->setWidth(120);
    _context_menu->setHeight(44);
    _context_menu->setPos(D_WIDTH - _context_menu->getWidth(), D_HEIGHT - _context_menu->getHeight());

    if (_books_list_menu->getCurrentItemID() != 0)
    {
        MenuItem *del_item = creator.getMenuItem(ID_ITEM_DEL);
        _context_menu->addItem(del_item);

        Label *upd_lbl = creator.getItemLabel(STR_DELETE, 4, 2);
        del_item->setLbl(upd_lbl);
    }

    _mode = MODE_CONTEXT_MENU;
    layout->enable();
}

void ReaderContext::hideContextMenu()
{
    getLayout()->deleteWidgetByID(ID_BOOK_MENU);
    _books_list_menu->enable();
    _mode = MODE_BOOK_SEL;
}

void ReaderContext::showBookDirsTmpl()
{
    WidgetCreator creator{_display};
    IWidgetContainer *layout = creator.getEmptyLayout();

    _book_dirs_menu = new FixedMenu(ID_F_MENU, _display);
    layout->addWidget(_book_dirs_menu);
    _book_dirs_menu->setBackColor(COLOR_MENU_ITEM);
    _book_dirs_menu->setWidth(D_WIDTH - SCROLLBAR_WIDTH);
    _book_dirs_menu->setHeight(D_HEIGHT);
    _book_dirs_menu->setItemHeight((_book_dirs_menu->getHeight()) / BOOK_DIR_ITEMS_NUM);

    _scrollbar = new ScrollBar(ID_SCROLL, _display);
    layout->addWidget(_scrollbar);
    _scrollbar->setWidth(SCROLLBAR_WIDTH);
    _scrollbar->setHeight(D_HEIGHT);
    _scrollbar->setPos(D_WIDTH - SCROLLBAR_WIDTH, 0);

    if (!_book_name.isEmpty())
    {
        MenuItem *cont_item = creator.getMenuItem(ID_CONT_ITEM);
        _book_dirs_menu->addItem(cont_item);

        Label *cont_lbl = creator.getItemLabel(STR_CONTINUE, 4, 2);
        cont_item->setLbl(cont_lbl);
    }

    _mode = MODE_BOOK_DIR_SEL;

    setLayout(layout);
}

void ReaderContext::fillBookDirs()
{
    std::vector<MenuItem *> items;
    makeBookDirsItems(items);

    uint16_t size = items.size();

    for (size_t i = 0; i < size; ++i)
        _book_dirs_menu->addItem(items[i]);

    _scrollbar->setValue(0);
    _scrollbar->setMax(_book_dirs_menu->getSize());
}

void ReaderContext::makeBookDirsItems(std::vector<MenuItem *> &items)
{
    WidgetCreator creator{_display};
    items.clear();

    uint16_t books_num = _dirs.size();
    items.reserve(books_num);

    for (uint16_t i = 0, counter = ID_CONT_ITEM; i < books_num; ++i)
    {
        ++counter;
        MenuItem *item = creator.getMenuItem(counter);
        items.push_back(item);

        Label *lbl = new Label(1, _display);
        item->setLbl(lbl);
        lbl->setTickerInFocus(true);

        lbl->setText(_dirs[i].getName());
    }
}

void ReaderContext::showBooksListTmpl()
{
    WidgetCreator creator{_display};
    IWidgetContainer *layout = creator.getEmptyLayout();

    _books_list_menu = creator.getDynamicMenu(ID_D_MENU);
    layout->addWidget(_books_list_menu);
    _books_list_menu->setWidth(D_WIDTH - SCROLLBAR_WIDTH);
    _books_list_menu->setHeight(D_HEIGHT);
    _books_list_menu->setItemHeight((_books_list_menu->getHeight() - 2) / BOOKS_ITEMS_NUM);

    _books_list_menu->setOnNextItemsLoadHandler(onNextItemsLoad, this);
    _books_list_menu->setOnPrevItemsLoadHandler(onPrevItemsLoad, this);

    _scrollbar = new ScrollBar(ID_SCROLL, _display);
    layout->addWidget(_scrollbar);
    _scrollbar->setWidth(SCROLLBAR_WIDTH);
    _scrollbar->setHeight(D_HEIGHT);
    _scrollbar->setPos(D_WIDTH - SCROLLBAR_WIDTH, 0);

    _mode = MODE_BOOK_SEL;

    setLayout(layout);
}

void ReaderContext::fillBooks(uint16_t pos)
{
    _books_list_menu->deleteWidgets();

    uint16_t pl_sz = _books.size();
    if (pl_sz > 0 && pos >= pl_sz)
        --pos;

    std::vector<MenuItem *> items;
    makeBooksItems(items, pos, _books_list_menu->getItemsNumOnScreen());

    uint16_t books_num = _books.size();

    uint16_t size = items.size();

    for (size_t i = 0; i < size; ++i)
        _books_list_menu->addItem(items[i]);

    _scrollbar->setMax(pl_sz);
    _scrollbar->setValue(pos);
}

void ReaderContext::makeBooksItems(std::vector<MenuItem *> &items, uint16_t file_pos, uint8_t size)
{
    if (file_pos >= _books.size())
        return;

    uint16_t read_to = file_pos + size;

    if (read_to > _books.size())
        read_to = _books.size();

    WidgetCreator creator{_display};
    items.clear();
    items.reserve(read_to - file_pos);

    for (uint16_t i = file_pos; i < read_to; ++i)
    {
        ++file_pos;

        MenuItem *item = creator.getMenuItem(file_pos);
        items.push_back(item);

        Label *lbl = new Label(1, _display);
        item->setLbl(lbl);
        lbl->setTickerInFocus(true);

        lbl->setText(_books[i].getName());
    }
}

void ReaderContext::showReadTmpl()
{
    WidgetCreator creator{_display};
    IWidgetContainer *layout = creator.getEmptyLayout();

    layout->setBackColor(TFT_BLACK);

    _page = new Label(ID_PAGE_LBL, _display);
    layout->addWidget(_page);
    _page->setMultiline(true);
    _page->setWidth(D_WIDTH - 10);
    _page->setHeight(D_HEIGHT - 16);
    _page->setBackColor(TFT_BLACK);
    _page->setTextColor(TFT_WHITE);
    _page->setPos(5, 0);

    _time_lbl = new Label(ID_TIME_LBL, _display);
    layout->addWidget(_time_lbl);
    _time_lbl->setText(EMPTY_TIME);
    _time_lbl->setTextColor(TFT_WHITE);
    _time_lbl->setFontID(4);
    _time_lbl->initWidthToFit();
    _time_lbl->setPos(5, D_HEIGHT - _time_lbl->getHeight());

    _progress_lbl = _time_lbl->clone(ID_PROGRESS_LBL);
    layout->addWidget(_progress_lbl);
    _progress_lbl->setText(EMPTY_READ_PROGRESS);
    _progress_lbl->initWidthToFit();
    _progress_lbl->setPos(getCenterX(_progress_lbl), _time_lbl->getYPos());

    _mode = MODE_BOOK_READ;

    setLayout(layout);
}

void ReaderContext::showSDErrTmpl()
{
    WidgetCreator creator{_display};
    IWidgetContainer *layout = creator.getEmptyLayout();

    Label *msg_lbl = creator.getStatusMsgLable(ID_MSG_LBL, STR_SD_ERR);
    layout->addWidget(msg_lbl);

    _mode = MODE_SD_UNCONN;

    setLayout(layout);
}

//-------------------------------------------------------------------------------------------

ReaderContext::ReaderContext(GraphicsDriver &display) : IContext(display)
{
    WidgetCreator creator{_display};
    EmptyLayout *layout = creator.getEmptyLayout();
    setLayout(layout);

    if (!_f_mgr.isSdMounted())
    {
        showSDErrTmpl();
        return;
    }

    _watch_inited = _watch.begin();

    _old_brightness = atoi(_settings.get(STR_PREF_BRIGHT).c_str());

    if (_old_brightness == 0)
        _old_brightness = 100;

    String book_br = _settings.get(STR_BOOK_BRIGHT_PREF);

    if (book_br.isEmpty())
        _brightness = _old_brightness;
    else
        _brightness = atoi(book_br.c_str());

    _dir_name = _settings.get(STR_BOOK_DIR_PREF);
    _book_name = _settings.get(STR_BOOK_NAME_PREF);
    _read_pos = atoi(_settings.get(STR_READ_POS_PREF).c_str());

    showBookDirsTmpl();
    indexDirs();
    fillBookDirs();
}

void ReaderContext::update()
{
    if (_mode == MODE_SD_UNCONN)
    {
        if (_input.isReleased(KeyID::KEY_BACK))
        {
            _input.lock(KeyID::KEY_BACK, CLICK_LOCK);
            setCpuFrequencyMhz(240);
            openContextByID(ID_CONTEXT_MENU);
        }

        return;
    }

    if (_input.isPressed(KeyID::KEY_OK))
    {
        _input.lock(KeyID::KEY_OK, PRESS_LOCK);
        if (_mode == MODE_BOOK_SEL)
            showContextMenuTmpl();
    }
    else if (_input.isPressed(KeyID::KEY_BACK))
    {
        _input.lock(KeyID::KEY_BACK, PRESS_LOCK);
        backPressed();
    }
    else if (_input.isHolded(KeyID::KEY_UP))
    {
        _input.lock(KeyID::KEY_UP, HOLD_LOCK);
        up();
    }
    else if (_input.isHolded(KeyID::KEY_DOWN))
    {
        _input.lock(KeyID::KEY_DOWN, HOLD_LOCK);
        down();
    }
    else if (_input.isReleased(KeyID::KEY_RIGHT))
    {
        _input.lock(KeyID::KEY_RIGHT, CLICK_LOCK);
        right();
    }
    else if (_input.isReleased(KeyID::KEY_LEFT))
    {
        _input.lock(KeyID::KEY_LEFT, CLICK_LOCK);
        left();
    }
    else if (_input.isReleased(KeyID::KEY_OK))
    {
        _input.lock(KeyID::KEY_OK, CLICK_LOCK);
        ok();
    }
    else if (_input.isReleased(KeyID::KEY_BACK))
    {
        _input.lock(KeyID::KEY_BACK, CLICK_LOCK);
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
}

void ReaderContext::ok()
{
    if (_mode == MODE_BOOK_DIR_SEL)
    {
        uint16_t item_ID = _book_dirs_menu->getCurrentItemID();

        if (item_ID == ID_CONT_ITEM)
        {
            indexBooks();
            openBook(true);
        }
        else
        {
            _dir_name = _book_dirs_menu->getCurrentItemText();
            _book_pos = 0;
            indexBooks();
            showBooksListTmpl();
            fillBooks(_book_pos);
        }
    }
    else if (_mode == MODE_BOOK_SEL)
    {
        if (_books_list_menu->getSize() > 0)
            openBook(false);
    }
    else if (_mode == MODE_CONTEXT_MENU)
    {
        uint16_t id = _context_menu->getCurrentItemID();

        if (id == ID_ITEM_DEL)
        {
            _book_name = _books_list_menu->getCurrentItemText();
            if (_book_name.isEmpty())
                return;

            String book_path = getBookPath(_dir_name.c_str(), _book_name.c_str());

            if (_f_mgr.rmFile(book_path.c_str(), true))
            {
                if (_books_list_menu->getCurrentItemID() - 2 > -1)
                    _book_pos = _books_list_menu->getCurrentItemID() - 2;
                else
                    _book_pos = 0;

                hideContextMenu();
                indexBooks();
                updateBookPos();
                fillBooks(_book_pos);
            }
        }
    }
    else if (_mode == MODE_BOOK_READ)
    {
        _brightness_edit_en = !_brightness_edit_en;

        if (_brightness_edit_en)
        {
            _input.enablePin(KeyID::KEY_UP);
            _input.enablePin(KeyID::KEY_DOWN);
            _input.enablePin(KeyID::KEY_BACK);
            _time_lbl->setVisibility(IWidget::VISIBLE);
            _progress_lbl->setVisibility(IWidget::VISIBLE);
        }
        else
        {
            _input.disablePin(KeyID::KEY_UP);
            _input.disablePin(KeyID::KEY_DOWN);
            _input.disablePin(KeyID::KEY_BACK);
            _time_lbl->setVisibility(IWidget::INVISIBLE);
            _progress_lbl->setVisibility(IWidget::INVISIBLE);
        }
    }
}

void ReaderContext::updateBookPos()
{
    if (_book_pos > 0)
    {
        if (_book_pos >= _books.size())
            _book_pos = _books.size() - 1;
    }
}

void ReaderContext::indexDirs()
{
    String dirs_path = ROOT_PATH;
    _f_mgr.indexDirs(_dirs, dirs_path.c_str());
}

void ReaderContext::indexBooks()
{
    String books_path = ROOT_PATH;
    books_path += "/";
    books_path += _dir_name;
    _f_mgr.indexFilesExt(_books, books_path.c_str(), BOOK_EXT);
}

void ReaderContext::handleNextItemsLoad(std::vector<MenuItem *> &items, uint8_t size, uint16_t cur_id)
{
    if (!cur_id)
        return;

    makeBooksItems(items, cur_id, size);
}

void ReaderContext::handlePrevItemsLoad(std::vector<MenuItem *> &items, uint8_t size, uint16_t cur_id)
{
    if (!cur_id)
        return;

    uint16_t item_pos = cur_id - 1;

    if (!item_pos)
        return;

    if (cur_id > size)
        item_pos = cur_id - size - 1;
    else
    {
        item_pos = 0;
        _scrollbar->setValue(cur_id);
    }

    makeBooksItems(items, item_pos, size);
}

void ReaderContext::onNextItemsLoad(std::vector<MenuItem *> &items, uint8_t size, uint16_t cur_id, void *arg)
{
    ReaderContext *this_ptr = static_cast<ReaderContext *>(arg);
    this_ptr->handleNextItemsLoad(items, size, cur_id);
}

void ReaderContext::onPrevItemsLoad(std::vector<MenuItem *> &items, uint8_t size, uint16_t cur_id, void *arg)
{
    ReaderContext *this_ptr = static_cast<ReaderContext *>(arg);
    this_ptr->handlePrevItemsLoad(items, size, cur_id);
}

//-------------------------------------------------------------------------------------------

void ReaderContext::up()
{
    if (_mode == MODE_BOOK_DIR_SEL)
    {
        _book_dirs_menu->focusUp();
        _scrollbar->scrollUp();
    }
    else if (_mode == MODE_BOOK_SEL)
    {
        _books_list_menu->focusUp();
        _scrollbar->scrollUp();
    }
    else if (_mode == MODE_CONTEXT_MENU)
    {
        _context_menu->focusUp();
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
}

void ReaderContext::down()
{
    if (_mode == MODE_BOOK_DIR_SEL)
    {
        _book_dirs_menu->focusDown();
        _scrollbar->scrollDown();
    }
    else if (_mode == MODE_BOOK_SEL)
    {
        _books_list_menu->focusDown();
        _scrollbar->scrollDown();
    }
    else if (_mode == MODE_CONTEXT_MENU)
    {
        _context_menu->focusDown();
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
}

void ReaderContext::left()
{
    if (_mode == MODE_BOOK_READ)
        loadPrevTxt();
}

void ReaderContext::right()
{
    if (_mode == MODE_BOOK_READ)
        loadNextTxt();
}

void ReaderContext::back()
{
    if (_mode == MODE_BOOK_DIR_SEL)
    {
        openContextByID(ID_CONTEXT_MENU);
    }
    else if (_mode == MODE_BOOK_SEL)
    {
        indexDirs();
        showBookDirsTmpl();
        fillBookDirs();
    }
    else if (_mode == MODE_CONTEXT_MENU)
    {
        hideContextMenu();
    }
}

void ReaderContext::backPressed()
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
        showBooksListTmpl();
        fillBooks(_book_pos);
    }
}

void ReaderContext::openBook(bool contn)
{
    if (contn)
    {
        if ((_dir_name.isEmpty() || _book_name.isEmpty()))
            return;
    }
    else
    {
        _book_name = _books_list_menu->getCurrentItemText();
        _book_pos = _books_list_menu->getCurrentItemID() - 1;
        _read_pos = 0;
    }

    String book_path = getBookPath(_dir_name.c_str(), _book_name.c_str());

    _book_size = _f_mgr.getFileSize(book_path.c_str());

    if (containCyrillic(_dir_name.c_str(), _book_name.c_str()))
        _num_char_to_read = KIR_NUM_NYTES_TO_READ;
    else
        _num_char_to_read = LAT_NUM_BYTES_TO_READ;

    if (_book_size > 0)
    {
        showReadTmpl();
        loadNextTxt();
        updateReadProgress();
        updateTime();
    }
}

void ReaderContext::loadNextTxt()
{
    setCpuFrequencyMhz(240);

    String txt;
    readText(txt, _dir_name.c_str(), _book_name.c_str(), _num_char_to_read, _read_pos);

    if (!txt.isEmpty())
    {
        _read_pos += txt.length();

        _page->setText(txt);
        updateReadProgress();
    }

    setCpuFrequencyMhz(80);
}

void ReaderContext::loadPrevTxt()
{
    if (_read_pos == 0)
        return;

    setCpuFrequencyMhz(240);

    if (_read_pos > _num_char_to_read)
        _read_pos -= _num_char_to_read;
    else
        _read_pos = 0;

    String txt;
    readText(txt, _dir_name.c_str(), _book_name.c_str(), _num_char_to_read, _read_pos);

    if (!txt.isEmpty())
    {
        _page->setText(txt);
        updateReadProgress();
    }

    setCpuFrequencyMhz(80);
}

void ReaderContext::updateTime()
{
    if (!_watch_inited)
        return;

    DS3231DateTime date_time = _watch.getDateTime();

    if (date_time == _temp_date_time)
        return;

    _temp_date_time = date_time;

    _time_lbl->setText(_temp_date_time.timeToStr());
    _time_lbl->updateWidthToFit();
}

void ReaderContext::updateReadProgress()
{
    String prog_txt;
    prog_txt += _read_pos / _num_char_to_read;
    prog_txt += "/";
    prog_txt += _book_size / _num_char_to_read;

    _progress_lbl->setText(prog_txt);
    _progress_lbl->updateWidthToFit();
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------

String ReaderContext::getBookPath(const char *dir_name, const char *book_name)
{
    String book_path = ROOT_PATH;
    book_path += "/";
    book_path += dir_name;
    book_path += "/";
    book_path += book_name;
    return book_path;
}

bool ReaderContext::isCyrillic(char ch)
{
    unsigned char uc = static_cast<unsigned char>(ch);
    return (uc >= 0xC0 && uc <= 0xFF) || (uc >= 0x80 && uc <= 0xBF);
}

bool ReaderContext::containCyrillic(const char *dir_name, const char *book_name)
{
    String path = getBookPath(dir_name, book_name);

    const uint8_t ARR_SIZE = 160;
    char ch_arr[ARR_SIZE];

    size_t read_bytes = _f_mgr.readFile(ch_arr, path.c_str(), ARR_SIZE);

    for (size_t i{0}; i < read_bytes; ++i)
    {
        if (isCyrillic(ch_arr[i]))
            return true;
    }

    return false;
}

bool ReaderContext::readText(String &out_str, const char *dir_name, const char *book_name, size_t len, size_t pos)
{
    char *buffer;
    if (psramInit())
        buffer = (char *)ps_malloc(len + 1);
    else
        buffer = (char *)malloc(len + 1);

    if (!buffer)
    {
        log_e("alloc error: %zu b", len + 1);
        out_str = "";
        return false;
    }

    String book_path = getBookPath(dir_name, book_name);

    size_t bytes_read = _f_mgr.readFile(buffer, book_path.c_str(), len, pos);
    buffer[bytes_read] = '\0';

    bool is_oef = false;

    if (bytes_read != len)
        is_oef = true;

    if (bytes_read > 2 && (buffer[bytes_read - 1] & 0x80) != 0)
    {
        size_t correct_char_pos{0};

        for (size_t i{bytes_read - 2}; i > 0; --i)
        {
            if ((buffer[i] & 0x80) == 0)
            {
                correct_char_pos = i;
                break;
            }
        }

        if (correct_char_pos > 0)
        {
            char *temp_buff = (char *)malloc(correct_char_pos + 2);
            if (temp_buff)
            {
                memcpy(temp_buff, buffer, correct_char_pos + 1);
                temp_buff[correct_char_pos + 1] = '\0';
                out_str = String(temp_buff, correct_char_pos);
                free(temp_buff);
            }
            else
                log_e("Помилка створення temp буферу на %zu байт", len);
        }
    }
    else
        out_str = String(buffer, bytes_read);

    free(buffer);
    return !is_oef;
}
