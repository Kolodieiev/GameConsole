#pragma once
#include <Arduino.h>

#include "meow/ui/screen/IScreen.h"

using namespace meow;

//TODO
class PrefFileServerScreen : public IScreen
{
public:
    PrefFileServerScreen(GraphicsDriver &display);
    virtual ~PrefFileServerScreen() {}

protected:
    virtual void loop() override;
    virtual void update() override;

private:
    enum Widget_ID : uint8_t
    {
        ID_NAVBAR = 2,
        ID_LBL_SSID,
        ID_LBL_PWD,
        ID_TXT_SSID,
        ID_TXT_PWD,
    };
};