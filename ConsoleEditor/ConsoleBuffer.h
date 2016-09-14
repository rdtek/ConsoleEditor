#pragma once
#include "AppCommon.h"
#include <vector>
#include <stdio.h>
#include <string>
#include "FileTools.h"
#include "CharInfoBuffer.h"
#include "ConsoleColors.h"

using namespace std;

class ConsoleBuffer
{
public:
    ConsoleBuffer();
    ~ConsoleBuffer();

    HANDLE h_screen_buff() const { return m_handle_screen_buff; }
    void h_screen_buff(HANDLE h_screen_buff) { m_handle_screen_buff = h_screen_buff; }

    void ConsoleBuffer::load_file(const string& filename);
    void load_buffer(HANDLE h_source_buffer);

    bool line_numbers_on() const { return m_line_numbers_on; }
    void line_numbers_on(int bool_on) { m_line_numbers_on = bool_on; }

    /* get_char_buffer: read the char_info items into the output array. */
    void char_buffer_array(CHAR_INFO *char_info_buff_out);
    void insert_line_num(size_t line_num, size_t *line_num_size_out);
    void insert_line_chars(size_t num_chars_this_line);
    void append(string& str_content);

    int length();
    void render(CONSOLE_SCREEN_BUFFER_INFO screen_info);
    void display(CONSOLE_SCREEN_BUFFER_INFO screen_info, CHAR_INFO* ptr_char_info_arr);

    void refresh_cursor();
    void move_cursor(DIRECTION direction, int distance);
    void ConsoleBuffer::move_cursor_to(int column, int line, HANDLE h_screen_buff);

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
    CharInfoBuffer m_model_char_info_buff;
    CharInfoBuffer m_view_char_info_buff;
    int m_cursor_X;
    int m_cursor_Y;
};

