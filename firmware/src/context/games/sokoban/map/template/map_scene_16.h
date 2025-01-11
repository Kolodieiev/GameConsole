#pragma once
#include <Arduino.h>

const uint16_t MAP_SCENE_16[] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1,
    1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1,
    1, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1,
    1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1,
    1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1,
    1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

// --------------------------------- 16
const uint16_t POINT_POS_16[][2] = {
    {1 * 32, 1 * 32},
    {2 * 32, 1 * 32},
    {1 * 32, 2 * 32},
    {2 * 32, 2 * 32},
    {1 * 32, 3 * 32},
    {2 * 32, 3 * 32},
    {1 * 32, 4 * 32},
    {2 * 32, 4 * 32},
    {1 * 32, 5 * 32},
    {2 * 32, 5 * 32},
};

const uint16_t BOX_POS_16[][2] = {
    {8 * 32, 3 * 32},
    {9 * 32, 3 * 32},
    {9 * 32, 4 * 32},
    {9 * 32, 5 * 32},
    {5 * 32, 6 * 32},
    {8 * 32, 6 * 32},
    {6 * 32, 7 * 32},
    {9 * 32, 7 * 32},
    {5 * 32, 8 * 32},
    {8 * 32, 8 * 32},
};