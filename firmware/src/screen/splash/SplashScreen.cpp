#include "SplashScreen.h"

#include "meow/util/preferences/PrefUtil.h"
#include "meow/util/display/DisplayUtil.h"
#include "meow/util/sd/SdUtil.h"

#include "../WidgetCreator.h"
#include "meow/ui/widget/image/Image.h"

#include "../resources/color.h"
#include "../resources/string.h"
#include "./res/paw_src.h"
#include "./res/logo_txt_src.h"

#define RES_X_OFFSET 5
#define LBL_X_OFFSET 70

SplashScreen::SplashScreen(GraphicsDriver &display) : IScreen(display)
{
    _start_time = millis();

    WidgetCreator creator{_display};
    //
    EmptyLayout *layout = creator.getEmptyLayout();
    setLayout(layout);
    //
    uint16_t y_pos{5};

    // check sd
    SdUtil sd;

    if (sd.begin())
    {
        addLabel(RES_X_OFFSET, y_pos, STR_SUCCSESS, TFT_GREEN);

        PrefUtil pref;
        String bright = pref.get(STR_PREF_BRIGHT);

        if (bright.equals(""))
            DisplayUtil::setBrightness(240);
        else
            DisplayUtil::setBrightness(atoi(bright.c_str()));
    }
    else
        addLabel(RES_X_OFFSET, y_pos, STR_FAIL, TFT_RED);

    sd.end();

    addLabel(LBL_X_OFFSET, y_pos, STR_SD_LBL, TFT_WHITE);

    y_pos += chr_hgt_font2 + 3;

    // check psram
    bool psram_ok = psramInit();
    if (psram_ok)
        addLabel(RES_X_OFFSET, y_pos, STR_SUCCSESS, TFT_GREEN);
    else
        addLabel(RES_X_OFFSET, y_pos, STR_FAIL, TFT_RED);

    addLabel(LBL_X_OFFSET, y_pos, STR_PSRAM_LBL, TFT_WHITE);
}

void SplashScreen::loop()
{
}

void SplashScreen::update()
{
    if (!_logo_showed)
        if (millis() - _start_time > SHOWING_INIT_TIME)
        {
            IWidgetContainer *container = getLayout();
            container->deleteWidgets();
            container->setBackColor(TFT_WHITE);

            Image *paw_img = new Image(1, _display);
            container->addWidget(paw_img);
            paw_img->init(99, 110);
            paw_img->setSrc(PAW_SRC);
            paw_img->setPos((_display.width() - 99) * 0.5, 5);

            Image *logo_txt_img = new Image(2, _display);
            container->addWidget(logo_txt_img);
            logo_txt_img->init(235, 72);
            logo_txt_img->setSrc(LOGO_TXT_SRC);
            logo_txt_img->setPos(((_display.width() - 235) * 0.5), _display.height() - 72 - 5);

            _logo_showed = true;
        }

    if (millis() - _start_time > SHOWING_LOGO_TIME)
    {
        openScreenByID(ScreenID::ID_SCREEN_HOME);
    }
}

void SplashScreen::addLabel(uint16_t x_pos, uint16_t y_pos, const char *res_str, uint16_t color)
{
    EmptyLayout *layout = (EmptyLayout *)getLayout();

    Label *lbl = new Label(_widget_id, _display);
    layout->addWidget(lbl);
    lbl->setText(res_str);
    lbl->setBackColor(COLOR_MAIN_BACK);
    lbl->setTextColor(color);
    lbl->setPos(x_pos, y_pos);
    lbl->initWidthToFit();

    ++_widget_id;
}
