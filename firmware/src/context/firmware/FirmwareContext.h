#pragma once
#include <Arduino.h>
#include "meow/ui/context/IContext.h"
#include "meow/manager/files/FileManager.h"

using namespace meow;

class FirmwareContext : public IContext
{
public:
    FirmwareContext(GraphicsDriver &display);
    virtual ~FirmwareContext(){}

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