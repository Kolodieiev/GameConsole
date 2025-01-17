#pragma once
#include <Arduino.h>

const uint16_t MAP_SCENE_12[] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1,
    1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1,
    1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1,
    1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1,
    1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

// --------------------------------- 12
const uint16_t POINT_POS_12[][2] = {
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

const uint16_t BOX_POS_12[][2] = {
    {7 * 32, 2 * 32},
    {10 * 32, 2 * 32},
    {6 * 32, 3 * 32},
    {10 * 32, 5 * 32},
    {9 * 32, 6 * 32},
    {11 * 32, 6 * 32},
    {4 * 32, 7 * 32},
    {7 * 32, 7 * 32},
    {9 * 32, 7 * 32},
    {11 * 32, 7 * 32},
};
