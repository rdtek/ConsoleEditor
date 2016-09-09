#include "ConsoleBuffer.h"

ConsoleBuffer::ConsoleBuffer()
{
    init_buff();
}

int ConsoleBuffer::init_buff() {
    
    m_handle_screen_buff = CreateConsoleScreenBuffer(
        GENERIC_READ |           // read/write access 
        GENERIC_WRITE,
        FILE_SHARE_READ |
        FILE_SHARE_WRITE,        // shared 
        NULL,                    // default security attributes 
        CONSOLE_TEXTMODE_BUFFER, // must be TEXTMODE 
        NULL);                   // reserved; must be NULL 

    if (m_handle_screen_buff == INVALID_HANDLE_VALUE) return 1;

    return 0;
}

//Console - Console object constructor, takes handle to console screen buffer
//          Saves the state of the console so it can be retrieved.
void ConsoleBuffer::copy_buffer(HANDLE h_source_buffer)
{
    SMALL_RECT read_rect;
    SMALL_RECT rect_write_area;
    CHAR_INFO ch_info_temp_buff[100000]; // [1000][100]; 1000 lines * 100 characters 
    COORD coord_ch_temp_buff_size;
    COORD coord_start_temp_buff;
    CONSOLE_SCREEN_BUFFER_INFO src_screen_info;
    BOOL func_success;

    // Check source buffer is valid
    if (h_source_buffer == INVALID_HANDLE_VALUE) {
        printf("CreateConsoleScreenBuffer failed - (%d)\n", GetLastError());
        return;
    }

    // Get the curson position from the source buffer
    GetConsoleScreenBufferInfo(h_source_buffer, &src_screen_info);
    m_cursor_X = src_screen_info.dwCursorPosition.X;
    m_cursor_Y = src_screen_info.dwCursorPosition.Y;

    size(src_screen_info.dwSize);

    // Set rectangle defining portion of source buffer to copy
    read_rect.Top = src_screen_info.dwCursorPosition.Y - 1001;
    if (read_rect.Top < 0) read_rect.Top = 0;
    read_rect.Left = 0;
    //read_rect.Bottom = src_screen_info.dwCursorPosition.Y; // bot. right: row 1, col 79 
    read_rect.Bottom = src_screen_info.dwSize.Y - 1;
    //if (read_rect.Bottom <= 0) src_screen_info.dwSize.Y;
    read_rect.Right = src_screen_info.dwSize.X - 1;

    // The temporary buffer size
    coord_ch_temp_buff_size.Y = 100;
    coord_ch_temp_buff_size.X = read_rect.Right + 1;

    // The top left destination cell of the temporary buffer is 
    coord_start_temp_buff.X = 0;
    coord_start_temp_buff.Y = 0;

    // Copy the block from the screen buffer to the temp. buffer. 
    func_success = ReadConsoleOutput(
        h_source_buffer,          // screen buffer to read from 
        ch_info_temp_buff,        // buffer to copy into 
        coord_ch_temp_buff_size,  // col-row size of ch_info_buff 
        coord_start_temp_buff,    // top left dest. cell in ch_info_buff 
        &read_rect);              // screen buffer source rectangle 

    if (!func_success) {
        printf("ReadConsoleOutput failed - (%d)\n", GetLastError());
        return;
    }

    // Set the destination rectangle. 
    rect_write_area.Top = 0;     // top lt: row 10, col 0 
    rect_write_area.Left = 0;
    rect_write_area.Bottom = read_rect.Bottom; // bot. rt: row 11, col 79 
    rect_write_area.Right = read_rect.Right;

    // Copy from the temporary buffer to the new screen buffer. 
    func_success = WriteConsoleOutput(
        m_handle_screen_buff,    // screen buffer to write to 
        ch_info_temp_buff,       // buffer to copy from 
        coord_ch_temp_buff_size, // col-row size of ch_info_temp_buff 
        coord_start_temp_buff,   // top left src cell in ch_info_temp_buff 
        &rect_write_area         // dest. screen buffer rectangle 
    );  

    if (!func_success) {
        printf("WriteConsoleOutput failed - (%d)\n", GetLastError());
        return;
    }
}

/* get_char_buffer: read the char_info items into the output array. */
void ConsoleBuffer::get_char_buffer(CHAR_INFO *char_info_buff_out) {
    
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

/* size: set the screen size for this console. */
void ConsoleBuffer::size(COORD screen_size) {
    if (SetConsoleScreenBufferSize(m_handle_screen_buff, screen_size)) {
        m_width = screen_size.X;
        m_height = screen_size.Y;
    };
}

/* size: set the screen size for this console. */
void ConsoleBuffer::size(int width, int height) {
    COORD screen_size;
    screen_size.X = width;
    screen_size.Y = height;
    size(screen_size);
}

/* move_cursor_to: move the cursor to specified column and line number. */
void ConsoleBuffer::move_cursor_to(int column, int line)
{
    COORD coord;
    m_cursor_X = coord.X = column;
    m_cursor_Y = coord.Y = line;

    // Finally, call the SetConsoleCursorPosition function.
    if (!SetConsoleCursorPosition(m_handle_screen_buff, coord))
    {
        // TODO: Call GetLastError() to get a more specific error code.
        printf("Error setting cursor position.");
    }
}

/* move_cursor: move the cursor in specified direction by specified amount. */
void ConsoleBuffer::move_cursor(DIRECTION direction, int distance) {

    int cx = m_cursor_X;
    int cy = m_cursor_Y;
    int cols = m_width;
    int rows = m_height;

    switch (direction) {
    case LEFT:
        if (editor_mode == NORMAL_MODE && cx >= 2 && m_line_numbers_on
            || cx >= 1 && !m_line_numbers_on) {
            move_cursor_to(--m_cursor_X, m_cursor_Y);
        }
        break;
    case UP:
        if (editor_mode == NORMAL_MODE && cy >= 1) {
            move_cursor_to(m_cursor_X, --m_cursor_Y);
        }
        break;
    case RIGHT:
        if (editor_mode == NORMAL_MODE && cx < (cols - 1) && m_line_numbers_on) {
            move_cursor_to(++m_cursor_X, m_cursor_Y);
        }
        break;
    case DOWN:
        if (editor_mode == NORMAL_MODE && (cy < rows - 1)) {
            move_cursor_to(m_cursor_X, ++m_cursor_Y);
        }
        break;

    default: break;
    }
}

ConsoleBuffer::~ConsoleBuffer()
{
}
