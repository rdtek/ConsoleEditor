#pragma once
#include "AppCommon.h"
#include <iterator>     // std::back_inserter
#include <vector>       // std::vector
#include <algorithm>    // std::copy
#include <stdio.h>
#include <string>
#include "FileTools.h"
#include "CharItem.h"
#include "CharLine.h"
#include "CharInfoBuffer.h"
#include "ConsoleColors.h"

using namespace std;

class ConsoleBuffer
{
public:
    ConsoleBuffer();
    ~ConsoleBuffer();

    EDITOR_MODE editor_mode() const { return m_editor_mode; }
    void editor_mode(EDITOR_MODE mode) { m_editor_mode = mode; }

    HANDLE h_screen_buff() const { return m_handle_screen_buff; }
    void h_screen_buff(HANDLE h_screen_buff) { m_handle_screen_buff = h_screen_buff; }

    void ConsoleBuffer::load_file(const string& filename);
    void load_buffer(HANDLE h_source_buffer);

    bool line_numbers_on() const { return m_line_numbers_on; }
    void line_numbers_on(int bool_on) { m_line_numbers_on = bool_on; }

    /* char_buffer_array: read the char_info items into the output array. */
    void char_buffer_array(CHAR_INFO* char_info_buff_out);

    void build_line_num_string(size_t line_num, string& str_line_num_out);
    void build_line_content_string(size_t& idx_model_char_start, size_t content_length, string& str_content_out);

    int length();
    void render(CONSOLE_SCREEN_BUFFER_INFO screen_info);
    void display(CONSOLE_SCREEN_BUFFER_INFO screen_info);

    void refresh_cursor();
    void move_cursor(DIRECTION direction, int distance);
    void move_cursor_to(int column, int line, HANDLE h_screen_buff);

    int cursor_X() const { return m_cursor_X; }
    void cursor_X(int x_position) { m_cursor_X = x_position; }

    int cursor_Y() const { return m_cursor_Y; }
    void cursor_Y(int y_position) { m_cursor_Y = y_position; }

    void size(COORD screen_size);
    void size(int width, int height);

    int width();
    int height();

protected:
    EDITOR_MODE         m_editor_mode = NORMAL_MODE;
    bool                m_line_numbers_on;
    HANDLE              m_handle_screen_buff;
    vector<CharLine>    m_model_char_lines;
    vector<CharItem>    m_model_char_items;
    CharInfoBuffer      m_view_char_info_buff;
    size_t              m_view_top_left_index;
    int                 m_cursor_X;
    int                 m_cursor_Y;

    void read_char_line(const string& str_line, CharLine& char_line);

    void read_char_line
        (CHAR_INFO* char_info_arr, size_t idx_start, size_t size_total_chars, CharLine& char_line, size_t size_line);
    
    void read_char_lines
        (CHAR_INFO* char_info_arr, size_t size_total_chars, size_t size_line, size_t max_lines);
};

