#include "FirmwareContext.h"
#include "Update.h"

#include "../WidgetCreator.h"

#include "../resources/string.h"
#include "../resources/color.h"
#include "../resources/const.h"

const char STR_FIRMWARE_FN[] = "/firmware.bin";

FirmwareContext::FirmwareContext(GraphicsDriver &display) : IContext(display)
{
    WidgetCreator creator{_display};

    EmptyLayout *layout = creator.getEmptyLayout();
    setLayout(layout);

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
    author->setPos(0, getCenterY(author));
    author->setBackColor(COLOR_MAIN_BACK);
    author->setTextSize(2);
    author->setFontID(4);

    Label *version = author->clone(ID_VERSION);
    layout->addWidget(version);
    version->setText(STR_FIRM_VERSION);
    version->setPos(0, author->getYPos() + author->getHeight() + 10);
    version->setTextSize(1);
    version->setFontID(2);
}

void FirmwareContext::loop()
{
}

void FirmwareContext::update()
{
    if (_input.isReleased(KeyID::KEY_OK))
    {
        _input.lock(KeyID::KEY_OK, 500);

        if (_file_mngr.fileExist(STR_FIRMWARE_FN))
        {
            size_t firm_size = _file_mngr.getFileSize(STR_FIRMWARE_FN);
            FILE *firm_ptr = _file_mngr.getFileDescriptor(STR_FIRMWARE_FN, "rb");

            FileStream f_stream(firm_ptr, STR_FIRMWARE_FN, firm_size);

            showUpdating();

            delay(200);

            Update.begin(firm_size, U_FLASH);
            Update.writeStream(f_stream);
            Update.end();

            _file_mngr.closeFile(firm_ptr);
            ESP.restart();
        }
    }
    else if (_input.isReleased(KeyID::KEY_BACK))
    {
        _input.lock(KeyID::KEY_BACK, 500);
        openContextByID(ID_CONTEXT_MENU);
    }
}

void FirmwareContext::showUpdating()
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