#pragma once
#include <Arduino.h>

const uint16_t MAP_SCENE_49[] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1,
    1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1,
    1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1,
    1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1,
    1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
    1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1,
    1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

const uint16_t POINT_POS_49[][2] = {
    {15 * 32, 1 * 32},
    {14 * 32, 2 * 32},
    {15 * 32, 2 * 32},
    {16 * 32, 2 * 32},
    {17 * 32, 2 * 32},
    {14 * 32, 3 * 32},
    {15 * 32, 3 * 32},
    {16 * 32, 3 * 32},
    {17 * 32, 3 * 32},
    {14 * 32, 4 * 32},
    {15 * 32, 4 * 32},
    {16 * 32, 4 * 32},
    {17 * 32, 4 * 32},
    {14 * 32, 5 * 32},
    {15 * 32, 5 * 32},
    {16 * 32, 5 * 32},
    {17 * 32, 5 * 32},
    {14 * 32, 6 * 32},
    {15 * 32, 6 * 32},
    {16 * 32, 6 * 32},
    {17 * 32, 6 * 32},
    {14 * 32, 7 * 32},
    {15 * 32, 7 * 32},
    {16 * 32, 7 * 32},
    {17 * 32, 7 * 32},
};

const uint16_t BOX_POS_49[][2] = {
    {3 * 32, 5 * 32},
    {4 * 32, 5 * 32},
    {9 * 32, 5 * 32},
    {7 * 32, 6 * 32},
    {8 * 32, 6 * 32},
    {10 * 32, 6 * 32},
    {3 * 32, 7 * 32},
    {8 * 32, 7 * 32},
    {9 * 32, 7 * 32},
    {3 * 32, 8 * 32},
    {8 * 32, 8 * 32},
    {5 * 32, 10 * 32},
    {8 * 32, 10 * 32},
    {2 * 32, 11 * 32},
    {4 * 32, 12 * 32},
    {3 * 32, 13 * 32},
    {6 * 32, 13 * 32},
    {8 * 32, 13 * 32},
    {10 * 32, 13 * 32},
    {2 * 32, 14 * 32},
    {3 * 32, 14 * 32},
    {4 * 32, 14 * 32},
    {7 * 32, 14 * 32},
    {8 * 32, 15 * 32},
    {9 * 32, 15 * 32},
};