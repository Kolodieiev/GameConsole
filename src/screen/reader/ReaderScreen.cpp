#include "ReaderScreen.h"

#include "meow/lib/DS3231/DS3231.h"

#include "../WidgetCreator.h"

#include "../resources/string.h"
#include "../resources/color.h"
#include "../resources/const.h"

ReaderScreen::ReaderScreen(GraphicsDriver &display) : IScreen(display)
{
    WidgetCreator creator{_display};

    EmptyLayout *layout = creator.getEmptyLayout();
    setLayout(layout);

    _page = new Label(ID_PAGE, _display);
    layout->addWidget(_page);
    _page->setMultiline(true);
    _page->setWidth(DWIDTH);
    _page->setHeight(DHEIGHT);
    _page->setBackColor(TFT_BLACK);
    _page->setTextColor(TFT_WHITE);

    // SdUtil sd;
    // sd.begin();

    // char b[MAX_CHAR_TO_READ] = "";

    // File f = SD.open("/lorem.txt", FILE_READ);

    // f.read((uint8_t *)b, sizeof(b) - 1);
    // f.close();
    // sd.end();

    // String str = b;

    // log_i("str %s", str);

    // _page->setText(str);
}

ReaderScreen::~ReaderScreen()
{
}

void ReaderScreen::loop()
{
}

void ReaderScreen::update()
{
    if (_input.isPressed(Input::PIN_OK))
    {
        _input.lock(Input::PIN_OK, 1000);
    }
    else if (_input.isPressed(Input::PIN_BACK))
    {
        _input.lock(Input::PIN_BACK, 2500);
    }
    else if (_input.isReleased(Input::PIN_UP))
    {
        _input.lock(Input::PIN_UP, 100);
        up();
    }
    else if (_input.isReleased(Input::PIN_DOWN))
    {
        _input.lock(Input::PIN_DOWN, 100);
        down();
    }
    else if (_input.isReleased(Input::PIN_RIGHT))
    {
        _input.lock(Input::PIN_RIGHT, 100);
        right();
    }
    else if (_input.isReleased(Input::PIN_LEFT))
    {
        _input.lock(Input::PIN_LEFT, 100);
        left();
    }
    else if (_input.isReleased(Input::PIN_OK))
    {
        _input.lock(Input::PIN_OK, 100);
        ok();
    }
    else if (_input.isReleased(Input::PIN_BACK))
    {
        _input.lock(Input::PIN_BACK, 100);
    }
}

void ReaderScreen::ok()
{
}

void ReaderScreen::up()
{
}

void ReaderScreen::down()
{
}

void ReaderScreen::left()
{
}

void ReaderScreen::right()
{
}

void ReaderScreen::back()
{
}

void ReaderScreen::backPressed()
{
}
