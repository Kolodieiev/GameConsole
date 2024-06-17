#pragma once
#include <Arduino.h>

const uint16_t MAP_SCENE_24[] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
    1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1,
    1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1,
    1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1,
    1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1,
    1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1,
    1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

const uint16_t POINT_POS_24[][2] = {
    {13 * 32, 5 * 32},
    {14 * 32, 5 * 32},
    {15 * 32, 5 * 32},
    {13 * 32, 6 * 32},
    {14 * 32, 6 * 32},
    {15 * 32, 6 * 32},
    {13 * 32, 7 * 32},
    {14 * 32, 7 * 32},
    {15 * 32, 7 * 32},
    {13 * 32, 8 * 32},
    {14 * 32, 8 * 32},
    {15 * 32, 8 * 32},
    {13 * 32, 9 * 32},
    {14 * 32, 9 * 32},
    {15 * 32, 9 * 32},
};

const uint16_t BOX_POS_24[][2] = {
    {5 * 32, 3 * 32},
    {7 * 32, 3 * 32},
    {9 * 32, 3 * 32},
    {11 * 32, 3 * 32},
    {6 * 32, 4 * 32},
    {8 * 32, 4 * 32},
    {5 * 32, 5 * 32},
    {7 * 32, 5 * 32},
    {9 * 32, 5 * 32},
    {6 * 32, 6 * 32},
    {8 * 32, 6 * 32},
    {5 * 32, 7 * 32},
    {6 * 32, 7 * 32},
    {7 * 32, 7 * 32},
    {9 * 32, 7 * 32},
};