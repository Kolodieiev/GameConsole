#pragma once
#include <Arduino.h>

#include "meow/ui/screen/IScreen.h"

using namespace meow;

class TorchScreen : public IScreen
{
public:
    TorchScreen(GraphicsDriver &display);
    virtual ~TorchScreen() {}

protected:
    virtual void loop() override;
    virtual void update() override;

private:
    uint8_t _torch_brightness{50};
    uint8_t _display_brightness;
    bool _is_light_on{true};
};