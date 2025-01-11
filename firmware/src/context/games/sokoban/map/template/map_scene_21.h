#pragma once
#include <Arduino.h>

const uint16_t MAP_SCENE_21[] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1,
    1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1,
    1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

const uint16_t POINT_POS_21[][2] = {
    {1 * 32, 10 * 32},
    {1 * 32, 11 * 32},
    {2 * 32, 11 * 32},
    {4 * 32, 11 * 32},
    {5 * 32, 11 * 32},
    {1 * 32, 12 * 32},
    {2 * 32, 12 * 32},
    {3 * 32, 12 * 32},
    {5 * 32, 12 * 32},
    {1 * 32, 13 * 32},
    {2 * 32, 13 * 32},
    {3 * 32, 13 * 32},
    {4 * 32, 13 * 32},
    {5 * 32, 13 * 32},
};

const uint16_t BOX_POS_21[][2] = {
    {11 * 32, 2 * 32},
    {10 * 32, 3 * 32},
    {5 * 32, 4 * 32},
    {7 * 32, 4 * 32},
    {8 * 32, 4 * 32},
    {5 * 32, 5 * 32},
    {5 * 32, 6 * 32},
    {7 * 32, 6 * 32},
    {8 * 32, 6 * 32},
    {5 * 32, 7 * 32},
    {12 * 32, 7 * 32},
    {11 * 32, 8 * 32},
    {12 * 32, 8 * 32},
    {8 * 32, 9 * 32},
};