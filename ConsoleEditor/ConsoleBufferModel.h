#pragma once
#include "AppCommon.h"
#include <stdio.h>
#include <string>
#include "ConsoleBufferBase.h"

using namespace std;

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

class ConsoleBufferModel : public ConsoleBufferBase
{
public:
    ConsoleBufferModel() : ConsoleBufferBase() {};
    ~ConsoleBufferModel();

    void copy_from_buffer(HANDLE h_source_buffer);

    void refresh_cursor();
    void move_cursor_to(int column, int line);
    void move_cursor(DIRECTION direction, int distance);
    
    HANDLE h_screen_buff() const { return m_handle_screen_buff; }
    void h_screen_buff(HANDLE h_screen_buff) { m_handle_screen_buff = h_screen_buff; }

    void size(COORD screen_size);
    void size(int width, int height);

    int width() const { return m_width; }
    int height() const { return m_height; }

    int cursor_X() const { return m_cursor_X; }
    void cursor_X(int x_position) { m_cursor_X = x_position; }

    int cursor_Y() const { return m_cursor_Y; }
    void cursor_Y(int y_position) { m_cursor_Y = y_position; }

    bool line_numbers_on() const { return m_line_numbers_on; }
    void line_numbers_on(int bool_on) { m_line_numbers_on = bool_on; }

private:
    EDITOR_MODE editor_mode = NORMAL_MODE;
    bool m_line_numbers_on;
    int m_cursor_X;
    int m_cursor_Y;
};

