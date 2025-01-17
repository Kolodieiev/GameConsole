#include "BoxObj.h"
#include "../ClassID.h"

#include "../../../common_res/box_img/sprite_box.h"
#include "../../../common_res/box_img/sprite_box_ok.h"

#include "../../ResID.h"

namespace sokoban
{
    void BoxObj::init()
    {
        _obj_id = IdGen::genID();
        _class_ID = ClassID::CLASS_BOX;
        _layer = 1; // Об'єкт повинен бути вище об'єктів точок
        _sprite.img_ptr = SPRITE_BOX;
        _sprite.has_img = true;
        _sprite.width = 32;
        _sprite.height = 32;
        _sprite.pass_abillity_mask = Tile::TYPE_GROUND;

        initSprite();
    }

    void BoxObj::update()
    {
        _is_ok = false; // Необхідно на випадок, якщо ящик було переміщено із ключової точки
        _sprite.img_ptr = SPRITE_BOX;

        std::list<IGameObject *> objs = getObjInPoint(_x_global, _y_global); // Отримати список усіх об'єктів у точці

        for (auto it = objs.begin(), last_it = objs.end(); it != last_it; ++it)
        {
            if ((*it)->getClassID() == ClassID::CLASS_BOX_POINT) // Якщо об'єкт належить до типу BoxPointObj
            {
                _is_ok = true;                   // Підняти прапор, який вказує, що ящик встановлено в потрібному місці
                _sprite.img_ptr = SPRITE_BOX_OK; // Змінити спрайт об'єкта
                break;
            }
        }
    }

    void BoxObj::serialize(DataStream &ds)
    {
    }

    void BoxObj::deserialize(DataStream &ds)
    {
    }

    size_t BoxObj::getDataSize() const
    {
        return 0;
    }

    bool BoxObj::moveTo(uint16_t x, uint16_t y)
    {
        std::list<IGameObject *> objs = getObjInPoint(x, y); // Вибрати всі об'єкти на плитці куди повинен бути встановлений ящик
        for (auto it = objs.begin(), last_it = objs.end(); it != last_it; ++it)
        {
            if ((*it)->getClassID() == ClassID::CLASS_BOX) // Якщо знайдено об'єкт ящика, рух продовжувати не можна
                return false;
        }

        // Якщо ящик не знайдено, перевірити, чи не вприємося в стіну за ящиком
        if (!_terrain.canPass(_x_global, _y_global, x, y, _sprite))
        {
            // Якщо впираємося в стіну, рух продовжувати не можна
            return false;
        }

        // Якщо всі перевірки пройдено
        _x_global = x; //  переміщуємо об'єкт ящика
        _y_global = y;

        // Відтворення звуку по його ідентфікатору

        // int start = 0;
        // int end = 4;

        // // Отримати випадковий звук переміщення
        // uint8_t track_ID = rand() % (end - start + 1) + start;
        // WavData *s_data = _res_manager.getWav(track_ID);

        // if (s_data && s_data->size > 0)
        // {
        //     WavTrack *track = new WavTrack(s_data->data_ptr, s_data->size);
        //     track->setVolume(4);

        //     if (!track)
        //     {
        //         log_e("Помилка виділення пам'яті");
        //         esp_restart();
        //     }

        //     _audio.addToMix(track);
        // }
        // else
        // {
        //     log_e("Звукові дані з таким ID{%d} відсутні", track_ID);
        // }

        return true;
    }
}