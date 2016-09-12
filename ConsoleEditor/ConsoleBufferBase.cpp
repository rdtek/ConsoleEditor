#include "ConsoleBufferBase.h"

ConsoleBufferBase::ConsoleBufferBase() {}

ConsoleBufferBase::~ConsoleBufferBase() {}

/* size: set the screen size for this console. */
void ConsoleBufferBase::size(COORD screen_size) {
    if (SetConsoleScreenBufferSize(m_handle_screen_buff, screen_size)) {
        //m_width = screen_size.X;
        //m_height = screen_size.Y;
    };
}

/* size: set the screen size for this console. */
void ConsoleBufferBase::size(int width, int height) {
    COORD screen_size;
    screen_size.X = width;
    screen_size.Y = height;
    size(screen_size);
}

int ConsoleBufferBase::width() {
    CONSOLE_SCREEN_BUFFER_INFO  screen_info;
    GetConsoleScreenBufferInfo(m_handle_screen_buff, &screen_info);
    return screen_info.dwSize.X; 
}

int ConsoleBufferBase::height() {
    CONSOLE_SCREEN_BUFFER_INFO  screen_info;
    GetConsoleScreenBufferInfo(m_handle_screen_buff, &screen_info);
    return  screen_info.dwSize.Y;
}

void ConsoleBufferBase::char_buffer_array(CHAR_INFO *char_info_buff_out) {

    COORD coord_buff_size;
    COORD coord_destination;
    SMALL_RECT read_rect;

    read_rect.Top = 0;
    read_rect.Left = 0;
    read_rect.Right = coord_buff_size.X - 1;
    read_rect.Bottom = coord_buff_size.Y - 1;

    ReadConsoleOutput(
        m_handle_screen_buff, // screen buffer to read from 
        char_info_buff_out,   // buffer to copy into 
        coord_buff_size,      // col-row size of char_info_buff_out 
        coord_destination,    // top left destination cell in char_buff_out 
        &read_rect
    );
}

/* move_cursor_to: move the cursor to specified column and line number. */
void ConsoleBufferBase::move_cursor_to(int column, int line, HANDLE h_screen_buff)
{
    COORD coord;
    m_cursor_X = coord.X = column;
    m_cursor_Y = coord.Y = line;

    if (!SetConsoleCursorPosition(h_screen_buff, coord))
    {
        // TODO: Call GetLastError() to get a more specific error code.
        log_wstr(L"move_cursor_to(): ", L"Error setting cursor position.");
    }
}

/* refresh_cursor: cause the cursor to be shown in current position. */
void ConsoleBufferBase::refresh_cursor() {
    if (cursor_X() >= 0 && cursor_Y() >= 0) {
        move_cursor_to(cursor_X(), cursor_Y(), m_handle_screen_buff);
    }
}

/* move_cursor: move the cursor in specified direction by specified amount. */
void ConsoleBufferBase::move_cursor(DIRECTION direction, int distance) {

    int cx = m_cursor_X;
    int cy = m_cursor_Y;
    int cols = width();
    int rows = height();
    HANDLE h_screen_buff = m_handle_screen_buff;

    switch (direction) {
    case LEFT:
        if (editor_mode == NORMAL_MODE && cx >= 2 && m_line_numbers_on
            || cx >= 1 && !m_line_numbers_on) {
            move_cursor_to(--m_cursor_X, m_cursor_Y, h_screen_buff);
        }
        break;
    case UP:
        if (editor_mode == NORMAL_MODE && cy >= 1) {
            move_cursor_to(m_cursor_X, --m_cursor_Y, h_screen_buff);
        }
        break;
    case RIGHT:
        if (editor_mode == NORMAL_MODE && cx < (cols - 1) && m_line_numbers_on) {
            move_cursor_to(++m_cursor_X, m_cursor_Y, h_screen_buff);
        }
        break;
    case DOWN:
        if (editor_mode == NORMAL_MODE && (cy < rows - 1)) {
            move_cursor_to(m_cursor_X, ++m_cursor_Y, h_screen_buff);
        }
        break;

    default: break;
    }
}
