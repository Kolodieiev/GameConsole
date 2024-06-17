#pragma once
#include <Arduino.h>

#include "meow/game/IGameScene.h"
#include "../obj/ghost/GhostObj.h"
#include "../obj/sokoban/SokobanObj.h"

using namespace meow;

class SokobanScene : public IGameScene
{

public:
    SokobanScene(GraphicsDriver &display, Input &input, std::vector<IObjShape *> &stored_objs, bool is_loaded = false, uint8_t lvl = 1);
    // Деструктор обов'язковий до реалізації.
    virtual ~SokobanScene();

    // Метод обов'язковий до реалізації. Викликається кожен кадр екраном для оновлення та відрисовки сцени.
    virtual void update() override;

    // Метод обов'язковий до реалізації. Його буде викликано, якщо будь-який об'єкт повідомить про свій тригер. В параметр буде передано id тригера
    virtual void onTriggered(int16_t id) override;

private:
    // ----------------------------------------
    uint8_t _level{0};
    GhostObj *_ghost;     // Об'єкт який дозволяє переміщувати камеру по ігровому рівню.
    SokobanObj *_sokoban; // Вказівник на персонажа. Його не можна видаляти самостійно. Тому що цей об'єкт буде додано до ігрового світу і сцена видалить його сама

    bool _is_ghost_selected{true}; // Прапор, що допомогає розпізнати, який об'єкт зараз обрано головним.

    // Завантажити ігровий рівень
    void buildMap();
    // Створити об'єкт привида
    void createGhost();
    // Створити об'єкт комірника
    void createSokoban();
    // Створити об'єкти ящиків
    void createBoxes();
    // Створити об'єкти ключових точок, на яких повинні бути встановлені ящики
    void createBoxPoints();
    // Завантажити звукові ресурси
    void loadFX();
};
