#pragma once
#include <Arduino.h>

#include "meow/ui/context/IContext.h"

using namespace meow;

class TorchContext : public IContext
{
public:
    TorchContext(GraphicsDriver &display);
    virtual ~TorchContext() {}

protected:
    virtual void loop() override;
    virtual void update() override;

private:
    uint8_t _torch_brightness{50};
    uint8_t _display_brightness;
    bool _is_light_on{true};
};