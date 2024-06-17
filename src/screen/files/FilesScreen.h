#pragma once
#include <Arduino.h>

#include "meow/util/files/FileManager.h"

#include "meow/ui/widget/scrollbar/ScrollBar.h"
#include "meow/ui/widget/menu/FixedMenu.h"
#include "meow/ui/widget/menu/DynamicMenu.h"
#include "meow/ui/widget/keyboard/Keyboard.h"
#include "meow/ui/widget/progress/ProgressBar.h"
#include "meow/ui/widget/image/Image.h"
#include "meow/ui/widget/text/TextBox.h"

#include "meow/ui/screen/IScreen.h"

using namespace meow;

class FilesScreen : public IScreen, public IItemsLoader
{
public:
    FilesScreen(GraphicsDriver &display);
    virtual ~FilesScreen();

    virtual std::vector<MenuItem *> loadPrev(uint8_t size, uint16_t current_ID) override;
    virtual std::vector<MenuItem *> loadNext(uint8_t size, uint16_t current_ID) override;

protected:
    virtual void loop() override;
    virtual void update() override;

private:
    const uint8_t MENU_ITEMS_NUM{10};

    enum Widget_ID : uint8_t
    {
        ID_NAVBAR = 1,
        ID_SCROLLBAR,
        ID_DYNAMIC_MENU,
        ID_CONTEXT_MENU,
        ID_KEYBOARD,
        ID_DIALOG_TXT,
        ID_UPD_LBL,
        ID_ERR_LBL,
        ID_PROGRESS,
    };

    enum Item_ID : uint8_t
    {
        ID_ITEM_UPDATE = 1,
        ID_ITEM_PASTE,
        ID_ITEM_MOVE,
        ID_ITEM_COPY,
        ID_ITEM_REMOVE,
        ID_ITEM_NEW_DIR,
        ID_ITEM_RENAME
    };

    enum BTN_ID : uint8_t
    {
        ID_BTN_Q = 1,
        ID_BTN_W,
        ID_BTN_E,
        ID_BTN_R,
        ID_BTN_T,
        ID_BTN_Y,
        ID_BTN_U,
        ID_BTN_I,
        ID_BTN_O,
        ID_BTN_P,
        ID_BTN_A,
        ID_BTN_S,
        ID_BTN_D,
        ID_BTN_F,
        ID_BTN_G,
        ID_BTN_H,
        ID_BTN_J,
        ID_BTN_K,
        ID_BTN_L,
        ID_BTN_Z,
        ID_BTN_X,
        ID_BTN_C,
        ID_BTN_V,
        ID_BTN_B,
        ID_BTN_N,
        ID_BTN_M,
        //
        ID_BTN_0,
        ID_BTN_1,
        ID_BTN_2,
        ID_BTN_3,
        ID_BTN_4,
        ID_BTN_5,
        ID_BTN_6,
        ID_BTN_7,
        ID_BTN_8,
        ID_BTN_9,
        //
        ID_BTN_DOT,
        ID_BTN_UNDERLINE,
        ID_BTN_HYPHEN,
    };

    enum Mode : uint8_t
    {
        MODE_NAVIGATION = 0,
        MODE_COPYING,
        MODE_REMOVING,
        MODE_MOVING,
        MODE_UPDATING,
        MODE_CONTEXT_MENU,
        MODE_NEW_DIR_DIALOG,
        MODE_RENAME_DIALOG,
        MODE_BAD_CONNECT
    };
    //
    FileManager _fm;
    //
    Label *_task_lbl;
    const uint16_t UPD_TRACK_INF_INTERVAL{1000};
    unsigned long _upd_inf_time{0};
    uint8_t _upd_counter{0};
    //
    Mode _mode{MODE_NAVIGATION};
    //
    FixedMenu *_context_menu;
    ScrollBar *_scrollbar;
    ProgressBar *_task_progress;
    DynamicMenu *_files_list;
    //
    Keyboard *_keyboard;
    TextBox *_dialog_txt;
    bool _dialog_success_res{false};
    String _old_name;
    //
    bool _is_dir{false};
    bool _has_moving_file{false};
    bool _has_copying_file{false};
    String _path_from;
    String _name_from;
    //
    std::vector<String> _breadcrumbs;
    String makePathFromBreadcrumbs();
    //
    void showFiles();
    void showDirUpdating();
    void showCopying();
    void showRemoving();
    void showSDErr();
    //
    void showContextMenu();
    void hideContextMenu();
    //
    void keyboardClickHandler();
    void showDialog(Mode mode);
    void hideDialog();
    //
    void prepareFileMoving();
    void prepareFileCopying();
    void pasteFile();
    //
    void ok();
    void back();
    //
    void openNextLevel();
    void openPrevlevel();
    //
    void loadDir(bool need_update);
    void updateDir(const char *dir_path);

    void saveDialogResult();

    std::vector<MenuItem *> getMenuFilesItems(const char *path, uint16_t file_pos, uint8_t size);
};