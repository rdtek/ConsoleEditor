#pragma once
#include "AppCommon.h"
#include <vector>

using namespace std;

class ConsoleBufferBase
{
public:
    ConsoleBufferBase();
    ~ConsoleBufferBase();

    bool line_numbers_on() const { return m_line_numbers_on; }
    void line_numbers_on(int bool_on) { m_line_numbers_on = bool_on; }

    /* get_char_buffer: read the char_info items into the output array. */
    void char_buffer_array(CHAR_INFO *char_info_buff_out);
    
    int length() const { return m_vect_char_buffer.size(); }
    vector<CHAR_INFO> char_info_buffer() const { return m_vect_char_buffer; }

    void refresh_cursor();
    void move_cursor(DIRECTION direction, int distance);
    void ConsoleBufferBase::move_cursor_to(int column, int line, HANDLE h_screen_buff);

    int cursor_X() const { return m_cursor_X; }
    void cursor_X(int x_position) { m_cursor_X = x_position; }

    int cursor_Y() const { return m_cursor_Y; }
    void cursor_Y(int y_position) { m_cursor_Y = y_position; }

    void size(COORD screen_size);
    void size(int width, int height);

    int width();
    int height();

protected:
    EDITOR_MODE editor_mode = NORMAL_MODE;
    bool m_line_numbers_on;
    HANDLE m_handle_screen_buff;
    vector<CHAR_INFO> m_vect_char_buffer;
    int m_cursor_X;
    int m_cursor_Y;
};

