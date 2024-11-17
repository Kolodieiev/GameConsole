#include "FirmwareScreen.h"
#include "Update.h"

#include "../WidgetCreator.h"

#include "../resources/string.h"
#include "../resources/color.h"
#include "../resources/const.h"

const char STR_FIRMWARE_FN[] = "/firmware.bin";

FirmwareScreen::FirmwareScreen(GraphicsDriver &display) : IScreen(display)
{
    WidgetCreator creator{_display};

    EmptyLayout *layout = creator.getEmptyLayout();
    setLayout(layout);

    if (_file_mngr.fileExist(STR_FIRMWARE_FN))
        layout->addWidget(creator.getNavbar(ID_NAVBAR, STR_UPDATE, "", STR_BACK));
    else
        layout->addWidget(creator.getNavbar(ID_NAVBAR, "", "", STR_BACK));

    Label *title = new Label(ID_TITLE, _display);
    layout->addWidget(title);
    title->setText(STR_UPDATE_FIRMWARE);
    title->setWidth(_display.width());
    title->setHeight(30);
    title->setGravity(IWidget::GRAVITY_CENTER);
    title->setAlign(IWidget::ALIGN_CENTER);
    title->setBackColor(TFT_BLACK);
    title->setTextSize(2);
    title->setFontID(4);

    Label *author = title->clone(ID_AUTHOR);
    layout->addWidget(author);
    author->setText(STR_AUTHOR);
    author->setPos(0, (_display.height() - NAVBAR_HEIGHT - 55) / 2);
    author->setBackColor(COLOR_MAIN_BACK);
    author->setTextSize(2);
    author->setFontID(4);

    Label *version = author->clone(ID_VERSION);
    layout->addWidget(version);
    version->setText(STR_FIRM_VERSION);
    version->setPos(0, (_display.height() - NAVBAR_HEIGHT + 10) / 2);
    version->setTextSize(1);
    version->setFontID(2);
}

void FirmwareScreen::loop()
{
}

void FirmwareScreen::update()
{
    if (_input.isReleased(KeyID::KEY_OK))
    {
        _input.lock(KeyID::KEY_OK, 500);

        if (_file_mngr.fileExist(STR_FIRMWARE_FN))
        {
            File firmware = SD.open(STR_FIRMWARE_FN, FILE_READ);

            showUpdating();

            delay(200);

            Update.begin(firmware.size(), U_FLASH);
            Update.writeStream(firmware);
            Update.end();

            firmware.close();
            ESP.restart();
        }
    }
    else if (_input.isReleased(KeyID::KEY_BACK))
    {
        _input.lock(KeyID::KEY_BACK, 500);
        openScreenByID(ID_SCREEN_MENU);
    }
}

void FirmwareScreen::showUpdating()
{
    WidgetCreator creator{_display};

    EmptyLayout *layout = creator.getEmptyLayout();
    setLayout(layout);

    Label *updating_msg = new Label(1, _display);
    layout->addWidget(updating_msg);
    updating_msg->setText(STR_FIRMWARE_UPDATING);
    updating_msg->setWidth(_display.width());
    updating_msg->setHeight(_display.height());
    updating_msg->setGravity(IWidget::GRAVITY_CENTER);
    updating_msg->setAlign(IWidget::ALIGN_CENTER);
    updating_msg->setBackColor(COLOR_MAIN_BACK);
    layout->onDraw();
}