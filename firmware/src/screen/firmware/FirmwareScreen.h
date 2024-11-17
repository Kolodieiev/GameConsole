#pragma once
#include <Arduino.h>
#include "meow/ui/screen/IScreen.h"
#include "meow/manager/files/FileManager.h"

using namespace meow;

class FirmwareScreen : public IScreen
{
public:
    FirmwareScreen(GraphicsDriver &display);
    virtual ~FirmwareScreen(){}

protected:
    virtual void loop() override;
    virtual void update() override;

private:
    enum Widget_ID : uint8_t
    {
        ID_NAVBAR = 1,
        ID_TITLE,
        ID_AUTHOR,
        ID_VERSION,
    };

    FileManager _file_mngr;

    void showUpdating();
};