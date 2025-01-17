#pragma once
#pragma GCC optimize("O3")
#include <Arduino.h>
#include "meow/ui/context/IContext.h"

#include "meow/ui/widget/text/TextBox.h"
#include "meow/ui/widget/keyboard/Keyboard.h"

using namespace meow;

class PrefFileServerContext : public IContext
{
public:
    PrefFileServerContext(GraphicsDriver &display);
    virtual ~PrefFileServerContext() {}

protected:
    virtual void loop() override;
    virtual void update() override;

private:
    const uint16_t KEY_LOCK_DUR = 250;

    enum Widget_ID : uint8_t
    {
        ID_NAVBAR = 2,
        ID_LBL_SSID,
        ID_LBL_PWD,
        ID_TXT_SSID,
        ID_TXT_PWD,
        ID_KEYBOARD,
        ID_TXT_DIALOG
    };

    enum Mode : uint8_t
    {
        MODE_MAIN,
        MODE_DIALOG
    };

    Label *_ssid_lbl;
    TextBox *_ssid_txt;
    String _ssid_str;

    Label *_pwd_lbl;
    TextBox *_pwd_txt;
    String _pwd_str;

    Keyboard *_keyboard;

    TextBox *_dialog_txt;

    bool _is_ssid_edit = true;

    Mode _mode = MODE_MAIN;

    void showMainTmpl();
    void showDialogTmpl();
    void hideDialog();
    void saveSettings();

    void up();
    void down();
    void left();
    void right();

    void ok();
    void okPressed();

    void back();
    void backPressed();

    void changeTbFocus();

    void keyboardClickHandler();
};