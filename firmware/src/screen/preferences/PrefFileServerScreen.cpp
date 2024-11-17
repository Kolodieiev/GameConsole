#include "PrefFileServerScreen.h"

#include "meow/manager/settings/SettingsManager.h"

#include "../WidgetCreator.h"

#include "../resources/string.h"
#include "../resources/color.h"
#include "../resources/const.h"

PrefFileServerScreen::PrefFileServerScreen(GraphicsDriver &display) : IScreen(display)
{
    SettingsManager settings;

    String ssid_str = settings.get(STR_PREF_FS_AP_SSID);
    String pwd_str = settings.get(STR_PREF_FS_AP_PWD);

    WidgetCreator creator{_display};

    EmptyLayout *layout = creator.getEmptyLayout();
    setLayout(layout);

    NavBar *navbar = creator.getNavbar(ID_NAVBAR, STR_SAVE, "", STR_BACK);
    layout->addWidget(navbar);

    
}

void PrefFileServerScreen::loop()
{
}

void PrefFileServerScreen::update()
{
}
