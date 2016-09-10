#include "ConsoleBufferModel.h"

/* ConsoleModelBuffer: constructor. */
// ConsoleModelBuffer::ConsoleModelBuffer() {}

//Console - Console object constructor, takes handle to console screen buffer
//          Saves the state of the console so it can be retrieved.
void ConsoleBufferModel::copy_from_buffer(HANDLE h_source_buffer)
{
    SMALL_RECT rect_src_read_area;
    SMALL_RECT rect_dest_write_area;
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
    rect_src_read_area.Top       = src_screen_info.dwCursorPosition.Y - 1001;
    rect_src_read_area.Left      = 0;
    rect_src_read_area.Bottom    = src_screen_info.dwSize.Y - 1;
    rect_src_read_area.Right     = src_screen_info.dwSize.X - 1;
    if (rect_src_read_area.Top < 0) rect_src_read_area.Top = 0;
    if (rect_src_read_area.Bottom < 0) rect_src_read_area.Bottom = 0;

    // The temporary buffer size
    coord_ch_temp_buff_size.Y = 100;
    coord_ch_temp_buff_size.X = rect_src_read_area.Right + 1;

    // The top left destination cell of the temporary buffer is 
    coord_start_temp_buff.X = 0;
    coord_start_temp_buff.Y = 0;

    // Copy the block from the screen buffer to the temp. buffer. 
    func_success = ReadConsoleOutput(
        h_source_buffer,          // screen buffer to read from 
        ch_info_temp_buff,        // buffer to copy into 
        coord_ch_temp_buff_size,  // col-row size of ch_info_buff 
        coord_start_temp_buff,    // top left dest. cell in ch_info_buff 
        &rect_src_read_area           // screen buffer source rectangle 
    );         

    if (!func_success) {
        printf("ReadConsoleOutput failed - (%d)\n", GetLastError());
        return;
    }

    // Set the destination rectangle. 
    rect_dest_write_area.Top     = 0;     // top lt: row 10, col 0 
    rect_dest_write_area.Left    = 0;
    rect_dest_write_area.Bottom  = rect_src_read_area.Bottom; // bot. rt: row 11, col 79 
    rect_dest_write_area.Right   = rect_src_read_area.Right;

    // Copy from the temporary buffer to the new screen buffer. 
    func_success = WriteConsoleOutput(
        m_handle_screen_buff,    // screen buffer to write to 
        ch_info_temp_buff,       // buffer to copy from 
        coord_ch_temp_buff_size, // col-row size of ch_info_temp_buff 
        coord_start_temp_buff,   // top left src cell in ch_info_temp_buff 
        &rect_dest_write_area         // dest. screen buffer rectangle 
    );  

    if (!func_success) {
        printf("WriteConsoleOutput failed - (%d)\n", GetLastError());
        return;
    }
}

/* get_char_buffer: read the char_info items into the output array. */
void ConsoleBufferModel::get_char_buffer(CHAR_INFO *char_info_buff_out) {
    
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
void ConsoleBufferModel::size(COORD screen_size) {
    if (SetConsoleScreenBufferSize(m_handle_screen_buff, screen_size)) {
        m_width = screen_size.X;
        m_height = screen_size.Y;
    };
}

/* size: set the screen size for this console. */
void ConsoleBufferModel::size(int width, int height) {
    COORD screen_size;
    screen_size.X = width;
    screen_size.Y = height;
    size(screen_size);
}

/* refresh_cursor: cause the cursor to be shown in current position. */
void ConsoleBufferModel::refresh_cursor() {
    if (cursor_X() >= 0 && cursor_Y() >= 0) {
        move_cursor_to(cursor_X(), cursor_Y());
    }
}

/* move_cursor_to: move the cursor to specified column and line number. */
void ConsoleBufferModel::move_cursor_to(int column, int line)
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
void ConsoleBufferModel::move_cursor(DIRECTION direction, int distance) {

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

/* ~ConsoleModelBuffer: destructor. */
ConsoleBufferModel::~ConsoleBufferModel()
{
}
