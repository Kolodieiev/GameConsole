#pragma once
#include <Arduino.h>

const uint16_t MAP_SCENE_25[] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1,
    1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1,
    1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
    1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1,
    1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1,
    1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1,
    1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1,
    1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

const uint16_t POINT_POS_25[][2] = {
    {5 * 32, 9 * 32},
    {6 * 32, 9 * 32},
    {7 * 32, 9 * 32},
    {8 * 32, 9 * 32},
    {9 * 32, 9 * 32},
    {5 * 32, 10 * 32},
    {6 * 32, 10 * 32},
    {7 * 32, 10 * 32},
    {9 * 32, 10 * 32},
    {5 * 32, 11 * 32},
    {6 * 32, 11 * 32},
    {7 * 32, 11 * 32},
    {8 * 32, 11 * 32},
    {9 * 32, 11 * 32},
};

const uint16_t BOX_POS_25[][2] = {
    {6 * 32, 3 * 32},
    {8 * 32, 3 * 32},
    {6 * 32, 4 * 32},
    {4 * 32, 5 * 32},
    {9 * 32, 5 * 32},
    {8 * 32, 6 * 32},
    {12 * 32, 6 * 32},
    {11 * 32, 7 * 32},
    {8 * 32, 8 * 32},
    {3 * 32, 9 * 32},
    {4 * 32, 10 * 32},
    {11 * 32, 10 * 32},
    {2 * 32, 13 * 32},
    {3 * 32, 13 * 32},
};
