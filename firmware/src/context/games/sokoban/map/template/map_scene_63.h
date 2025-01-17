#pragma once
#include <Arduino.h>

const uint16_t MAP_SCENE_63[] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1,
    1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1,
    1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1,
    1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1,
    1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1,
    1, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1,
    1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1,
    1, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1,
    1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1,
    1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1,
    1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1,
    1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1,
    1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

const uint16_t POINT_POS_63[][2] = {
    {7 * 32, 5 * 32},
    {7 * 32, 6 * 32},
    {8 * 32, 6 * 32},
    {7 * 32, 7 * 32},
    {7 * 32, 8 * 32},
    {8 * 32, 8 * 32},
    {7 * 32, 9 * 32},
    {8 * 32, 9 * 32},
    {7 * 32, 11 * 32},
    {8 * 32, 11 * 32},
    {7 * 32, 12 * 32},
};

const uint16_t BOX_POS_63[][2] = {
    {5 * 32, 2 * 32},
    {9 * 32, 2 * 32},
    {10 * 32, 3 * 32},
    {5 * 32, 8 * 32},
    {10 * 32, 8 * 32},
    {4 * 32, 9 * 32},
    {4 * 32, 11 * 32},
    {10 * 32, 11 * 32},
    {10 * 32, 15 * 32},
    {2 * 32, 16 * 32},
    {9 * 32, 16 * 32},
};