#pragma once
#pragma GCC optimize("Ofast")

#include <Arduino.h>

#include "./MenuItem.h"
#include "meow/ui/widget/toggle/ToggleSwitch.h"

namespace meow
{
    class ToggleItem : public MenuItem
    {
    public:
        ToggleItem(uint16_t widget_ID, GraphicsDriver &display);
        virtual ~ToggleItem();

        virtual void onDraw() override;
        ToggleItem *clone(uint16_t id) const override;

        void setToggle(ToggleSwitch *toggle);

        void on();
        void off();
        bool isOn() const;

    private:
        using IWidget::setVisibility;

        ToggleSwitch *_toggle{nullptr};
    };
}