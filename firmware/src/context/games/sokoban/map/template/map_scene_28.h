#pragma once
#include <Arduino.h>

const uint16_t MAP_SCENE_28[] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1,
    1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1,
    1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1,
    1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1,
    1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1,
    1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

const uint16_t POINT_POS_28[][2] = {
    {9 * 32, 1 * 32},
    {9 * 32, 2 * 32},
    {10 * 32, 3 * 32},
    {11 * 32, 3 * 32},
    {12 * 32, 3 * 32},
    {6 * 32, 4 * 32},
    {7 * 32, 4 * 32},
    {8 * 32, 4 * 32},
    {6 * 32, 5 * 32},
    {7 * 32, 5 * 32},
    {8 * 32, 5 * 32},
    {6 * 32, 6 * 32},
    {7 * 32, 6 * 32},
};

const uint16_t BOX_POS_28[][2] = {
    {2 * 32, 5 * 32},
    {13 * 32, 5 * 32},
    {18 * 32, 5 * 32},
    {4 * 32, 7 * 32},
    {8 * 32, 7 * 32},
    {11 * 32, 7 * 32},
    {13 * 32, 8 * 32},
    {15 * 32, 8 * 32},
    {16 * 32, 8 * 32},
    {5 * 32, 9 * 32},
    {6 * 32, 9 * 32},
    {12 * 32, 9 * 32},
    {16 * 32, 9 * 32},
};
