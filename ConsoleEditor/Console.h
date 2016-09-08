#pragma once
#include <windows.h>
#include <stdio.h>
#include <string>

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

class Console
{
public:
    Console();
    Console(HANDLE h_console_screen_buff);
    ~Console();

    void deactivate();

    void MoveCursorToXY(int column, int line);
    void MoveCursor(DIRECTION direction, int distance);
    
    HANDLE h_screen_buff() const { return m_handle_screen_buff; }
    void h_screen_buff(HANDLE h_screen_buff) { m_handle_screen_buff = h_screen_buff; }

    void get_char_buffer(CHAR_INFO *char_buff_out);

    void size(COORD screen_size);
    void size(int width, int height);

    int width() const { return m_width; }
    int height() const { return m_height; }

    int cursor_Y() const { return m_cursor_Y; }
    void cursor_Y(int y_position) { m_cursor_Y = y_position; }

    bool line_numbers_on() const { return m_line_numbers_on; }
    void line_numbers_on(int bool_on) { m_line_numbers_on = bool_on; }

private:
    HANDLE m_handle_screen_buff;
    EDITOR_MODE editor_mode = NORMAL_MODE;
    bool m_line_numbers_on;
    CHAR_INFO m_char_buffer;
    int m_cursor_X;
    int m_cursor_Y;
    int m_width;
    int m_height;
};

