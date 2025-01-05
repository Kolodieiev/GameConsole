#include "PrefFileServerScreen.h"
#pragma GCC optimize("O3")

#include "meow/manager/settings/SettingsManager.h"

#include "../WidgetCreator.h"

#include "../resources/string.h"
#include "../resources/color.h"
#include "../resources/const.h"

void PrefFileServerScreen::loop()
{
}

PrefFileServerScreen::PrefFileServerScreen(GraphicsDriver &display) : IScreen(display)
{
    SettingsManager settings;
    _ssid_str = settings.get(STR_PREF_FS_AP_SSID);
    if (_ssid_str.isEmpty())
        _ssid_str = STR_DEF_SSID;

    _pwd_str = settings.get(STR_PREF_FS_AP_PWD);
    if (_pwd_str.isEmpty())
        _pwd_str = STR_DEF_PWD;

    showMainTmpl();
}

void PrefFileServerScreen::showMainTmpl()
{
    WidgetCreator creator{_display};
    EmptyLayout *layout = creator.getEmptyLayout();
    setLayout(layout);

    //----------------------------------------------------

    const uint8_t LEFT_PADDING = 5;
    const uint8_t TOP_PADDING = 5;
    const uint8_t SPACE_BETWEEN = 40;

    _ssid_lbl = new Label(ID_LBL_SSID, _display);
    layout->addWidget(_ssid_lbl);
    _ssid_lbl->setText(STR_AP_NAME);
    _ssid_lbl->setPos(LEFT_PADDING, TOP_PADDING);
    _ssid_lbl->setBackColor(COLOR_MAIN_BACK);
    _ssid_lbl->initWidthToFit();

    _ssid_txt = new TextBox(ID_TXT_SSID, _display);
    layout->addWidget(_ssid_txt);
    _ssid_txt->setText(_ssid_str);
    _ssid_txt->setTextOffset(5);
    _ssid_txt->setFocusBorderColor(TFT_ORANGE);
    _ssid_txt->setChangingBorder(true);
    _ssid_txt->setTextSize(2);
    _ssid_txt->setWidth(D_WIDTH - LEFT_PADDING * 2);
    _ssid_txt->setPos(LEFT_PADDING, _ssid_lbl->getYPos() + _ssid_lbl->getHeight() + 1);

    //----------------------------------------------------

    _pwd_lbl = _ssid_lbl->clone(ID_LBL_PWD);
    layout->addWidget(_pwd_lbl);
    _pwd_lbl->setText(STR_AP_PWD);
    _pwd_lbl->initWidthToFit();
    _pwd_lbl->setPos(LEFT_PADDING, _ssid_txt->getYPos() + _ssid_txt->getHeight() + SPACE_BETWEEN);

    _pwd_txt = _ssid_txt->clone(ID_TXT_PWD);
    layout->addWidget(_pwd_txt);
    _pwd_txt->setType(TextBox::TYPE_PASSWORD);
    _pwd_txt->setText(_pwd_str);
    _pwd_txt->setPos(LEFT_PADDING, _pwd_lbl->getYPos() + _pwd_lbl->getHeight() + 1);

    if (_is_ssid_edit)
    {
        _ssid_lbl->setTextColor(TFT_ORANGE);
        _pwd_lbl->setTextColor(TFT_WHITE);

        _ssid_txt->setFocus();
        _pwd_txt->removeFocus();
    }
    else
    {
        _ssid_lbl->setTextColor(TFT_WHITE);
        _pwd_lbl->setTextColor(TFT_ORANGE);

        _ssid_txt->removeFocus();
        _pwd_txt->setFocus();
    }

    _mode = MODE_MAIN;
}

void PrefFileServerScreen::showDialogTmpl()
{
    IWidgetContainer *layout = getLayout();
    layout->disable();
    layout->deleteWidgets();
    layout->setBackColor(TFT_BLACK);

    WidgetCreator creator{_display};

    _dialog_txt = new TextBox(ID_TXT_DIALOG, _display);
    layout->addWidget(_dialog_txt);
    _dialog_txt->setTextOffset(5);
    _dialog_txt->setWidth(_display.width() - 10);
    _dialog_txt->setHeight(40);
    _dialog_txt->setBackColor(TFT_WHITE);
    _dialog_txt->setTextColor(TFT_BLACK);
    _dialog_txt->setFontID(2);
    _dialog_txt->setTextSize(2);
    _dialog_txt->setPos(5, 5);
    _dialog_txt->setCornerRadius(3);

    if (_is_ssid_edit)
        _dialog_txt->setText(_ssid_str);
    else
        _dialog_txt->setText(_pwd_str);

    _keyboard = creator.getStandardEnKeyboard(ID_KEYBOARD);
    layout->addWidget(_keyboard);

    _mode = MODE_DIALOG;

    layout->enable();
}

void PrefFileServerScreen::hideDialog()
{
    String dialog_res_str = _dialog_txt->getText();

    if (_is_ssid_edit)
        _ssid_str = dialog_res_str.isEmpty() ? STR_DEF_SSID : dialog_res_str;
    else
        _pwd_str = dialog_res_str.isEmpty() ? STR_DEF_PWD : dialog_res_str;

    showMainTmpl();
}

void PrefFileServerScreen::saveSettings()
{
    SettingsManager settings;
    settings.set(STR_PREF_FS_AP_SSID, _ssid_str.c_str());
    settings.set(STR_PREF_FS_AP_PWD, _pwd_str.c_str());
}

void PrefFileServerScreen::up()
{
    if (_mode == MODE_MAIN)
        changeTbFocus();
    else
        _keyboard->focusUp();
}

void PrefFileServerScreen::down()
{
    if (_mode == MODE_MAIN)
        changeTbFocus();
    else
        _keyboard->focusDown();
}

void PrefFileServerScreen::left()
{
    if (_mode == MODE_DIALOG)
        _keyboard->focusLeft();
}

void PrefFileServerScreen::right()
{
    if (_mode == MODE_DIALOG)
        _keyboard->focusRight();
}

void PrefFileServerScreen::okPressed()
{
    if (_mode == MODE_MAIN)
    {
        saveSettings();
        openScreenByID(ID_SCREEN_PREF_SEL);
    }
    else if (_mode == MODE_DIALOG)
        hideDialog();
}

void PrefFileServerScreen::ok()
{
    if (_mode == MODE_MAIN)
        showDialogTmpl();
    else if (_mode == MODE_DIALOG)
        keyboardClickHandler();
}

void PrefFileServerScreen::keyboardClickHandler()
{
    _dialog_txt->addChars(_keyboard->getCurrentBtnTxt().c_str());
}

void PrefFileServerScreen::back()
{
    if (_mode == MODE_MAIN)
        openScreenByID(ID_SCREEN_PREF_SEL);
    else if (_mode == MODE_DIALOG)
        _dialog_txt->removeLastChar();
}

void PrefFileServerScreen::backPressed()
{
    if (_mode == MODE_MAIN)
        openScreenByID(ID_SCREEN_PREF_SEL);
    else
        hideDialog();
}

void PrefFileServerScreen::changeTbFocus()
{
    if (_is_ssid_edit)
    {
        _ssid_lbl->setTextColor(TFT_WHITE);
        _pwd_lbl->setTextColor(TFT_ORANGE);

        _ssid_txt->removeFocus();
        _pwd_txt->setFocus();
    }
    else
    {
        _ssid_lbl->setTextColor(TFT_ORANGE);
        _pwd_lbl->setTextColor(TFT_WHITE);

        _ssid_txt->setFocus();
        _pwd_txt->removeFocus();
    }

    _is_ssid_edit = !_is_ssid_edit;
}

void PrefFileServerScreen::update()
{
    if (_input.isPressed(KeyID::KEY_OK))
    {
        _input.lock(KeyID::KEY_OK, 1500);
        okPressed();
    }
    else if (_input.isPressed(KeyID::KEY_BACK))
    {
        _input.lock(KeyID::KEY_BACK, 1500);
        backPressed();
    }
    else if (_input.isHolded(KeyID::KEY_UP))
    {
        _input.lock(KeyID::KEY_UP, KEY_LOCK_DUR);
        up();
    }
    else if (_input.isHolded(KeyID::KEY_DOWN))
    {
        _input.lock(KeyID::KEY_DOWN, KEY_LOCK_DUR);
        down();
    }
    else if (_input.isHolded(KeyID::KEY_RIGHT))
    {
        _input.lock(KeyID::KEY_RIGHT, KEY_LOCK_DUR);
        right();
    }
    else if (_input.isHolded(KeyID::KEY_LEFT))
    {
        _input.lock(KeyID::KEY_LEFT, KEY_LOCK_DUR);
        left();
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
}
