#pragma once
#include <Arduino.h>

const uint16_t MAP_SCENE_0_2[] = {1, 1, 1, 1, 1, 1, 1, 1,
                                 1, 1, 1, 0, 0, 0, 1, 1,
                                 1, 0, 0, 0, 1, 0, 1, 1,
                                 1, 0, 1, 0, 0, 0, 0, 1,
                                 1, 0, 0, 0, 0, 1, 0, 1,
                                 1, 1, 0, 1, 0, 0, 0, 1,
                                 1, 1, 0, 0, 0, 1, 1, 1,
                                 1, 1, 1, 1, 1, 1, 1, 1,
                                 1, 1, 1, 1, 1, 1, 1, 1,
                                 1, 1, 1, 1, 1, 1, 1, 1};

// --------------------------------- 0
const uint16_t BOX_POS_0[][2] =
    {
        {2 * 32, 2 * 32},
};

const uint16_t POINT_POS_0[][2] =
    {
        {5 * 32, 3 * 32},
};

// --------------------------------- 1
const uint16_t BOX_POS_1[][2] =
    {
        {2 * 32, 2 * 32},
        {2 * 32, 5 * 32},
};

const uint16_t POINT_POS_1[][2] =
    {
        {5 * 32, 3 * 32},
        {4 * 32, 5 * 32},
};

// --------------------------------- 2
const uint16_t BOX_POS_2[][2] =
    {
        {2 * 32, 2 * 32},
        {2 * 32, 5 * 32},
        {5 * 32, 5 * 32},
};

const uint16_t POINT_POS_2[][2] =
    {
        {5 * 32, 3 * 32},
        {2 * 32, 4 * 32},
        {4 * 32, 5 * 32},
};