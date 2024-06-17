#pragma once
#include <Arduino.h>

// 35, // Height of image
// 35, // Width of image

const unsigned short SETTINGS_IMG[1225] = {0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
                                           0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
                                           0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
                                           0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
                                           0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
                                           0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
                                           0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf81f, 0xf81f, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
                                           0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf81f, 0xf81f, 0xf81f, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
                                           0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf81f, 0xf81f, 0xf81f, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf960, 0xf960, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
                                           0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf960, 0xf960, 0xf960, 0xf960, 0xf81f, 0xf81f, 0xf81f, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf960, 0xf960, 0xf960, 0xf960, 0xf81f, 0xf81f, 0xf81f,
                                           0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf960, 0xf960, 0xf960, 0xf960, 0xf81f, 0xf81f, 0xf81f, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf81f, 0xf81f, 0xf81f,
                                           0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf960, 0xf960, 0xf960, 0xf960, 0xf81f, 0xf81f, 0xf81f, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf81f, 0xf81f, 0xf81f,
                                           0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf960, 0xf960, 0xf960, 0xf960, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf81f, 0xf81f, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf81f, 0xf81f, 0xf81f,
                                           0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf960, 0xf960, 0xf960, 0xf960, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf81f, 0xf81f, 0xf81f,
                                           0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf960, 0xf960, 0xf960, 0xf960, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf81f, 0xf960, 0xf960, 0xf960, 0xf81f, 0xf81f, 0xf81f,
                                           0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf960, 0xf960, 0xf960, 0xf960, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf81f, 0xf81f, 0xf960, 0xf960, 0xf960, 0xf81f, 0xf81f, 0xf81f,
                                           0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf960, 0xf960, 0xf960, 0xf960, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf960, 0xf960, 0xf960, 0xf960, 0xf81f, 0xf81f, 0xf960, 0xf960, 0xf960, 0xf960, 0xf81f, 0xf81f, 0xf81f,
                                           0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf960, 0xf960, 0xf960, 0xf960, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf960, 0xf960, 0xf81f, 0xf81f, 0xf81f, 0xf960, 0xf960, 0xf960, 0xf960, 0xf81f, 0xf81f, 0xf81f,
                                           0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf960, 0xf960, 0xf960, 0xf960, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
                                           0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
                                           0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
                                           0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
                                           0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
                                           0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
                                           0xf81f, 0xf81f, 0xf81f, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
                                           0xf81f, 0xf81f, 0xf81f, 0xf960, 0xf960, 0xf960, 0xf960, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
                                           0xf81f, 0xf81f, 0xf81f, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf81f, 0xf81f, 0xf81f, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
                                           0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf81f, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
                                           0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
                                           0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
                                           0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf960, 0xf960, 0xf960, 0xf960, 0xf960, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
                                           0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf960, 0xf960, 0xf960, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
                                           0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
                                           0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
                                           0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f};