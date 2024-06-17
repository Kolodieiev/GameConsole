#pragma once
#include <Arduino.h>

const uint16_t MAP_SCENE_43[] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1,
    1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
    1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1,
    1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1,
    1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1,
    1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1,
    1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1,
    1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
    1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

const uint16_t POINT_POS_43[][2] = {
    {2 * 32, 6 * 32},
    {3 * 32, 6 * 32},
    {4 * 32, 6 * 32},
    {5 * 32, 6 * 32},
    {2 * 32, 7 * 32},
    {3 * 32, 7 * 32},
    {4 * 32, 7 * 32},
    {5 * 32, 7 * 32},
    {2 * 32, 8 * 32},
    {3 * 32, 8 * 32},
    {4 * 32, 8 * 32},
    {5 * 32, 8 * 32},
    {2 * 32, 9 * 32},
    {3 * 32, 9 * 32},
    {4 * 32, 9 * 32},
};

const uint16_t BOX_POS_43[][2] = {
    {8 * 32, 2 * 32},
    {3 * 32, 3 * 32},
    {2 * 32, 4 * 32},
    {4 * 32, 4 * 32},
    {8 * 32, 4 * 32},
    {10 * 32, 5 * 32},
    {7 * 32, 6 * 32},
    {9 * 32, 6 * 32},
    {10 * 32, 7 * 32},
    {8 * 32, 8 * 32},
    {9 * 32, 8 * 32},
    {8 * 32, 9 * 32},
    {7 * 32, 10 * 32},
    {9 * 32, 10 * 32},
    {7 * 32, 12 * 32},
};
