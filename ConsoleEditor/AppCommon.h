#pragma once
#include <windows.h>
#include "logging.h"

enum DIRECTION
{
    LEFT = 0,
    UP,
    RIGHT,
    DOWN
};

enum EDITOR_MODE
{
    NORMAL_MODE = 0,
    INSERT_MODE,
    VISUAL_MODE,
    SELECT_MODE
};