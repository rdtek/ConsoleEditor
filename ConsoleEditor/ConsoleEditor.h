#pragma once
#include <windows.h>
#include <stdio.h>
#include <iostream>

enum EDITOR_MODE
{
    NORMAL_MODE = 0,
    INSERT_MODE,
    VISUAL_MODE,
    SELECT_MODE
};

enum DIRECTION
{
    LEFT = 0,
    UP,
    RIGHT,
    DOWN
};