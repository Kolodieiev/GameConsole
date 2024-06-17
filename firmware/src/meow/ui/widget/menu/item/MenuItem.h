#pragma once
#include <Arduino.h>

#include "meow/ui/widget/image/Image.h"
#include "meow/ui/widget/text/Label.h"

namespace meow
{
    class MenuItem : public IWidget
    {
    public:
        MenuItem(uint16_t widget_ID, GraphicsDriver &display);
        virtual ~MenuItem();
        virtual void onDraw() override;
        MenuItem *clone(uint16_t id) const override;

        void setImg(Image *img);
        void setLbl(Label *lbl);
        String getText() const;

    protected:
        const uint8_t ITEM_PADDING{5};
        using IWidget::setVisibility;

        Image *_image{nullptr};
        Label *_label{nullptr};
    };
}