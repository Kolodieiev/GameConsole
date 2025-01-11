#pragma once
#include <Arduino.h>

const uint16_t MAP_SCENE_48[] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
    1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1,
    1, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1,
    1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1,
    1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1,
    1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1,
    1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1,
    1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1,
    1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1,
    1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1,
    1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
    1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

const uint16_t POINT_POS_48[][2] = {
    {2 * 32, 1 * 32},
    {3 * 32, 1 * 32},
    {4 * 32, 1 * 32},
    {5 * 32, 1 * 32},
    {2 * 32, 2 * 32},
    {4 * 32, 2 * 32},
    {2 * 32, 3 * 32},
    {3 * 32, 3 * 32},
    {4 * 32, 3 * 32},
    {5 * 32, 3 * 32},
    {2 * 32, 4 * 32},
    {3 * 32, 4 * 32},
    {4 * 32, 4 * 32},
    {5 * 32, 4 * 32},
};

const uint16_t BOX_POS_48[][2] = {
    {8 * 32, 2 * 32},
    {7 * 32, 5 * 32},
    {11 * 32, 5 * 32},
    {8 * 32, 6 * 32},
    {2 * 32, 7 * 32},
    {5 * 32, 7 * 32},
    {7 * 32, 7 * 32},
    {9 * 32, 7 * 32},
    {6 * 32, 8 * 32},
    {8 * 32, 8 * 32},
    {4 * 32, 11 * 32},
    {9 * 32, 11 * 32},
    {4 * 32, 12 * 32},
    {6 * 32, 12 * 32},
};