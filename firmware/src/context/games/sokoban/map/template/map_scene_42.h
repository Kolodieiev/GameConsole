#pragma once
#include <Arduino.h>

const uint16_t MAP_SCENE_42[] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1,
    1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
    1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1,
    1, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1,
    1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1,
    1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1,
    1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

const uint16_t POINT_POS_42[][2] = {
    {5 * 32, 10 * 32},
    {6 * 32, 10 * 32},
    {7 * 32, 10 * 32},
    {8 * 32, 10 * 32},
    {5 * 32, 11 * 32},
    {6 * 32, 11 * 32},
    {7 * 32, 11 * 32},
    {8 * 32, 11 * 32},
    {9 * 32, 11 * 32},
    {10 * 32, 11 * 32},
    {4 * 32, 12 * 32},
    {5 * 32, 12 * 32},
    {6 * 32, 12 * 32},
    {7 * 32, 12 * 32},
    {4 * 32, 13 * 32},
    {5 * 32, 13 * 32},
    {6 * 32, 13 * 32},
    {4 * 32, 14 * 32},
    {5 * 32, 14 * 32},
    {6 * 32, 14 * 32},
};

const uint16_t BOX_POS_42[][2] = {
    {4 * 32, 3 * 32},
    {7 * 32, 3 * 32},
    {9 * 32, 3 * 32},
    {2 * 32, 4 * 32},
    {5 * 32, 4 * 32},
    {10 * 32, 4 * 32},
    {12 * 32, 4 * 32},
    {4 * 32, 5 * 32},
    {9 * 32, 5 * 32},
    {11 * 32, 5 * 32},
    {12 * 32, 6 * 32},
    {13 * 32, 6 * 32},
    {14 * 32, 6 * 32},
    {2 * 32, 7 * 32},
    {7 * 32, 7 * 32},
    {10 * 32, 7 * 32},
    {2 * 32, 8 * 32},
    {4 * 32, 8 * 32},
    {7 * 32, 8 * 32},
    {12 * 32, 9 * 32},
};