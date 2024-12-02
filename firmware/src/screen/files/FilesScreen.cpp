#include "FilesScreen.h"
#pragma GCC optimize("O3")

#include "meow/util/display/DisplayUtil.h"
#include "meow/lib/qr/QR_Gen.h"
#include "meow/manager/settings/SettingsManager.h"
//
#include "meow/ui/widget/progress/ProgressBar.h"
#include "meow/ui/widget/menu/item/MenuItem.h"

#include "../WidgetCreator.h"

#include "../resources/color.h"
#include "../resources/string.h"
#include "../resources/const.h"
#include "./res/folder.h"

const char FILES_DB_NAME[] = "files.ldb";

void FilesScreen::loop()
{
}

FilesScreen::FilesScreen(GraphicsDriver &display) : IScreen(display)
{
    WidgetCreator creator{_display};
    EmptyLayout *layout = creator.getEmptyLayout();
    setLayout(layout);

    if (!_file_manager.hasConnection())
    {
        showSDErrTmpl();
        return;
    }

    showFilesTmpl();
    showDir(true);
}

//-------------------------------------------------------------------------------------------

void FilesScreen::showSDErrTmpl()
{
    WidgetCreator creator{_display};
    IWidgetContainer *layout = creator.getEmptyLayout();

    layout->addWidget(creator.getNavbar(ID_NAVBAR, "", "", STR_EXIT));

    _msg_lbl = creator.getStatusMsgLable(ID_MSG_LBL, STR_SD_ERR);
    layout->addWidget(_msg_lbl);

    _mode = MODE_SD_UNCONN;
    setLayout(layout);
}

void FilesScreen::showServerTmpl()
{
    WidgetCreator creator{_display};
    IWidgetContainer *layout = creator.getEmptyLayout();

    layout->addWidget(creator.getNavbar(ID_NAVBAR, "", "", STR_STOP));

    String header_str;

    if (_server.getMode() == FileServer::SERVER_MODE_SEND)
        header_str = STR_EXPORT;
    else
        header_str = STR_IMPORT;

    header_str += ": ";
    header_str += _server.getAddress().c_str();

    _msg_lbl = creator.getStatusMsgLable(ID_MSG_LBL, header_str.c_str());
    layout->addWidget(_msg_lbl);
    _msg_lbl->setHeight(_msg_lbl->getCharHgt() + 4);

    if (_qr_img_buff)
    {
        _qr_img = new Image(ID_QR_IMG, _display);
        layout->addWidget(_qr_img);
        _qr_img->init(_qr_width, _qr_width);
        _qr_img->setSrc(_qr_img_buff);
        _qr_img->setPos((DWIDTH - _qr_width) / 2, (DHEIGHT - _qr_width) / 2 - NAVBAR_HEIGHT);
    }

    _mode = MODE_FILE_SERVER;
    setLayout(layout);
}

void FilesScreen::showUpdatingTmpl()
{
    WidgetCreator creator{_display};
    IWidgetContainer *layout = creator.getEmptyLayout();

    layout->addWidget(creator.getNavbar(ID_NAVBAR, "", "", STR_CANCEL));

    _msg_lbl = creator.getStatusMsgLable(ID_MSG_LBL, STR_UPDATING, 2);
    layout->addWidget(_msg_lbl);

    _mode = MODE_UPDATING;
    _has_task = true;

    setLayout(layout);
}

void FilesScreen::showCopyingTmpl()
{
    WidgetCreator creator{_display};
    IWidgetContainer *layout = creator.getEmptyLayout();

    layout->addWidget(creator.getNavbar(ID_NAVBAR, "", "", STR_CANCEL));

    _msg_lbl = creator.getStatusMsgLable(ID_MSG_LBL, STR_COPYING, 2);
    layout->addWidget(_msg_lbl);
    _msg_lbl->setHeight(32);
    _msg_lbl->setPos(0, _display.height() / 2 - _msg_lbl->getHeight() - 2);

    _task_progress = new ProgressBar(ID_PROGRESS, _display);
    layout->addWidget(_task_progress);
    _task_progress->setBackColor(TFT_BLACK);
    _task_progress->setProgressColor(TFT_ORANGE);
    _task_progress->setBorderColor(TFT_WHITE);
    _task_progress->setMax(100);
    _task_progress->setWidth(_display.width() - 5 * 8);
    _task_progress->setHeight(20);
    _task_progress->setProgress(0);
    _task_progress->setPos((_display.width() - _task_progress->getWidth()) / 2, _display.height() / 2 + 2);

    _mode = MODE_COPYING;
    _has_task = true;

    setLayout(layout);
}

void FilesScreen::showRemovingTmpl()
{
    WidgetCreator creator{_display};
    IWidgetContainer *layout = creator.getEmptyLayout();

    layout->addWidget(creator.getNavbar(ID_NAVBAR, "", "", STR_CANCEL));

    _msg_lbl = creator.getStatusMsgLable(ID_MSG_LBL, STR_REMOVING, 2);
    layout->addWidget(_msg_lbl);

    _mode = MODE_REMOVING;
    _has_task = true;

    setLayout(layout);
}

void FilesScreen::showCancelingTmpl()
{
    WidgetCreator creator{_display};
    IWidgetContainer *layout = creator.getEmptyLayout();

    layout->addWidget(creator.getNavbar(ID_NAVBAR, "", "", ""));

    _msg_lbl = creator.getStatusMsgLable(ID_MSG_LBL, STR_CANCELING, 2);
    layout->addWidget(_msg_lbl);

    _mode = MODE_CANCELING;
    _has_task = true;

    setLayout(layout);
}

void FilesScreen::showFilesTmpl()
{
    WidgetCreator creator{_display};
    IWidgetContainer *layout = creator.getEmptyLayout();

    layout->setBackColor(TFT_BLACK);

    layout->addWidget(creator.getNavbar(ID_NAVBAR, STR_SELECT, "", STR_BACK));

    _files_list = creator.getDynamicMenu(ID_DYNAMIC_MENU, this);
    layout->addWidget(_files_list);
    _files_list->setHeight(_display.height() - NAVBAR_HEIGHT - 1);
    _files_list->setItemHeight((_display.height() - NAVBAR_HEIGHT - 2) / MENU_ITEMS_NUM);
    _files_list->setWidth(_display.width() - SCROLLBAR_WIDTH);

    _scrollbar = new ScrollBar(ID_SCROLLBAR, _display);
    layout->addWidget(_scrollbar);
    _scrollbar->setWidth(SCROLLBAR_WIDTH);
    _scrollbar->setHeight(_display.height() - NAVBAR_HEIGHT);
    _scrollbar->setPos(_display.width() - SCROLLBAR_WIDTH, 0);
    _scrollbar->setBackColor(COLOR_MAIN_BACK);

    _mode = MODE_NAVIGATION;

    setLayout(layout);
}

//-------------------------------------------------------------------------------------------

std::vector<MenuItem *> FilesScreen::getMenuFilesItems(const char *path, uint16_t file_pos, uint8_t size)
{
    String db_path = path;
    db_path += "/";
    db_path += FILES_DB_NAME;

    std::vector<String> files = _file_manager.readFilesFromDB(db_path.c_str(), file_pos, size);

    WidgetCreator creator{_display};
    MenuItem *template_item = creator.getMenuItem();

    std::vector<MenuItem *> ret;
    ret.reserve(files.size());

    for (size_t i = 0; i < files.size(); ++i)
    {
        ++file_pos;
        MenuItem *item = template_item->clone(file_pos);
        ret.push_back(item);

        Label *lbl = new Label(1, _display);
        item->setLbl(lbl);
        lbl->setTickerInFocus(true);

        if (files[i].startsWith(FileManager::STR_DIR_PREFIX))
        {
            Image *dir_img = new Image(1, _display);
            item->setImg(dir_img);
            dir_img->setTransparentColor(Image::COLOR_TRANSPARENT);
            dir_img->init(16, 16);
            dir_img->setSrc(FOLDER_IMG);
            String clean_name = files[i].substring(3);
            lbl->setText(clean_name);
        }
        else
            lbl->setText(files[i]);
    }

    delete template_item;
    return ret;
}

std::vector<MenuItem *> FilesScreen::loadPrev(uint8_t size, uint16_t current_ID)
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

    String path = makePathFromBreadcrumbs();
    return getMenuFilesItems(path.c_str(), item_pos, size);
}

std::vector<MenuItem *> FilesScreen::loadNext(uint8_t size, uint16_t current_ID)
{
    if (!current_ID)
        return std::vector<MenuItem *>();

    String path = makePathFromBreadcrumbs();
    return getMenuFilesItems(path.c_str(), current_ID, size);
}

//-------------------------------------------------------------------------------------------

String FilesScreen::makePathFromBreadcrumbs()
{
    String ret_str;

    if (_breadcrumbs.size() > 0)
        for (uint8_t i{0}; i < _breadcrumbs.size(); ++i)
        {
            ret_str += "/";
            ret_str += _breadcrumbs[i];
        }

    if (ret_str.equals("//"))
        ret_str = "/";

    return ret_str;
}

void FilesScreen::showContextMenu()
{
    _mode = MODE_CONTEXT_MENU;
    _files_list->disable();

    WidgetCreator creator{_display};

    _context_menu = new FixedMenu(ID_CONTEXT_MENU, _display);
    getLayout()->addWidget(_context_menu);
    _context_menu->setItemHeight(18);
    _context_menu->setWidth((float)_display.width() / 2.2);
    _context_menu->setBackColor(TFT_BLACK);
    _context_menu->setBorder(true);
    _context_menu->setBorderColor(TFT_ORANGE);

    if (_has_moving_file || _has_copying_file)
    {
        // Якщо є файл для переміщення додати відповідний пункт меню
        MenuItem *paste_item = creator.getMenuItem(ID_ITEM_PASTE);
        _context_menu->addItem(paste_item);

        Label *paste_lbl = creator.getItemLabel(STR_PASTE, 2);
        paste_item->setLbl(paste_lbl);
        paste_lbl->setTextOffset(1);
    }

    // новий каталог
    MenuItem *new_dir_item = creator.getMenuItem(ID_ITEM_NEW_DIR);
    _context_menu->addItem(new_dir_item);

    Label *new_dir_lbl = creator.getItemLabel(STR_NEW_DIR, 2);
    new_dir_item->setLbl(new_dir_lbl);
    new_dir_lbl->setTextOffset(1);

    // контекст для вибраного файлу
    String cur_item_str = _files_list->getCurrentItemText();

    if (!cur_item_str.isEmpty())
    {
        // перейменувати
        MenuItem *rename_item = creator.getMenuItem(ID_ITEM_RENAME);
        _context_menu->addItem(rename_item);

        Label *rename_lbl = creator.getItemLabel(STR_RENAME, 2);
        rename_item->setLbl(rename_lbl);
        rename_lbl->setTextOffset(1);

        // копіювати
        MenuItem *copy_item = creator.getMenuItem(ID_ITEM_COPY);
        _context_menu->addItem(copy_item);

        Label *copy_lbl = creator.getItemLabel(STR_COPY, 2);
        copy_item->setLbl(copy_lbl);
        copy_lbl->setTextOffset(1);

        // перемістити
        MenuItem *move_item = creator.getMenuItem(ID_ITEM_MOVE);
        _context_menu->addItem(move_item);

        Label *move_lbl = creator.getItemLabel(STR_MOVE, 2);
        move_item->setLbl(move_lbl);
        move_lbl->setTextOffset(1);

        // видалити
        MenuItem *delete_item = creator.getMenuItem(ID_ITEM_REMOVE);
        _context_menu->addItem(delete_item);

        Label *delete_lbl = creator.getItemLabel(STR_DELETE, 2);
        delete_item->setLbl(delete_lbl);
        delete_lbl->setTextOffset(1);
    }

    // імпорт
    MenuItem *import_item = creator.getMenuItem(ID_ITEM_IMPORT);
    _context_menu->addItem(import_item);

    Label *import_lbl = creator.getItemLabel(STR_IMPORT, 2);
    import_item->setLbl(import_lbl);
    import_lbl->setTextOffset(1);

    // експорт
    MenuItem *export_item = creator.getMenuItem(ID_ITEM_EXPORT);
    _context_menu->addItem(export_item);

    Label *export_lbl = creator.getItemLabel(STR_EXPORT, 2);
    export_item->setLbl(export_lbl);
    export_lbl->setTextOffset(1);

    // оновити
    MenuItem *upd_item = creator.getMenuItem(ID_ITEM_UPDATE);
    _context_menu->addItem(upd_item);

    Label *upd_lbl = creator.getItemLabel(STR_UPDATE, 2);
    upd_item->setLbl(upd_lbl);
    upd_lbl->setTextOffset(1);

    //
    _context_menu->setHeight(_context_menu->getSize() * _context_menu->getItemHeight() + 4);
    _context_menu->setPos(_display.width() - _context_menu->getWidth() - 1,
                          _display.height() - NAVBAR_HEIGHT - _context_menu->getHeight() - 2);
}

void FilesScreen::hideContextMenu()
{
    getLayout()->deleteWidgetByID(ID_CONTEXT_MENU);
    _mode = MODE_NAVIGATION;
    _files_list->enable();
}

//-------------------------------------------------------------------------------------------

void FilesScreen::showDialog(Mode mode)
{
    WidgetCreator creator{_display};
    IWidgetContainer *layout = creator.getEmptyLayout();

    _dialog_txt = new TextBox(ID_DIALOG_TXT, _display);
    _dialog_txt->setTextOffset(5);
    _dialog_txt->setWidth(_display.width() - 10);
    _dialog_txt->setHeight(40);
    _dialog_txt->setBackColor(TFT_WHITE);
    _dialog_txt->setTextColor(TFT_BLACK);
    _dialog_txt->setFontID(2);
    _dialog_txt->setTextSize(2);
    _dialog_txt->setPos(5, 5);
    _dialog_txt->setCornerRadius(3);

    _mode = mode;
    if (_mode == MODE_RENAME_DIALOG)
    {
        _old_name = _files_list->getCurrentItemText();
        _dialog_txt->setText(_old_name);
    }

    _keyboard = creator.getStandardEnKeyboard(ID_KEYBOARD);

    layout->setBackColor(TFT_BLACK);
    layout->addWidget(_dialog_txt);
    layout->addWidget(creator.getNavbar(ID_NAVBAR, STR_SELECT, "", STR_ERASE));
    layout->addWidget(_keyboard);

    setLayout(layout);
}

void FilesScreen::hideDialog()
{
    if (_dialog_success_res)
    {
        _dialog_success_res = false;
        String db_path = makePathFromBreadcrumbs();
        db_path += "/";
        db_path += FILES_DB_NAME;

        if (_file_manager.startRemoving(db_path.c_str(), [this]
                                        {
                                            String db_path = makePathFromBreadcrumbs();

                                            updateDir(db_path.c_str()); }))
        {
            showUpdatingTmpl();
            return;
        }
    }
    showFilesTmpl();
    showDir(true);
}

void FilesScreen::saveDialogResult()
{
    if (_mode == MODE_NEW_DIR_DIALOG)
    {
        String dir_path = makePathFromBreadcrumbs();
        dir_path += "/";
        dir_path += _dialog_txt->getText();

        _dialog_success_res = _file_manager.createDir(dir_path.c_str());
    }
    else if (_mode = MODE_RENAME_DIALOG)
    {
        String old_name = makePathFromBreadcrumbs();
        old_name += "/";
        old_name += _old_name;
        _old_name = "";

        String new_name = makePathFromBreadcrumbs();
        new_name += "/";
        new_name += _dialog_txt->getText();
        _dialog_success_res = _file_manager.rename(old_name.c_str(), new_name.c_str());
    }

    hideDialog();
}

void FilesScreen::keyboardClickHandler()
{
    // uint16_t id = _keyboard->getCurrentBtnID();
    // if (id ==)
    // {
    // }

    // Можна оброблювати по ID кнопки, але в даному випадку зручніше оброблювати текст,
    // Тому що ніякі керуючі кнопки не використовуються.

    _dialog_txt->addChars(_keyboard->getCurrentBtnTxt().c_str());
}

//-------------------------------------------------------------------------------------------

void FilesScreen::prepareFileMoving()
{
    _path_from = makePathFromBreadcrumbs();
    _name_from = _files_list->getCurrentItemText();

    _has_moving_file = true;
    _has_copying_file = false;

    hideContextMenu();
}

void FilesScreen::prepareFileCopying()
{
    _path_from = makePathFromBreadcrumbs();
    _name_from = _files_list->getCurrentItemText();

    String temp = _path_from;
    temp += "/";
    temp += _name_from;

    if (!_file_manager.fileExist(temp.c_str()))
    {
        _path_from = "";
        _name_from = "";
        return;
    }

    _has_moving_file = false;
    _has_copying_file = true;

    hideContextMenu();
}

void FilesScreen::pasteFile()
{
    String old_file_path = _path_from;
    old_file_path += "/";
    old_file_path += _name_from;

    String cur_dir = makePathFromBreadcrumbs();
    String new_file_path = cur_dir;
    new_file_path += "/";
    new_file_path += _name_from;

    if (_has_moving_file)
    {
        if (_file_manager.exists(old_file_path.c_str()))
        {
            if (_file_manager.rename(old_file_path.c_str(), new_file_path.c_str()))
            {
                String old_db = _path_from;
                old_db += "/";
                old_db += FILES_DB_NAME;

                if (_file_manager.startRemoving(old_db.c_str(), [this, cur_dir]
                                                {
                                                    String cur_db = cur_dir;
                                                    cur_db += "/";
                                                    cur_db += FILES_DB_NAME;

                                                    if (!_file_manager.startRemoving(cur_db.c_str(), [this]  { showDir(true); }))
                                                    {
                                                        showDir(true);
                                                    } }))
                {
                    showUpdatingTmpl();
                }
                else
                {
                    showSDErrTmpl();
                }
            }
        }
    }
    else if (_has_copying_file)
    {
        if (_file_manager.fileExist(old_file_path.c_str()))
        {
            if (_file_manager.startCopyingFile(old_file_path.c_str(), new_file_path.c_str(),
                                               [this, cur_dir]
                                               {
                                                   String db_path = cur_dir;
                                                   db_path += "/";
                                                   db_path += FILES_DB_NAME;

                                                   if (!_file_manager.startRemoving(db_path.c_str(), [this]
                                                                                    { showDir(true); }))
                                                   {
                                                       showDir(true);
                                                   } }))
            {
                showCopyingTmpl();
            }
        }
    }

    _has_moving_file = false;
    _has_copying_file = false;

    _path_from = "";
    _name_from = "";
}

void FilesScreen::removeFile()
{
    String file_name = makePathFromBreadcrumbs();
    file_name += "/";
    file_name += _files_list->getCurrentItemText();

    if (_file_manager.startRemoving(file_name.c_str(), [this]
                                    {
                                                String db_name = makePathFromBreadcrumbs();
                                                db_name += "/";
                                                db_name += FILES_DB_NAME;

                                                if (!_file_manager.startRemoving(db_name.c_str(), [this] { showDir(true); }))
                                                {
                                                    showFilesTmpl();
                                                    showDir(true);
                                                } }))
        showRemovingTmpl();
}

//-------------------------------------------------------------------------------------------

void FilesScreen::update()
{
    if (_mode == MODE_SD_UNCONN)
    {
        if (_input.isReleased(KeyID::KEY_BACK))
        {
            _input.lock(KeyID::KEY_BACK, 500);
            openScreenByID(ID_SCREEN_MENU);
        }

        return;
    }

    if (_input.isPressed(KeyID::KEY_OK))
    {
        _input.lock(KeyID::KEY_OK, 1500);
        if (_mode == MODE_NAVIGATION)
            showContextMenu();
        else if (_mode == MODE_NEW_DIR_DIALOG || _mode == MODE_RENAME_DIALOG)
            saveDialogResult();
        else if (_mode == MODE_FILE_SERVER)
            switchBackLight();
    }
    else if (_input.isPressed(KeyID::KEY_BACK))
    {
        _input.lock(KeyID::KEY_BACK, 1500);

        if (_mode == MODE_NAVIGATION)
            openScreenByID(ID_SCREEN_MENU);
        else if (_mode == MODE_NEW_DIR_DIALOG || _mode == MODE_RENAME_DIALOG)
            hideDialog();
    }
    else if (_input.isHolded(KeyID::KEY_UP))
    {
        _input.lock(KeyID::KEY_UP, KEY_LOCK_DUR);
        if (_mode == MODE_NAVIGATION)
        {
            _files_list->focusUp();
            _scrollbar->scrollUp();
        }
        else if (_mode == MODE_CONTEXT_MENU)
            _context_menu->focusUp();
        else if (_mode == MODE_NEW_DIR_DIALOG || _mode == MODE_RENAME_DIALOG)
            _keyboard->focusUp();
    }
    else if (_input.isHolded(KeyID::KEY_DOWN))
    {
        _input.lock(KeyID::KEY_DOWN, KEY_LOCK_DUR);
        if (_mode == MODE_NAVIGATION)
        {
            _files_list->focusDown();
            _scrollbar->scrollDown();
        }
        else if (_mode == MODE_CONTEXT_MENU)
            _context_menu->focusDown();
        else if (_mode == MODE_NEW_DIR_DIALOG || _mode == MODE_RENAME_DIALOG)
            _keyboard->focusDown();
    }
    else if (_input.isHolded(KeyID::KEY_RIGHT))
    {
        _input.lock(KeyID::KEY_RIGHT, KEY_LOCK_DUR);
        if (_mode == MODE_NEW_DIR_DIALOG || _mode == MODE_RENAME_DIALOG)
            _keyboard->focusRight();
    }
    else if (_input.isHolded(KeyID::KEY_LEFT))
    {
        _input.lock(KeyID::KEY_LEFT, KEY_LOCK_DUR);
        if (_mode == MODE_NEW_DIR_DIALOG || _mode == MODE_RENAME_DIALOG)
            _keyboard->focusLeft();
    }
    else if (_input.isReleased(KeyID::KEY_OK))
    {
        _input.lock(KeyID::KEY_OK, KEY_LOCK_DUR);
        ok();
    }
    else if (_input.isReleased(KeyID::KEY_BACK))
    {
        _input.lock(KeyID::KEY_BACK, KEY_LOCK_DUR);
        back();
    }

    if (_has_task)
    {
        if (!_file_manager.isWorking())
            _has_task = false;
        else if ((millis() - _upd_msg_time) > UPD_TRACK_INF_INTERVAL)
        {
            String upd_txt;
            String upd_progress;

            if (_upd_counter > 2)
                _upd_counter = 0;
            else
                ++_upd_counter;

            for (uint8_t i{0}; i < _upd_counter; ++i)
                upd_progress += ".";

            if (_mode == MODE_CANCELING)
            {
                upd_txt = STR_CANCELING;
                upd_txt += upd_progress;
                _msg_lbl->setText(upd_txt);
            }
            else if (_mode == MODE_UPDATING)
            {
                upd_txt = STR_UPDATING;
                upd_txt += upd_progress;
                _msg_lbl->setText(upd_txt);
            }
            else if (_mode == MODE_COPYING)
            {
                _task_progress->setProgress(_file_manager.getCopyProgress());
                _upd_msg_time = millis();
            }
            else if (_mode == MODE_REMOVING)
            {
                upd_txt = STR_REMOVING;
                upd_txt += upd_progress;
                _msg_lbl->setText(upd_txt);
            }

            _upd_msg_time = millis();
        }
    }
}

void FilesScreen::ok()
{
    if (_mode == MODE_NAVIGATION)
        openNextLevel();
    else if (_mode == MODE_CONTEXT_MENU)
    {
        uint16_t id = _context_menu->getCurrentItemID();

        if (id == ID_ITEM_UPDATE)
            updateDir(makePathFromBreadcrumbs().c_str());
        else if (id == ID_ITEM_REMOVE)
            removeFile();
        else if (id == ID_ITEM_MOVE)
            prepareFileMoving();
        else if (id == ID_ITEM_COPY)
            prepareFileCopying();
        else if (id == ID_ITEM_PASTE)
            pasteFile();
        else if (id == ID_ITEM_NEW_DIR)
            showDialog(MODE_NEW_DIR_DIALOG);
        else if (id == ID_ITEM_RENAME)
            showDialog(MODE_RENAME_DIALOG);
        else if (id == ID_ITEM_IMPORT)
            startFileServer(FileServer::SERVER_MODE_RECEIVE);
        else if (id == ID_ITEM_EXPORT)
            startFileServer(FileServer::SERVER_MODE_SEND);
    }
    else if (_mode == MODE_NEW_DIR_DIALOG || _mode == MODE_RENAME_DIALOG)
        keyboardClickHandler();
}

void FilesScreen::back()
{
    if (_file_manager.isWorking() && _mode != MODE_CANCELING)
    {
        showCancelingTmpl();
        _file_manager.cancelTask();
    }
    else if (_mode == MODE_CONTEXT_MENU)
        hideContextMenu();
    else if (_mode == MODE_NAVIGATION)
        openPrevlevel();
    else if (_mode == MODE_NEW_DIR_DIALOG || _mode == MODE_RENAME_DIALOG)
        _dialog_txt->removeLastChar();
    else if (_mode == MODE_FILE_SERVER)
        stopFileServer();
}

void FilesScreen::openNextLevel()
{
    String next_dir = makePathFromBreadcrumbs();
    next_dir += "/";
    next_dir += _files_list->getCurrentItemText();

    if (!_file_manager.dirExist(next_dir.c_str()))
        return;

    _breadcrumbs.push_back(_files_list->getCurrentItemText());
    showDir(true);
}

void FilesScreen::openPrevlevel()
{
    if (_breadcrumbs.size() > 0)
    {
        _breadcrumbs.pop_back();
        showDir(true);
    }
}

void FilesScreen::showDir(bool need_update)
{
    String path = makePathFromBreadcrumbs();
    String db_path = path;
    db_path += "/";
    db_path += FILES_DB_NAME;

    if (!_file_manager.fileExist(db_path.c_str(), true))
        if (need_update)
        {
            updateDir(path.c_str());
            return;
        }
        else
            return;

    if (_mode == MODE_NAVIGATION)
    {
        _files_list->deleteWidgets();

        uint16_t db_sz = _file_manager.getDBSize(db_path.c_str());
        _scrollbar->setValue(0);
        _scrollbar->setMax(db_sz);

        std::vector<MenuItem *> items = getMenuFilesItems(path.c_str(), 0, _files_list->getItemsNumOnScreen());
        for (size_t i = 0; i < items.size(); ++i)
            _files_list->addItem(items[i]);
    }
    else
    {
        log_e("MODE != MODE_NAVIGATION");
        esp_restart();
    }
}

void FilesScreen::updateDir(const char *dir_path)
{
    String db_path = dir_path;
    db_path += "/";
    db_path += FILES_DB_NAME;
    uint16_t db_sz = _file_manager.getDBSize(db_path.c_str());

    String dir = dir_path;
    if (dir.isEmpty())
        dir = "/";

    if (_file_manager.indexAll(dir.c_str(), db_path.c_str(), [this]
                               { 
                                showFilesTmpl();
                                showDir(false); }))
    {
        showUpdatingTmpl();
    }
    else
    {
        showSDErrTmpl();
    }
}

void FilesScreen::startFileServer(FileServer::ServerMode mode)
{
    SettingsManager sm;
    String ssid = sm.get(STR_PREF_FS_AP_SSID);
    String pwd = sm.get(STR_PREF_FS_AP_PWD);

    if (ssid.isEmpty())
        ssid = STR_DEF_SSID;
    if (pwd.isEmpty())
        pwd = STR_DEF_PWD;

    _server.setSSID(ssid.c_str());
    _server.setPWD(pwd.c_str());
    //
    _server.setDomainName(ssid.c_str());

    String cur_path = makePathFromBreadcrumbs();
    if (_server.begin(cur_path.c_str(), mode))
    {
        QR_Gen gen;
        String addr = _server.getAddress();
        _qr_img_buff = gen.generateQR(addr.c_str(), 3);
        _qr_width = gen.getImageWidth();

        showServerTmpl();
    }
}

void FilesScreen::stopFileServer()
{
    if (!_is_back_eabled)
        return;

    _server.stop();

    showFilesTmpl();
    showDir(true);

    free(_qr_img_buff);
}

void FilesScreen::switchBackLight()
{
    DisplayUtil display;

    if (_is_back_eabled)
    {
        _display_brightness = display.getBrightness();
        display.setBrightness(0);
        _screen_enabled = false;
    }
    else
    {
        display.setBrightness(_display_brightness);
        _screen_enabled = true;
    }

    _is_back_eabled = !_is_back_eabled;
}
