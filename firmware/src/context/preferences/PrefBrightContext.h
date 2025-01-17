#pragma once
#include <Arduino.h>

#include "meow/ui/context/IContext.h"
#include "meow/ui/widget/progress/ProgressBar.h"

using namespace meow;

class PrefBrightContext : public IContext
{
public:
    PrefBrightContext(GraphicsDriver &display);
    virtual ~PrefBrightContext() {}

protected:
    virtual void loop() override;
    virtual void update() override;

private:
    enum Widget_ID : uint8_t
    {
        ID_NAVBAR = 2,
        ID_HEADER,
        ID_PROGRESSBAR
    };

    const uint8_t MAX_BRIGHT = 250;
    const uint8_t BRIGHT_STEP = 10;

    uint8_t _old_bright;
    ProgressBar *_progress;
};