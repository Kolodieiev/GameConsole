#pragma GCC optimize("O3")
#include "TestServerScreen.h"
#include "meow/manager/settings/SettingsManager.h"
#include "../../WidgetCreator.h"
#include "./SceneID.h"
#include "./scene/MainScene.h"

#include "../../resources/color.h"
#include "../../resources/string.h"
#include "../../resources/const.h"

#include "../common_res/btn_back/btn_back_normal.h"
#include "../common_res/btn_back/btn_back_hover.h"
#include "./res/string.h"

const char STR_SELECT_ROLE_TITLE[] = "Оберіть роль";
const char STR_WAITING_CLIENT[] = "Очікуйте приєднання клієнтів";
const char STR_SERVER_SCANNING[] = "Зачекайте, відбувається сканування";
const char STR_SERVER_SCANNING_DONE[] = "Сканування завершено";
const char STR_SELECT_SERVER[] = "Оберіть сервер";
const char STR_CLIENT[] = "Клієнт";
const char STR_SERVER[] = "Сервер";
const char STR_ENTER_PWD[] = "Введіть пароль до: ";
const char STR_CONNECTING_TO[] = "Очікуємо підключення до: ";

const char STR_SERVER_UNAVAILABLE[] = "Сервер не відповідає";
const char STR_CONNECTING_ERROR[] = "Помилка підключення";
const char STR_CONNECTING[] = "Підключення до сервера";

const char STR_WANTS_TO_JOIN[] = " хоче приєднатися";

const char STR_WAITING_GAME[] = "Очікуйте запуск гри";
const char STR_DISCONNECTED[] = "Від'єднано від сервера";
//
const char STR_TEST_GAME_NICK[] = "test_game_nick";
const char STR_TEST_GAME_C_PWD[] = "test_game_c_pwd";
const char STR_TEST_GAME_S_PWD[] = "test_game_s_pwd";
const char STR_TEST_GAME_S_NAME[] = "test_game_s_name";
const char STR_NICKNAME[] = "Нікнейм";
const char STR_ENTER_NICK[] = "Введіть нікнейм";
const char STR_SERV_NAME[] = "Назва сервера";
const char STR_ENTER_SERV_NAME[] = "Введіть назву сервера";
const char STR_SERV_PWD[] = "Пароль від сервера";
const char STR_ENTER_SERV_PWD[] = "Введіть пароль від сервера";
//
const char STR_CONT_DISC_CLIENT[] = "Відключити клієнта";
const char STR_CONT_OPEN_LOBBY[] = "Відкрити лоббі";
const char STR_CONT_CLOSE_LOBBY[] = "Закрити лоббі";
const char STR_CONT_GAME_START[] = "Розпочати гру";
const char STR_CONT_GAME_EXIT[] = "Завершити гру";
//
const char STR_SERVER_ID[] = "Test_Game";
//
const char STR_DEF_NICK[] = "Nemo";

namespace test_server
{
    TestServerScreen::TestServerScreen(GraphicsDriver &display) : IContext{display}
    {
        showMainMenu();
    }

    TestServerScreen::~TestServerScreen()
    {
        delete _img_back_normal;
        delete _img_back_hover;

        _client.disconnect();
        _server.stop();
    }

    //-----------------------------------------------------------------------------------------------------------------------------------------------

    void TestServerScreen::loop()
    {
    }

    void TestServerScreen::update()
    {
        switch (_context_id)
        {
        case ID_CONT_GAME:
            updateGame();
            break;
            //
        case ID_CONT_MAIN:
            handleMainBtns();
            break;
        case ID_CONT_ROLE:
            handleRoleBtns();
            break;
        case ID_CONT_WIFI_LIST:
            handleWifiListBtns();
            break;
        case ID_CONT_CLIENT_LOBBY:
            handleClientLobbyBtns();
            break;
        case ID_CONT_CONN_TO_AP:
            handleConnToAPBtns();
            break;
        //
        case ID_CONT_SERVER_LOBBY:
            handleServerLobbyBtns();
            break;
        case ID_CONT_LOBBY_CONT_MENU:
            handleLobbyContMenuBtns();
            break;
        case ID_CONT_CLIENT_CONFIRM:
            handleClientConnBtns();
            break;
        //
        case ID_CONT_PREF_MAIN:
            handlePrefMainBtns();
            break;

        case ID_CONT_CONN_DIALOG:
        case ID_CONT_PREF_NICK:
        case ID_CONT_PREF_SERV_NAME:
        case ID_CONT_PREF_SERV_PWD:
            handleDialBtns();
            break;
        }
    }

    void TestServerScreen::addDialog(EmptyLayout *layout, const char *title_txt, const char *start_txt)
    {
        Label *title_lbl = new Label(ID_CONTEXT_LBL, _display);
        layout->addWidget(title_lbl);
        title_lbl->setText(title_txt);
        title_lbl->setAlign(IWidget::ALIGN_CENTER);
        title_lbl->setGravity(IWidget::GRAVITY_CENTER);
        title_lbl->setWidth(D_WIDTH);
        title_lbl->setBackColor(COLOR_MAIN_BACK);
        title_lbl->setTextColor(TFT_WHITE);
        title_lbl->setTicker(true);

        _dialog_txt = new TextBox(ID_PWD_TXT, _display);
        layout->addWidget(_dialog_txt);
        _dialog_txt->setText(start_txt);
        _dialog_txt->setTextOffset(5);
        _dialog_txt->setWidth(D_WIDTH - 10);
        _dialog_txt->setHeight(40);
        _dialog_txt->setBackColor(TFT_WHITE);
        _dialog_txt->setTextColor(TFT_BLACK);
        _dialog_txt->setFontID(2);
        _dialog_txt->setTextSize(2);
        _dialog_txt->setPos(5, 0);
        _dialog_txt->setCornerRadius(3);

        WidgetCreator creator{_display};
        _keyboard = creator.getStandardEnKeyboard(ID_KEYBOARD);
        layout->addWidget(_keyboard);
    }

    //-----------------------------------------------------------------------------------------------------------------------------------------------

    void TestServerScreen::wifiScanDoneHandler(void *arg)
    {
        TestServerScreen *this_ptr = static_cast<TestServerScreen *>(arg);
        this_ptr->readWifiScan();
    }

    void TestServerScreen::readWifiScan()
    {
        if (_context_id != ID_CONT_WIFI_LIST)
            return;

        IWidget *w_lbl = getLayout()->findWidgetByID(ID_CONTEXT_LBL);
        if (!w_lbl)
            return;

        Label *title = static_cast<Label *>(w_lbl);
        title->setText(STR_SERVER_SCANNING_DONE);

        WidgetCreator creator{_display};
        std::vector<String> result;
        _wifi_scanner.getScanResult(result);
        uint16_t item_id_counter = 1;
        for (auto it = result.begin(), last_it = result.end(); it != last_it; ++it, ++item_id_counter)
        {
            MenuItem *item_client = creator.getMenuItem(item_id_counter);
            _wifi_list_menu->addItem(item_client);
            item_client->setBackColor(COLOR_MAIN_BACK);
            item_client->setCornerRadius(10);

            Label *lbl_client = new Label(1, _display);
            item_client->setLbl(lbl_client);
            lbl_client->setText(*it);
            lbl_client->setAlign(IWidget::ALIGN_CENTER);
            lbl_client->setGravity(IWidget::GRAVITY_CENTER);
            lbl_client->setTextColor(TFT_WHITE);
            lbl_client->setCornerRadius(10);
        }
    }

    void TestServerScreen::showMainMenu()
    {
        _context_id = ID_CONT_MAIN;

        WidgetCreator creator{_display};
        EmptyLayout *layout = creator.getEmptyLayout();

        _img_back_normal = new Image(1, _display);
        _img_back_normal->init(IMG_W, IMG_H);
        _img_back_normal->setSrc(BTN_BACK_NORMAL);
        _img_back_normal->setBackColor(COLOR_MAIN_BACK);
        _img_back_normal->setTransparentColor(Image::COLOR_TRANSPARENT);

        _img_back_hover = new Image(1, _display);
        _img_back_hover->init(IMG_W, IMG_H);
        _img_back_hover->setSrc(BTN_BACK_HOVER);
        _img_back_hover->setBackColor(COLOR_MAIN_BACK);
        _img_back_hover->setTransparentColor(Image::COLOR_TRANSPARENT);
        //
        _main_menu = new FixedMenu(ID_MAIN_MENU, _display);
        layout->addWidget(_main_menu);
        _main_menu->setBackColor(COLOR_MAIN_BACK);
        _main_menu->setItemsSpacing(ITEMS_SPASING);
        _main_menu->setWidth(IMG_W + 2);
        _main_menu->setHeight((IMG_H + ITEMS_SPASING) * 3);
        _main_menu->setItemWidth(IMG_W);
        _main_menu->setItemHeight(IMG_H);
        _main_menu->setPos(getCenterX(_main_menu), 0);
        _main_menu->setLoopState(true);

        //
        MenuItem *item_online = new MenuItem(ID_ITEM_ONLINE, _display);
        _main_menu->addItem(item_online);
        item_online->setBackColor(COLOR_MAIN_BACK);

        Label *lbl_online = new Label(1, _display);
        item_online->setLbl(lbl_online);
        lbl_online->setAlign(IWidget::ALIGN_CENTER);
        lbl_online->setGravity(IWidget::GRAVITY_CENTER);
        lbl_online->setBackImg(_img_back_hover);
        lbl_online->setText(STR_MULTI_GAME);
        lbl_online->setTextColor(TFT_BLACK);

        //
        MenuItem *item_offline = new MenuItem(ID_ITEM_OFFLINE, _display);
        _main_menu->addItem(item_offline);
        item_offline->setBackColor(COLOR_MAIN_BACK);

        Label *lbl_offline = lbl_online->clone(1);
        item_offline->setLbl(lbl_offline);
        lbl_offline->setBackImg(_img_back_normal);
        lbl_offline->setText(STR_SOLO_GAME);

        //
        MenuItem *item_pref = new MenuItem(ID_ITEM_PREF, _display);
        _main_menu->addItem(item_pref);
        item_pref->setBackColor(COLOR_MAIN_BACK);

        Label *lbl_pref = lbl_online->clone(1);
        item_pref->setLbl(lbl_pref);
        lbl_pref->setBackImg(_img_back_normal);
        lbl_pref->setText(STR_PREF);

        setLayout(layout);
    }

    void TestServerScreen::handleMainBtns()
    {
        if (_input.isReleased(KeyID::KEY_BACK))
        {
            _input.lock(KeyID::KEY_BACK, CLICK_LOCK);
            openContextByID(ID_CONTEXT_GAMES);
        }
        else if (_input.isHolded(KeyID::KEY_UP))
        {
            _input.lock(KeyID::KEY_UP, HOLD_LOCK);
            MenuItem *item = _main_menu->getCurrentItem();
            item->getLabel()->setBackImg(_img_back_normal);
            _main_menu->focusUp();
            item = _main_menu->getCurrentItem();
            item->getLabel()->setBackImg(_img_back_hover);
        }
        else if (_input.isHolded(KeyID::KEY_DOWN))
        {
            _input.lock(KeyID::KEY_DOWN, HOLD_LOCK);
            MenuItem *item = _main_menu->getCurrentItem();
            item->getLabel()->setBackImg(_img_back_normal);
            _main_menu->focusDown();
            item = _main_menu->getCurrentItem();
            item->getLabel()->setBackImg(_img_back_hover);
        }
        else if (_input.isReleased(KeyID::KEY_OK))
        {
            _input.lock(KeyID::KEY_OK, CLICK_LOCK);

            switch (_main_menu->getCurrentItemID())
            {
            case ID_ITEM_ONLINE:
                showSelectRole();
                break;
            case ID_ITEM_OFFLINE:
                showMainScene();
                break;
            case ID_ITEM_PREF:
                showPrefMain();
                break;
            }
        }
    }

    void TestServerScreen::showSelectRole()
    {
        _context_id = ID_CONT_ROLE;

        _is_server = false;
        _is_client = false;

        WidgetCreator creator{_display};
        EmptyLayout *layout = creator.getEmptyLayout();

        Label *title_lbl = new Label(ID_CONTEXT_LBL, _display);
        layout->addWidget(title_lbl);
        title_lbl->setText(STR_SELECT_ROLE_TITLE);
        title_lbl->setAlign(IWidget::ALIGN_CENTER);
        title_lbl->setGravity(IWidget::GRAVITY_CENTER);
        title_lbl->setWidth(D_WIDTH);
        title_lbl->setBackColor(COLOR_MAIN_BACK);
        title_lbl->setTextColor(TFT_WHITE);

        _role_menu = new FixedMenu(ID_ROLE_MENU, _display);
        layout->addWidget(_role_menu);
        _role_menu->setBackColor(COLOR_MAIN_BACK);
        _role_menu->setWidth(200);
        _role_menu->setHeight(IMG_H * 2 + 4);
        _role_menu->setItemHeight(IMG_H);
        _role_menu->setPos(getCenterX(_role_menu), 30);
        _role_menu->setLoopState(true);

        // Клієнт
        MenuItem *item_client = creator.getMenuItem(ID_ITEM_CLIENT);
        _role_menu->addItem(item_client);
        item_client->setBackColor(COLOR_MAIN_BACK);
        item_client->setCornerRadius(10);

        Label *lbl_client = new Label(1, _display);
        item_client->setLbl(lbl_client);
        lbl_client->setText(STR_CLIENT);
        lbl_client->setAlign(IWidget::ALIGN_CENTER);
        lbl_client->setGravity(IWidget::GRAVITY_CENTER);
        lbl_client->setTextColor(TFT_WHITE);
        lbl_client->setCornerRadius(10);

        // Сервер
        MenuItem *item_server = creator.getMenuItem(ID_ITEM_SERVER);
        _role_menu->addItem(item_server);
        item_server->setBackColor(COLOR_MAIN_BACK);
        item_server->setCornerRadius(10);

        Label *lbl_server = new Label(1, _display);
        item_server->setLbl(lbl_server);
        lbl_server->setText(STR_SERVER);
        lbl_server->setAlign(IWidget::ALIGN_CENTER);
        lbl_server->setGravity(IWidget::GRAVITY_CENTER);
        lbl_server->setTextColor(TFT_WHITE);
        lbl_server->setCornerRadius(10);

        setLayout(layout);
    }

    void TestServerScreen::handleRoleBtns()
    {
        if (_input.isHolded(KeyID::KEY_UP))
        {
            _input.lock(KeyID::KEY_UP, HOLD_LOCK);
            _role_menu->focusUp();
        }
        else if (_input.isHolded(KeyID::KEY_DOWN))
        {
            _input.lock(KeyID::KEY_DOWN, HOLD_LOCK);
            _role_menu->focusDown();
        }
        else if (_input.isReleased(KeyID::KEY_OK))
        {
            _input.lock(KeyID::KEY_OK, CLICK_LOCK);

            switch (_role_menu->getCurrentItemID())
            {
            case ID_ITEM_CLIENT:
                showWifiList();
                break;
            case ID_ITEM_SERVER:
                openServerLobby();
                break;
            }
        }
        else if (_input.isReleased(KeyID::KEY_BACK))
        {
            _input.lock(KeyID::KEY_BACK, CLICK_LOCK);
            showMainMenu();
        }
    }

    void TestServerScreen::showClientLobby()
    {
        _context_id = ID_CONT_CLIENT_LOBBY;

        WidgetCreator creator{_display};
        EmptyLayout *layout = creator.getEmptyLayout();

        Label *title_lbl = new Label(ID_CONTEXT_LBL, _display);
        layout->addWidget(title_lbl);
        title_lbl->setText(STR_WAITING_GAME);
        title_lbl->setAlign(IWidget::ALIGN_CENTER);
        title_lbl->setGravity(IWidget::GRAVITY_CENTER);
        title_lbl->setWidth(D_WIDTH);
        title_lbl->setBackColor(COLOR_MAIN_BACK);
        title_lbl->setTextColor(TFT_WHITE);
        title_lbl->setTicker(true);

        setLayout(layout);
    }

    void TestServerScreen::showWifiList()
    {
        _context_id = ID_CONT_WIFI_LIST;

        _is_client = true;

        WidgetCreator creator{_display};
        EmptyLayout *layout = creator.getEmptyLayout();

        Label *title_lbl = new Label(ID_CONTEXT_LBL, _display);
        layout->addWidget(title_lbl);
        title_lbl->setText(STR_SERVER_SCANNING);
        title_lbl->setAlign(IWidget::ALIGN_CENTER);
        title_lbl->setGravity(IWidget::GRAVITY_CENTER);
        title_lbl->setWidth(D_WIDTH);
        title_lbl->setBackColor(COLOR_MAIN_BACK);
        title_lbl->setTextColor(TFT_WHITE);

        _wifi_list_menu = new FixedMenu(ID_WIFI_LIST_MENU, _display);
        layout->addWidget(_wifi_list_menu);
        _wifi_list_menu->setBackColor(COLOR_MAIN_BACK);
        _wifi_list_menu->setWidth(D_WIDTH);
        _wifi_list_menu->setHeight(IMG_H * 4 + 4);
        _wifi_list_menu->setItemHeight(IMG_H);
        _wifi_list_menu->setPos(0, 30);
        _wifi_list_menu->setLoopState(true);

        setLayout(layout);

        _wifi_scanner.setOnDoneHandler(wifiScanDoneHandler, this);
        _wifi_scanner.startScan();
    }

    void TestServerScreen::handleWifiListBtns()
    {
        if (_input.isHolded(KeyID::KEY_UP))
        {
            _input.lock(KeyID::KEY_UP, HOLD_LOCK);
            _wifi_list_menu->focusUp();
        }
        else if (_input.isHolded(KeyID::KEY_DOWN))
        {
            _input.lock(KeyID::KEY_DOWN, HOLD_LOCK);
            _wifi_list_menu->focusDown();
        }
        else if (_input.isReleased(KeyID::KEY_OK))
        {
            _input.lock(KeyID::KEY_OK, CLICK_LOCK);

            MenuItem *cur_item = _wifi_list_menu->getCurrentItem();
            if (!cur_item)
                return;

            _serv_ssid = cur_item->getText();

            showConnectDialog();
        }
        else if (_input.isReleased(KeyID::KEY_BACK))
        {
            _input.lock(KeyID::KEY_BACK, CLICK_LOCK);
            _wifi_connect.disconnect();
            showSelectRole();
        }
    }

    void TestServerScreen::showConnectDialog()
    {
        SettingsManager settings;
        _client_nick = settings.get(STR_TEST_GAME_NICK);
        _serv_pwd = settings.get(STR_TEST_GAME_C_PWD);

        if (_client_nick.isEmpty())
            _client_nick = STR_DEF_NICK;
        if (_serv_pwd.isEmpty())
            _serv_pwd = STR_DEF_PWD;

        _context_id = ID_CONT_CONN_DIALOG;

        WidgetCreator creator{_display};
        EmptyLayout *layout = creator.getEmptyLayout();

        String title = STR_ENTER_PWD;
        title += _serv_ssid;
        addDialog(layout, title.c_str(), _serv_pwd.c_str());
        setLayout(layout);
    }

    void TestServerScreen::showConnToAP()
    {
        _context_id = ID_CONT_CONN_TO_AP;
        WidgetCreator creator{_display};
        EmptyLayout *layout = creator.getEmptyLayout();

        Label *title_lbl = new Label(ID_CONTEXT_LBL, _display);
        layout->addWidget(title_lbl);
        title_lbl->setText(STR_CONNECTING_TO + _serv_ssid);
        title_lbl->setAlign(IWidget::ALIGN_CENTER);
        title_lbl->setGravity(IWidget::GRAVITY_CENTER);
        title_lbl->setWidth(D_WIDTH);
        title_lbl->setPos(0, 30);
        title_lbl->setBackColor(COLOR_MAIN_BACK);
        title_lbl->setTextColor(TFT_WHITE);
        title_lbl->setTicker(true);

        setLayout(layout);

        _wifi_connect.setOnDoneHandler(wifiConnDoneHandler, this);
        if (!_wifi_connect.tryConnectTo(_serv_ssid.c_str(), _serv_pwd.c_str(), SERVER_WIFI_CHANN))
            title_lbl->setText(STR_CONNECTING_ERROR);
    }

    void TestServerScreen::updateConnectState(wl_status_t conn_status)
    {
        if (_context_id != ID_CONT_CONN_TO_AP)
            return;

        IWidget *widget = getLayout()->findWidgetByID(ID_CONTEXT_LBL);

        if (!widget)
            return;

        Label *context_lbl = static_cast<Label *>(widget);

        if (conn_status != WL_CONNECTED)
        {
            context_lbl->setText(STR_CONNECTING_ERROR);
        }
        else
        {
            context_lbl->setText(STR_CONNECTING);

            _client.setName(_client_nick.c_str());
            _client.setServerID(STR_SERVER_ID);
            _client.onConnect(serverConnHandler, this);
            _client.onDisconnect(serverDisconnHandler, this);
            _client.connect();
        }
    }

    void TestServerScreen::handleServerData(UdpPacket *packet)
    {
        uint8_t START_GAME_DATA_CMD = 100;
        if (packet->isDataEquals(&START_GAME_DATA_CMD, 0, 1))
        {
            log_i("Запуск гри");
            _client.onData(nullptr, nullptr);
            showMainScene();
        }
    }

    void TestServerScreen::onServerData(UdpPacket *packet, void *arg)
    {
        TestServerScreen *this_ptr = static_cast<TestServerScreen *>(arg);
        this_ptr->handleServerData(packet);
    }

    void TestServerScreen::handleServerConn()
    {
        log_i("Авторизовано на сервері");

        if (_context_id == ID_CONT_CONN_TO_AP)
        {
            _client.onData(onServerData, this);
            showClientLobby();
        }
    }

    void TestServerScreen::handleServerDisconn()
    {
        if (_context_id == ID_CONT_CONN_TO_AP)
        {
            IWidget *widget = getLayout()->findWidgetByID(ID_CONTEXT_LBL);

            if (!widget)
                return;

            Label *context_lbl = static_cast<Label *>(widget);
            context_lbl->setText(STR_DISCONNECTED);
        }
        else if (_context_id == ID_CONT_CLIENT_LOBBY)
        {
            showWifiList();
        }
    }

    void TestServerScreen::handleClientLobbyBtns()
    {
        if (_input.isReleased(KeyID::KEY_BACK))
        {
            _input.lock(KeyID::KEY_BACK, CLICK_LOCK);
            _wifi_connect.setOnDoneHandler(nullptr, nullptr);
            _client.disconnect();
            showWifiList();
        }
    }

    void TestServerScreen::serverConnHandler(void *arg)
    {
        TestServerScreen *this_ptr = static_cast<TestServerScreen *>(arg);
        this_ptr->handleServerConn();
    }

    void TestServerScreen::serverDisconnHandler(void *arg)
    {
        TestServerScreen *this_ptr = static_cast<TestServerScreen *>(arg);
        this_ptr->handleServerDisconn();
    }

    void TestServerScreen::handleConnToAPBtns()
    {
        if (_input.isReleased(KeyID::KEY_BACK))
        {
            _input.lock(KeyID::KEY_BACK, CLICK_LOCK);
            _wifi_connect.setOnDoneHandler(nullptr, nullptr);
            _client.disconnect();
            showSelectRole();
        }
    }

    void TestServerScreen::wifiConnDoneHandler(void *arg, wl_status_t conn_status)
    {
        TestServerScreen *this_ptr = static_cast<TestServerScreen *>(arg);
        this_ptr->updateConnectState(conn_status);
    }

    // ------------------------------------------------------------------------------------------------------------

    void TestServerScreen::showServerLobby()
    {
        _context_id = ID_CONT_SERVER_LOBBY;

        WidgetCreator creator{_display};
        EmptyLayout *layout = creator.getEmptyLayout();

        Label *title_lbl = new Label(ID_CONTEXT_LBL, _display);
        layout->addWidget(title_lbl);
        title_lbl->setText(STR_WAITING_CLIENT);
        title_lbl->setAlign(IWidget::ALIGN_CENTER);
        title_lbl->setGravity(IWidget::GRAVITY_CENTER);
        title_lbl->setWidth(D_WIDTH);
        title_lbl->setBackColor(COLOR_MAIN_BACK);
        title_lbl->setTextColor(TFT_WHITE);

        _clients_list_menu = new FixedMenu(ID_CLIENT_LIST_MENU, _display);
        layout->addWidget(_clients_list_menu);
        _clients_list_menu->setBackColor(COLOR_MAIN_BACK);
        _clients_list_menu->setWidth(D_WIDTH);
        _clients_list_menu->setHeight(IMG_H * 4 + 4);
        _clients_list_menu->setItemHeight(IMG_H);
        _clients_list_menu->setPos(0, 30);
        _clients_list_menu->setLoopState(true);

        setLayout(layout);

        updateClientsList();
    }

    void TestServerScreen::updateClientsList(ClientWrapper *disconnected_client)
    {
        if (_context_id != ID_CONT_SERVER_LOBBY && _context_id != ID_CONT_LOBBY_CONT_MENU)
            return;

        _clients_list_menu->deleteWidgets();
        _clients_list_menu->disable();
        WidgetCreator creator{_display};
        const std::unordered_map<uint32_t, ClientWrapper *> *clients = _server.getClients();

        uint16_t i = 1;
        for (auto it = clients->begin(), last_it = clients->end(); it != last_it; ++it, ++i)
        {
            if (!it->second->is(disconnected_client) && it->second->isConfirmed())
            {
                MenuItem *new_item = creator.getMenuItem(i);
                _clients_list_menu->addItem(new_item);

                Label *new_lbl = creator.getItemLabel(it->second->getName(), 2);
                new_item->setLbl(new_lbl);
                new_lbl->setTextOffset(3);
            }
        }

        _clients_list_menu->enable();
    }

    void TestServerScreen::handleServerLobbyBtns()
    {
        if (_input.isHolded(KeyID::KEY_UP))
        {
            _input.lock(KeyID::KEY_UP, HOLD_LOCK);
            _clients_list_menu->focusUp();
        }
        else if (_input.isHolded(KeyID::KEY_DOWN))
        {
            _input.lock(KeyID::KEY_DOWN, HOLD_LOCK);
            _clients_list_menu->focusDown();
        }
        else if (_input.isPressed(KeyID::KEY_OK))
        {
            _input.lock(KeyID::KEY_OK, PRESS_LOCK);
            showServLobbyContMenu();
        }
        else if (_input.isReleased(KeyID::KEY_BACK))
        {
            _input.lock(KeyID::KEY_BACK, CLICK_LOCK);
            _server.stop();
            showSelectRole();
        }
    }

    void TestServerScreen::openServerLobby()
    {
        SettingsManager settings;
        _serv_ssid = settings.get(STR_TEST_GAME_S_NAME);
        _serv_pwd = settings.get(STR_TEST_GAME_S_PWD);

        if (_serv_ssid.isEmpty())
            _serv_ssid = STR_DEF_SSID;
        if (_serv_pwd.isEmpty())
            _serv_pwd = STR_DEF_PWD;

        showServerLobby();
        _is_server = true;
        _server.setServerID(STR_SERVER_ID);
        _server.onConfirmation(clientConfirmHandler, this);
        _server.onDisconnect(clientDisconnHandler, this);

        _server.begin(_serv_ssid.c_str(), _serv_pwd.c_str());
        _server.open();
    }

    void TestServerScreen::showServLobbyContMenu()
    {
        _context_id = ID_CONT_LOBBY_CONT_MENU;
        _clients_list_menu->disable();

        WidgetCreator creator{_display};

        _lobby_context_menu = new FixedMenu(ID_LOBBY_CONTEXT_MENU, _display);
        getLayout()->addWidget(_lobby_context_menu);
        _lobby_context_menu->setItemHeight(18);
        _lobby_context_menu->setWidth((float)D_WIDTH / 2.2);
        _lobby_context_menu->setBackColor(TFT_BLACK);
        _lobby_context_menu->setBorder(true);
        _lobby_context_menu->setBorderColor(TFT_ORANGE);
        _lobby_context_menu->setLoopState(true);

        uint16_t id = _clients_list_menu->getCurrentItemID();

        if (id > 0)
        {
            // Відключити
            MenuItem *disconn_item = creator.getMenuItem(ID_ITEM_DISC_CLIENT);
            _lobby_context_menu->addItem(disconn_item);

            Label *disconn_lbl = creator.getItemLabel(STR_CONT_DISC_CLIENT, 1);
            disconn_item->setLbl(disconn_lbl);
            disconn_lbl->setTextOffset(1);
        }

        if (_server.isOpen())
        {
            // Закрити лоббі
            MenuItem *close_item = creator.getMenuItem(ID_ITEM_CLOSE_LOBBY);
            _lobby_context_menu->addItem(close_item);

            Label *close_lbl = creator.getItemLabel(STR_CONT_CLOSE_LOBBY, 1);
            close_item->setLbl(close_lbl);
            close_lbl->setTextOffset(1);
        }
        else
        {
            // Відкрити лоббі
            MenuItem *open_item = creator.getMenuItem(ID_ITEM_OPEN_LOBBY);
            _lobby_context_menu->addItem(open_item);

            Label *open_lbl = creator.getItemLabel(STR_CONT_OPEN_LOBBY, 1);
            open_item->setLbl(open_lbl);
            open_lbl->setTextOffset(1);
        }

        // Завершити гру
        MenuItem *end_item = creator.getMenuItem(ID_ITEM_END_GAME);
        _lobby_context_menu->addItem(end_item);

        Label *end_lbl = creator.getItemLabel(STR_CONT_GAME_EXIT, 1);
        end_item->setLbl(end_lbl);
        end_lbl->setTextOffset(1);

        // Розпочати гру
        MenuItem *start_item = creator.getMenuItem(ID_ITEM_START_GAME);
        _lobby_context_menu->addItem(start_item);

        Label *start_lbl = creator.getItemLabel(STR_CONT_GAME_START, 1);
        start_item->setLbl(start_lbl);
        start_lbl->setTextOffset(1);

        _lobby_context_menu->setHeight(_lobby_context_menu->getSize() * _lobby_context_menu->getItemHeight() + 4);
        _lobby_context_menu->setPos(D_WIDTH - _lobby_context_menu->getWidth() - 2,
                                    D_HEIGHT - _lobby_context_menu->getHeight() - 2);
    }

    void TestServerScreen::hideServLobbyContMenu()
    {
        if (_context_id != ID_CONT_LOBBY_CONT_MENU)
            return;

        _context_id = ID_CONT_SERVER_LOBBY;

        getLayout()->deleteWidgetByID(ID_LOBBY_CONTEXT_MENU);
        _clients_list_menu->enable();
        _context_id == ID_CONT_SERVER_LOBBY;
    }

    void TestServerScreen::handleLobbyContMenuBtns()
    {
        if (_context_id != ID_CONT_LOBBY_CONT_MENU)
            return;

        if (_input.isReleased(KeyID::KEY_OK))
        {
            _input.lock(KeyID::KEY_OK, CLICK_LOCK);

            uint16_t id = _lobby_context_menu->getCurrentItemID();

            String client_name;

            switch (id)
            {
            case ID_ITEM_DISC_CLIENT:
                client_name = _clients_list_menu->getCurrentItemText();
                if (!client_name.isEmpty())
                {
                    _server.removeClient(client_name.c_str());
                    updateClientsList();
                }
                break;
            case ID_ITEM_CLOSE_LOBBY:
                _server.close();
                break;
            case ID_ITEM_OPEN_LOBBY:
                _server.open();
                break;
            case ID_ITEM_START_GAME:
                showMainScene();
                break;
            case ID_ITEM_END_GAME:
                _server.stop();
                showSelectRole();
                break;
            }

            hideServLobbyContMenu();
        }
        else if (_input.isReleased(KeyID::KEY_BACK))
        {
            _input.lock(KeyID::KEY_BACK, CLICK_LOCK);
            hideServLobbyContMenu();
        }
        else if (_input.isHolded(KeyID::KEY_UP))
        {
            _input.lock(KeyID::KEY_UP, HOLD_LOCK);
            _lobby_context_menu->focusUp();
        }
        else if (_input.isHolded(KeyID::KEY_DOWN))
        {
            _input.lock(KeyID::KEY_DOWN, HOLD_LOCK);
            _lobby_context_menu->focusDown();
        }
    }

    void TestServerScreen::handleClientConnBtns()
    {
        if (_input.isReleased(KeyID::KEY_OK))
        {
            _input.lock(KeyID::KEY_OK, CLICK_LOCK);
            _confirm_handler(_conn_client_wrap, true, &_server);
            showServerLobby();
        }
        else if (_input.isReleased(KeyID::KEY_BACK))
        {
            _input.lock(KeyID::KEY_BACK, CLICK_LOCK);
            _confirm_handler(_conn_client_wrap, false, &_server);
            showServerLobby();
        }
    }

    void TestServerScreen::showClientConn()
    {
        _context_id = ID_CONT_CLIENT_CONFIRM;

        WidgetCreator creator{_display};
        EmptyLayout *layout = creator.getEmptyLayout();

        String msg_str = _conn_client_wrap->getName();
        msg_str += STR_WANTS_TO_JOIN;

        Label *title_lbl = new Label(ID_CONTEXT_LBL, _display);
        layout->addWidget(title_lbl);
        title_lbl->setText(msg_str.c_str());
        title_lbl->setAlign(IWidget::ALIGN_CENTER);
        title_lbl->setGravity(IWidget::GRAVITY_CENTER);
        title_lbl->setWidth(D_WIDTH);
        title_lbl->setBackColor(COLOR_MAIN_BACK);
        title_lbl->setTextColor(TFT_WHITE);
        title_lbl->setPos(0, 40);

        setLayout(layout);
    }

    void TestServerScreen::handleClientConn(ClientWrapper *cl_wrap, ConfirmResultHandler result_handler)
    {
        _conn_client_wrap = cl_wrap;
        _confirm_handler = result_handler;

        showClientConn();
    }

    void TestServerScreen::clientConfirmHandler(ClientWrapper *cl_wrap, ConfirmResultHandler result_handler, void *arg)
    {
        TestServerScreen *this_ptr = static_cast<TestServerScreen *>(arg);
        this_ptr->handleClientConn(cl_wrap, result_handler);
    }

    void TestServerScreen::handleClientDisconn(ClientWrapper *cl_wrap)
    {
        updateClientsList(cl_wrap);
    }

    void TestServerScreen::clientDisconnHandler(ClientWrapper *cl_wrap, void *arg)
    {
        TestServerScreen *this_ptr = static_cast<TestServerScreen *>(arg);
        this_ptr->handleClientDisconn(cl_wrap);
    }

    //-----------------------------------------------------------------------------------------------------------------------------------------------
    void TestServerScreen::showPrefMain()
    {
        _context_id = ID_CONT_PREF_MAIN;

        WidgetCreator creator{_display};
        EmptyLayout *layout = creator.getEmptyLayout();

        _pref_menu = new FixedMenu(ID_PREF_MAIN_MENU, _display);
        layout->addWidget(_pref_menu);
        _pref_menu->setBackColor(COLOR_MAIN_BACK);
        _pref_menu->setWidth(D_WIDTH);
        _pref_menu->setHeight(D_HEIGHT);
        _pref_menu->setItemHeight(IMG_H);
        _pref_menu->setLoopState(true);

        // nick
        MenuItem *nick_item = creator.getMenuItem(ID_ITEM_NICK);
        _pref_menu->addItem(nick_item);
        nick_item->setBackColor(COLOR_MAIN_BACK);

        Label *nick_lbl = creator.getItemLabel(STR_NICKNAME);
        nick_item->setLbl(nick_lbl);

        // serv name
        MenuItem *serv_name_item = creator.getMenuItem(ID_ITEM_SERV_NAME);
        _pref_menu->addItem(serv_name_item);
        serv_name_item->setBackColor(COLOR_MAIN_BACK);

        Label *serv_name_lbl = creator.getItemLabel(STR_SERV_NAME);
        serv_name_item->setLbl(serv_name_lbl);

        // client name
        MenuItem *serv_pwd_item = creator.getMenuItem(ID_ITEM_SERV_PWD);
        _pref_menu->addItem(serv_pwd_item);
        serv_pwd_item->setBackColor(COLOR_MAIN_BACK);

        Label *serv_pwd_lbl = creator.getItemLabel(STR_SERV_PWD);
        serv_pwd_item->setLbl(serv_pwd_lbl);

        setLayout(layout);
    }

    void TestServerScreen::handlePrefMainBtns()
    {
        if (_input.isHolded(KeyID::KEY_UP))
        {
            _input.lock(KeyID::KEY_UP, 250);
            _pref_menu->focusUp();
        }
        else if (_input.isHolded(KeyID::KEY_DOWN))
        {
            _input.lock(KeyID::KEY_DOWN, 250);
            _pref_menu->focusDown();
        }
        else if (_input.isReleased(KeyID::KEY_OK))
        {
            _input.lock(KeyID::KEY_OK, CLICK_LOCK);

            uint16_t id = _pref_menu->getCurrentItemID();

            if (id == ID_ITEM_NICK)
                showPrefNick();
            else if (id == ID_ITEM_SERV_NAME)
                showPrefServName();
            else if (id == ID_ITEM_SERV_PWD)
                showPrefServPwd();
        }
        else if (_input.isReleased(KeyID::KEY_BACK))
        {
            _input.lock(KeyID::KEY_BACK, CLICK_LOCK);
            showMainMenu();
        }
    }

    void TestServerScreen::showPrefNick()
    {
        _context_id = ID_CONT_PREF_NICK;
        SettingsManager setting;
        _client_nick = setting.get(STR_TEST_GAME_NICK);

        WidgetCreator creator{_display};
        EmptyLayout *layout = creator.getEmptyLayout();

        addDialog(layout, STR_ENTER_NICK, _client_nick.c_str());

        setLayout(layout);
    }

    void TestServerScreen::showPrefServName()
    {
        _context_id = ID_CONT_PREF_SERV_NAME;

        SettingsManager setting;
        _serv_ssid = setting.get(STR_TEST_GAME_S_NAME);

        WidgetCreator creator{_display};
        EmptyLayout *layout = creator.getEmptyLayout();

        addDialog(layout, STR_ENTER_SERV_NAME, _serv_ssid.c_str());

        setLayout(layout);
    }

    void TestServerScreen::showPrefServPwd()
    {
        _context_id = ID_CONT_PREF_SERV_PWD;

        SettingsManager setting;
        _serv_pwd = setting.get(STR_TEST_GAME_S_PWD);

        WidgetCreator creator{_display};
        EmptyLayout *layout = creator.getEmptyLayout();

        addDialog(layout, STR_ENTER_SERV_PWD, _serv_pwd.c_str());

        setLayout(layout);
    }

    void TestServerScreen::handleDialBtns()
    {
        if (_input.isHolded(KeyID::KEY_UP))
        {
            _input.lock(KeyID::KEY_UP, CLICK_LOCK);
            _keyboard->focusUp();
        }
        else if (_input.isHolded(KeyID::KEY_DOWN))
        {
            _input.lock(KeyID::KEY_DOWN, CLICK_LOCK);
            _keyboard->focusDown();
        }
        else if (_input.isHolded(KeyID::KEY_LEFT))
        {
            _input.lock(KeyID::KEY_LEFT, CLICK_LOCK);
            _keyboard->focusLeft();
        }
        else if (_input.isHolded(KeyID::KEY_RIGHT))
        {
            _input.lock(KeyID::KEY_RIGHT, CLICK_LOCK);
            _keyboard->focusRight();
        }
        else if (_input.isReleased(KeyID::KEY_OK))
        {
            _input.lock(KeyID::KEY_OK, CLICK_LOCK);
            _dialog_txt->addChars(_keyboard->getCurrentBtnTxt().c_str());
        }
        else if (_input.isReleased(KeyID::KEY_BACK))
        {
            _input.lock(KeyID::KEY_BACK, CLICK_LOCK);
            _dialog_txt->removeLastChar();
        }
        else if (_input.isPressed(KeyID::KEY_OK))
        {
            _input.lock(KeyID::KEY_OK, PRESS_LOCK);
            handlePrefSaveBtns();
        }
        else if (_input.isPressed(KeyID::KEY_BACK))
        {
            _input.lock(KeyID::KEY_BACK, PRESS_LOCK);

            if (_context_id == ID_CONT_CONN_DIALOG)
                showWifiList();
            else
                showPrefMain();
        }
    }

    void TestServerScreen::handlePrefSaveBtns()
    {
        SettingsManager settings;

        if (_context_id == ID_CONT_PREF_NICK)
        {
            _client_nick = _dialog_txt->getText();
            settings.set(STR_TEST_GAME_NICK, _client_nick.c_str());
        }
        else if (_context_id == ID_CONT_PREF_SERV_NAME)
        {
            _serv_ssid = _dialog_txt->getText();
            settings.set(STR_TEST_GAME_S_NAME, _serv_ssid.c_str());
        }
        else if (_context_id == ID_CONT_PREF_SERV_PWD)
        {
            _serv_pwd = _dialog_txt->getText();
            settings.set(STR_TEST_GAME_S_PWD, _serv_pwd.c_str());
        }
        else if (_context_id == ID_CONT_CONN_DIALOG)
        {
            _serv_pwd = _dialog_txt->getText();
            settings.set(STR_TEST_GAME_C_PWD, _serv_pwd.c_str());
            showConnToAP();
            return;
        }

        showPrefMain();
    }

    //-----------------------------------------------------------------------------------------------------------------------------------------------

    void TestServerScreen::showMainScene()
    {
        getLayout()->deleteWidgets();

        if (_is_client)
            _scene = new MainScene(_display, _input, _stored_objs, _client);
        else if (_is_server)
        {
            _server.onDisconnect(nullptr, nullptr);
            _server.onConfirmation(nullptr, nullptr);
            _server.close();
            _scene = new MainScene(_display, _input, _stored_objs, _server);
        }
        else
            _scene = new MainScene(_display, _input, _stored_objs);

        _context_id = ID_CONT_GAME;
    }

    void TestServerScreen::updateGame()
    {
        if (!_scene->isFinished() && !_scene->isReleased())
            _scene->update();
        else
        {
            log_i("Вихід з гри");

            if (_is_client)
                _client.disconnect();
            else if (_is_server)
                _server.stop();

            delete _scene;

            showMainMenu();
        }
    }

} // namespace test_server
