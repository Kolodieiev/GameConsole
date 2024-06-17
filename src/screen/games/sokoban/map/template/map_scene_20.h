#pragma once
#include <Arduino.h>

const uint16_t MAP_SCENE_20[] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1,
    1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};

// --------------------------------- 20
const uint16_t POINT_POS_20[][2] = {
    {15 * 32, 6 * 32},
    {16 * 32, 6 * 32},
    {17 * 32, 6 * 32},
    {18 * 32, 6 * 32},
    {15 * 32, 7 * 32},
    {16 * 32, 7 * 32},
    {17 * 32, 7 * 32},
    {18 * 32, 7 * 32},
    {19 * 32, 7 * 32},
    {15 * 32, 8 * 32},
    {16 * 32, 8 * 32},
    {17 * 32, 8 * 32},
    {18 * 32, 8 * 32},
    {19 * 32, 8 * 32},
    {15 * 32, 9 * 32},
    {16 * 32, 9 * 32},
    {17 * 32, 9 * 32},
    {18 * 32, 9 * 32},
    {19 * 32, 9 * 32},
    {15 * 32, 10 * 32},
    {16 * 32, 10 * 32},
    {17 * 32, 10 * 32},
    {18 * 32, 10 * 32},
    {19 * 32, 10 * 32},
    {15 * 32, 11 * 32},
    {16 * 32, 11 * 32},
    {17 * 32, 11 * 32},
    {18 * 32, 11 * 32},
    {19 * 32, 11 * 32},
    {15 * 32, 12 * 32},
    {16 * 32, 12 * 32},
    {17 * 32, 12 * 32},
    {18 * 32, 12 * 32},
    {19 * 32, 12 * 32},
};

const uint16_t BOX_POS_20[][2] = {
    {10 * 32, 5 * 32},
    {11 * 32, 5 * 32},
    {12 * 32, 5 * 32},
    {2 * 32, 6 * 32},
    {3 * 32, 6 * 32},
    {7 * 32, 6 * 32},
    {8 * 32, 6 * 32},
    {10 * 32, 6 * 32},
    {3 * 32, 7 * 32},
    {4 * 32, 7 * 32},
    {5 * 32, 7 * 32},
    {11 * 32, 7 * 32},
    {2 * 32, 8 * 32},
    {8 * 32, 8 * 32},
    {9 * 32, 8 * 32},
    {11 * 32, 8 * 32},
    {12 * 32, 8 * 32},
    {9 * 32, 9 * 32},
    {8 * 32, 10 * 32},
    {10 * 32, 10 * 32},
    {12 * 32, 10 * 32},
    {9 * 32, 12 * 32},
    {11 * 32, 12 * 32},
    {8 * 32, 13 * 32},
    {9 * 32, 13 * 32},
    {11 * 32, 13 * 32},
    {13 * 32, 13 * 32},
    {9 * 32, 14 * 32},
    {8 * 32, 15 * 32},
    {9 * 32, 15 * 32},
    {10 * 32, 15 * 32},
    {12 * 32, 15 * 32},
    {13 * 32, 15 * 32},
    {14 * 32, 15 * 32},
};
