#pragma once
#ifndef GLOBALS_H
#define GLOBALS_H

// Constants
const int WIDTH = 800;
const int HEIGHT = 600;

const int GEO_INPUT_DESC_SIZE = 4;
const int LIGHT_INPUT_DESC_SIZE = 1;
const int SHADOW_INPUT_DESC_SIZE = 1;

const int NUM_DEFERRED_OUTPUTS = 4;

const int NUM_KEYS = 7;
enum KEYS { W, A, S, D, ESC, SPACE, CTRL, MB, UP, F };
enum LIGHT_TYPE { SPOTLIGHT, DIRECTLIGHT, POINTLIGHT };

#define VK_W 87
#define VK_A 65
#define VK_S 83
#define VK_D 68
#define VK_F 70

#endif
