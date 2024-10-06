#pragma once
#pragma GCC optimize("Ofast")

#include <Arduino.h>
#include "IWidgetContainer.h"

namespace meow
{

  class EmptyLayout : public IWidgetContainer
  {
  public:
    EmptyLayout(uint16_t widget_ID, GraphicsDriver &display);
    virtual ~EmptyLayout();
    virtual void onDraw() override;
    EmptyLayout *clone(uint16_t id) const override;
  };

}