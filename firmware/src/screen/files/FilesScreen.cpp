#include "FilesScreen.h"

#include "meow/util/sd/SdUtil.h"
#include "meow/util/files/FileManager.h"
//
#include "meow/ui/widget/progress/ProgressBar.h"
#include "meow/ui/widget/menu/item/MenuItem.h"

#include "../WidgetCreator.h"

#include "../resources/color.h"
#include "../resources/string.h"
#include "../resources/const.h"

#include "./res/folder.h"

const char FILES_DB_NAME[] = "files.ldb";

FilesScreen::~FilesScreen()
{
    SdUtil sd;
    sd.end();
}

void FilesScreen::loop()
{
}

void FilesScreen::showSDErr()
{
    _mode = MODE_BAD_CONNECT;
    getLayout()->deleteWidgets();
    WidgetCreator creator{_display};
    getLayout()->addWidget(creator.getNavbar(ID_NAVBAR, "", "", STR_EXIT));

    Label *err_lbl = new Label(ID_ERR_LBL, _display);
    getLayout()->addWidget(err_lbl);
    err_lbl->setText(STR_SD_ERR);
    err_lbl->setAlign(IWidget::ALIGN_CENTER);
    err_lbl->setGravity(IWidget::GRAVITY_CENTER);
    err_lbl->setBackColor(COLOR_MAIN_BACK);
    err_lbl->setWidth(_display.width());
    err_lbl->setHeight(_display.height() - NAVBAR_HEIGHT);
}

FilesScreen::FilesScreen(GraphicsDriver &display) : IScreen(display)
{
    showFiles();
    loadDir(true);
}

void FilesScreen::showDirUpdating()
{
    _mode = MODE_UPDATING;

    WidgetCreator creator{_display};
    getLayout()->deleteWidgets();
    getLayout()->addWidget(creator.getNavbar(ID_NAVBAR, "", "", STR_CANCEL));

    _task_lbl = new Label(ID_UPD_LBL, _display);
    getLayout()->addWidget(_task_lbl);
    _task_lbl->setText(STR_UPDATING);
    _task_lbl->setTextSize(2);
    _task_lbl->setAlign(IWidget::ALIGN_CENTER);
    _task_lbl->setGravity(IWidget::GRAVITY_CENTER);
    _task_lbl->setBackColor(COLOR_MAIN_BACK);
    _task_lbl->setWidth(_display.width());
    _task_lbl->setHeight(_display.height() - NAVBAR_HEIGHT);
}

void FilesScreen::showCopying()
{
    _mode = MODE_COPYING;

    WidgetCreator creator{_display};
    getLayout()->deleteWidgets();
    getLayout()->addWidget(creator.getNavbar(ID_NAVBAR, "", "", STR_CANCEL));

    _task_lbl = new Label(ID_UPD_LBL, _display);
    getLayout()->addWidget(_task_lbl);
    _task_lbl->setText(STR_UPDATING);
    _task_lbl->setTextSize(2);
    _task_lbl->setGravity(IWidget::GRAVITY_CENTER);
    _task_lbl->setAlign(IWidget::ALIGN_CENTER);
    _task_lbl->setBackColor(COLOR_MAIN_BACK);
    _task_lbl->setWidth(_display.width());
    _task_lbl->setHeight(32);
    _task_lbl->setPos(0, _display.height() / 2 - _task_lbl->getHeight() - 2);

    _task_progress = new ProgressBar(ID_PROGRESS, _display);
    getLayout()->addWidget(_task_progress);
    _task_progress->setBackColor(TFT_BLACK);
    _task_progress->setProgressColor(TFT_ORANGE);
    _task_progress->setBorderColor(TFT_WHITE);
    _task_progress->setMax(100);
    _task_progress->setWidth(_display.width() - 5 * 8);
    _task_progress->setHeight(20);
    _task_progress->setProgress(0);
    _task_progress->setPos((_display.width() - _task_progress->getWidth()) / 2, _display.height() / 2 + 2);
}

void FilesScreen::showRemoving()
{
    _mode = MODE_REMOVING;

    WidgetCreator creator{_display};
    getLayout()->deleteWidgets();
    getLayout()->addWidget(creator.getNavbar(ID_NAVBAR, "", "", STR_CANCEL));

    _task_lbl = new Label(ID_UPD_LBL, _display);
    getLayout()->addWidget(_task_lbl);
    _task_lbl->setText(STR_REMOVING);
    _task_lbl->setTextSize(2);
    _task_lbl->setAlign(IWidget::ALIGN_CENTER);
    _task_lbl->setGravity(IWidget::GRAVITY_CENTER);
    _task_lbl->setBackColor(COLOR_MAIN_BACK);
    _task_lbl->setWidth(_display.width());
    _task_lbl->setHeight(_display.height() - NAVBAR_HEIGHT);
}

std::vector<MenuItem *> FilesScreen::getMenuFilesItems(const char *path, uint16_t file_pos, uint8_t size)
{
    String db_path = path;
    db_path += "/";
    db_path += FILES_DB_NAME;

    std::vector<String> files = _fm.readFilesFromDB(db_path.c_str(), file_pos, size);

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
            dir_img->setTransparentColor(Image::TRANSPARENT_COLOR);
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

void FilesScreen::showFiles()
{
    if (!_fm.hasConnection())
    {
        showSDErr();
        return;
    }

    _mode = MODE_NAVIGATION;

    WidgetCreator creator{_display};
    //
    EmptyLayout *layout = creator.getEmptyLayout();
    setLayout(layout);

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

void FilesScreen::update()
{
    if (_input.isPressed(KeyID::KEY_OK))
    {
        _input.lock(KeyID::KEY_OK, 1500);
        if (_mode == MODE_NAVIGATION)
            showContextMenu();
        else if (_mode == MODE_NEW_DIR_DIALOG || _mode == MODE_RENAME_DIALOG)
            saveDialogResult();
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
        _input.lock(KeyID::KEY_UP, 130);
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
        _input.lock(KeyID::KEY_DOWN, 130);
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
        _input.lock(KeyID::KEY_RIGHT, 130);
        if (_mode == MODE_NEW_DIR_DIALOG || _mode == MODE_RENAME_DIALOG)
            _keyboard->focusRight();
    }
    else if (_input.isHolded(KeyID::KEY_LEFT))
    {
        _input.lock(KeyID::KEY_LEFT, 130);
        if (_mode == MODE_NEW_DIR_DIALOG || _mode == MODE_RENAME_DIALOG)
            _keyboard->focusLeft();
    }
    else if (_input.isReleased(KeyID::KEY_OK))
    {
        _input.lock(KeyID::KEY_OK, 200);
        ok();
    }
    else if (_input.isReleased(KeyID::KEY_BACK))
    {
        _input.lock(KeyID::KEY_BACK, 300);
        back();
    }

    if (_mode == MODE_UPDATING)
    {
        if (_fm.isTaskDone())
        {
            showFiles();
            loadDir(false);
        }
        else if ((millis() - _upd_inf_time) > UPD_TRACK_INF_INTERVAL)
        {
            String upd_txt = STR_UPDATING;

            if (_upd_counter > 2)
                _upd_counter = 0;
            else
                ++_upd_counter;

            for (uint8_t i{0}; i < _upd_counter; ++i)
                upd_txt += ".";

            _task_lbl->setText(upd_txt);
            _upd_inf_time = millis();
        }
    }
    else if (_mode == MODE_COPYING)
    {
        if (_fm.isTaskDone())
        {
            showFiles();
            loadDir(true);
        }
        else if ((millis() - _upd_inf_time) > UPD_TRACK_INF_INTERVAL)
        {
            _task_lbl->setText(STR_COPYING);

            _task_progress->setProgress(_fm.getCopyProgress());
            _upd_inf_time = millis();
        }
    }
    else if (_mode == MODE_REMOVING)
    {
        if (_fm.isTaskDone())
        {
            showFiles();
            loadDir(true);
        }
        else if ((millis() - _upd_inf_time) > UPD_TRACK_INF_INTERVAL)
        {
            String upd_txt = STR_REMOVING;

            if (_upd_counter > 2)
                _upd_counter = 0;
            else
                ++_upd_counter;

            for (uint8_t i{0}; i < _upd_counter; ++i)
                upd_txt += ".";

            _task_lbl->setText(upd_txt);
            _upd_inf_time = millis();
        }
    }
}

String FilesScreen::makePathFromBreadcrumbs()
{
    String ret_str{""};

    if (_breadcrumbs.size() > 0)
    {
        for (uint8_t i{0}; i < _breadcrumbs.size(); ++i)
        {
            ret_str += "/";
            ret_str += _breadcrumbs[i];
        }
    }

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

void FilesScreen::saveDialogResult()
{
    if (_mode == MODE_NEW_DIR_DIALOG)
    {
        String dir_path = makePathFromBreadcrumbs();
        dir_path += "/";
        dir_path += _dialog_txt->getText();

        _dialog_success_res = _fm.createDir(dir_path.c_str());
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

        _dialog_success_res = _fm.rename(old_name.c_str(), new_name.c_str());
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

void FilesScreen::showDialog(Mode mode)
{
    _mode = mode;

    WidgetCreator creator{_display};

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

    if (_mode == MODE_RENAME_DIALOG)
    {
        _old_name = _files_list->getCurrentItemText();
        _dialog_txt->setText(_old_name);
    }

    _keyboard = new Keyboard(ID_KEYBOARD, _display);
    _keyboard->setWidth(_display.width());
    _keyboard->setHeight(_display.height() - 50 - NAVBAR_HEIGHT);
    _keyboard->setPos(0, 50);
    _keyboard->setCornerRadius(5);

    KeyboardRow *row1 = new KeyboardRow(1, _display); // Цифри
    _keyboard->addWidget(row1);
    row1->setHeight(40);
    row1->setBtnHeight(25);
    row1->setBtnWidth(18);
    //
    KeyboardRow *row2 = row1->clone(2);
    _keyboard->addWidget(row2);
    //
    KeyboardRow *row3 = row1->clone(3);
    _keyboard->addWidget(row3);
    row3->setBtnWidth(20);
    //
    KeyboardRow *row4 = row1->clone(4);
    _keyboard->addWidget(row4);
    row4->setBtnWidth(25);
    //
    KeyboardRow *row5 = row1->clone(5); // Символи
    _keyboard->addWidget(row5);
    row5->setBtnWidth(40);

    Label *q = new Label(ID_BTN_Q, _display);
    q->setText("q");
    q->setFontID(2);
    q->setGravity(IWidget::GRAVITY_CENTER);
    q->setAlign(IWidget::ALIGN_CENTER);
    q->setFocusBorderColor(COLOR_LIME);
    q->setChangingBorder(true);
    q->setFocusBackColor(COLOR_FOCUS_BACK);
    q->setChangingBack(true);
    q->setTextColor(TFT_WHITE);
    q->setBackColor(COLOR_MAIN_BACK);
    q->setCornerRadius(2);
    //
    Label *w = q->clone(ID_BTN_W);
    w->setText("w");

    Label *e = q->clone(ID_BTN_E);
    e->setText("e");

    Label *r = q->clone(ID_BTN_R);
    r->setText("r");

    Label *t = q->clone(ID_BTN_T);
    t->setText("t");

    Label *y = q->clone(ID_BTN_Y);
    y->setText("y");

    Label *u = q->clone(ID_BTN_U);
    u->setText("u");

    Label *i = q->clone(ID_BTN_I);
    i->setText("i");

    Label *o = q->clone(ID_BTN_O);
    o->setText("o");

    Label *p = q->clone(ID_BTN_P);
    p->setText("p");

    Label *a = q->clone(ID_BTN_A);
    a->setText("a");

    Label *s = q->clone(ID_BTN_S);
    s->setText("s");

    Label *d = q->clone(ID_BTN_D);
    d->setText("d");

    Label *f = q->clone(ID_BTN_F);
    f->setText("f");

    Label *g = q->clone(ID_BTN_G);
    g->setText("g");

    Label *h = q->clone(ID_BTN_H);
    h->setText("h");

    Label *j = q->clone(ID_BTN_J);
    j->setText("j");

    Label *k = q->clone(ID_BTN_K);
    k->setText("k");

    Label *l = q->clone(ID_BTN_L);
    l->setText("l");

    Label *z = q->clone(ID_BTN_Z);
    z->setText("z");

    Label *x = q->clone(ID_BTN_X);
    x->setText("x");

    Label *c = q->clone(ID_BTN_C);
    c->setText("c");

    Label *v = q->clone(ID_BTN_V);
    v->setText("v");

    Label *b = q->clone(ID_BTN_B);
    b->setText("b");

    Label *n = q->clone(ID_BTN_N);
    n->setText("n");

    Label *m = q->clone(ID_BTN_M);
    m->setText("m");

    // -----

    Label *l0 = q->clone(ID_BTN_0);
    l0->setText("0");

    Label *l1 = q->clone(ID_BTN_1);
    l1->setText("1");

    Label *l2 = q->clone(ID_BTN_2);
    l2->setText("2");

    Label *l3 = q->clone(ID_BTN_3);
    l3->setText("3");

    Label *l4 = q->clone(ID_BTN_4);
    l4->setText("4");

    Label *l5 = q->clone(ID_BTN_5);
    l5->setText("5");

    Label *l6 = q->clone(ID_BTN_6);
    l6->setText("6");

    Label *l7 = q->clone(ID_BTN_7);
    l7->setText("7");

    Label *l8 = q->clone(ID_BTN_8);
    l8->setText("8");

    Label *l9 = q->clone(ID_BTN_9);
    l9->setText("9");

    //

    Label *l_dot = q->clone(ID_BTN_DOT);
    l_dot->setText(".");

    Label *l_under = q->clone(ID_BTN_UNDERLINE);
    l_under->setText("_");

    Label *l_hyp = q->clone(ID_BTN_HYPHEN);
    l_hyp->setText("-");

    //
    row1->addWidget(l0);
    row1->addWidget(l1);
    row1->addWidget(l2);
    row1->addWidget(l3);
    row1->addWidget(l4);
    row1->addWidget(l5);
    row1->addWidget(l6);
    row1->addWidget(l7);
    row1->addWidget(l8);
    row1->addWidget(l9);
    //
    row2->addWidget(q);
    row2->addWidget(w);
    row2->addWidget(e);
    row2->addWidget(r);
    row2->addWidget(t);
    row2->addWidget(y);
    row2->addWidget(u);
    row2->addWidget(i);
    row2->addWidget(o);
    row2->addWidget(p);
    //
    row3->addWidget(a);
    row3->addWidget(s);
    row3->addWidget(d);
    row3->addWidget(f);
    row3->addWidget(g);
    row3->addWidget(h);
    row3->addWidget(j);
    row3->addWidget(k);
    row3->addWidget(l);
    //
    row4->addWidget(z);
    row4->addWidget(x);
    row4->addWidget(c);
    row4->addWidget(v);
    row4->addWidget(b);
    row4->addWidget(n);
    row4->addWidget(m);
    //
    row5->addWidget(l_dot);
    row5->addWidget(l_under);
    row5->addWidget(l_hyp);

    getLayout()->deleteWidgets();
    getLayout()->setBackColor(TFT_BLACK);
    getLayout()->addWidget(creator.getNavbar(ID_NAVBAR, STR_SELECT, "", STR_ERASE));
    getLayout()->addWidget(_dialog_txt);
    getLayout()->addWidget(_keyboard);
}

void FilesScreen::hideDialog()
{
    _mode = MODE_NAVIGATION;

    showFiles();

    if (_dialog_success_res)
    {
        _dialog_success_res = false;
        String db_path = makePathFromBreadcrumbs();
        db_path += "/";
        db_path += FILES_DB_NAME;
        SD.remove(db_path.c_str());
    }
    loadDir(true);
}

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

    if (!_fm.fileExist(temp.c_str()))
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
        if (!_fm.fileExist(old_file_path.c_str()) && !_fm.dirExist(old_file_path.c_str()))
            return;

        if (SD.rename(old_file_path.c_str(), new_file_path.c_str()))
        {
            String db_path = _path_from;
            db_path += "/";
            db_path += FILES_DB_NAME;
            SD.remove(db_path.c_str());

            db_path = cur_dir;
            db_path += "/";
            db_path += FILES_DB_NAME;
            SD.remove(db_path.c_str());

            loadDir(true);
        }
    }
    else if (_has_copying_file)
    {
        if (!_fm.fileExist(old_file_path.c_str()))
            return;

        if (_fm.copyFile(old_file_path.c_str(), new_file_path.c_str()))
        {
            String db_path = cur_dir;
            db_path += "/";
            db_path += FILES_DB_NAME;
            SD.remove(db_path.c_str());

            showCopying();
        }
    }

    _has_moving_file = false;
    _has_copying_file = false;

    _path_from = "";
    _name_from = "";
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
        {
            String path = makePathFromBreadcrumbs();
            String file_name = path;
            file_name += "/";
            file_name += _files_list->getCurrentItemText();

            if (_fm.remove(file_name.c_str()))
            {
                String db_path = path;
                db_path += "/";
                db_path += FILES_DB_NAME;
                SD.remove(db_path.c_str());

                showRemoving();
            }
        }
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
    }
    else if (_mode == MODE_NEW_DIR_DIALOG || _mode == MODE_RENAME_DIALOG)
        keyboardClickHandler();
}

void FilesScreen::back()
{
    if (_mode == MODE_UPDATING || _mode == MODE_COPYING || _mode == MODE_REMOVING)
        _fm.stopTasks();
    else if (_mode == MODE_CONTEXT_MENU)
        hideContextMenu();
    else if (_mode == MODE_NAVIGATION)
        openPrevlevel();
    else if (_mode == MODE_NEW_DIR_DIALOG || _mode == MODE_RENAME_DIALOG)
        _dialog_txt->removeLastChar();
}

void FilesScreen::openNextLevel()
{
    String next_dir = makePathFromBreadcrumbs();
    next_dir += "/";
    next_dir += _files_list->getCurrentItemText();

    File dir = SD.open(next_dir);

    if (!dir)
    {
        log_e("Відсутній каталог: %s", next_dir);
        return;
    }

    if (!dir.isDirectory())
    {
        dir.close();
        return;
    }

    _breadcrumbs.push_back(_files_list->getCurrentItemText());
    loadDir(true);
}

void FilesScreen::openPrevlevel()
{
    if (_breadcrumbs.size() > 0)
    {
        _breadcrumbs.pop_back();
        loadDir(true);
    }
}

void FilesScreen::loadDir(bool need_update)
{
    String path = makePathFromBreadcrumbs();
    String db_path = path;
    db_path += "/";
    db_path += FILES_DB_NAME;

    if (!SD.exists(db_path))
        if (need_update)
        {
            updateDir(path.c_str());
            return;
        }
        else
            return;

    _files_list->deleteWidgets();

    uint16_t db_sz = _fm.getDBSize(db_path.c_str());
    _scrollbar->setValue(0);
    _scrollbar->setMax(db_sz);

    std::vector<MenuItem *> items = getMenuFilesItems(path.c_str(), 0, _files_list->getItemsNumOnScreen());
    for (size_t i = 0; i < items.size(); ++i)
        _files_list->addItem(items[i]);
}

void FilesScreen::updateDir(const char *dir_path)
{
    String db_path = dir_path;
    db_path += "/";
    db_path += FILES_DB_NAME;
    uint16_t db_sz = _fm.getDBSize(db_path.c_str());

    String dir = dir_path;
    if (dir.isEmpty())
        dir = "/";

    if (_fm.indexAll(dir.c_str(), db_path.c_str()))
        showDirUpdating();
    else
        showSDErr();
}