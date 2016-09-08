#include "Console.h"

Console::Console()
{
}

void Console::deactivate() {
    //Detach the console buffer from the terminal

    //Clear the content from the console

    return;
}

//Console - Console object constructor, takes handle to console screen buffer
//          Saves the state of the console so it can be retrieved.
Console::Console(HANDLE h_std_out)
{
    SMALL_RECT read_rect;
    SMALL_RECT write_rect;
    CHAR_INFO ch_info_buff[100000]; // [1000][100]; 1000 lines * 100 characters 
    COORD coordBufSize;
    COORD coordBufCoord;
    BOOL fSuccess;
    CONSOLE_SCREEN_BUFFER_INFO original_screen_info;

    m_handle_screen_buff = CreateConsoleScreenBuffer(
        GENERIC_READ |           // read/write access 
        GENERIC_WRITE,
        FILE_SHARE_READ |
        FILE_SHARE_WRITE,        // shared 
        NULL,                    // default security attributes 
        CONSOLE_TEXTMODE_BUFFER, // must be TEXTMODE 
        NULL);                   // reserved; must be NULL 

    GetConsoleScreenBufferInfo(h_std_out, &original_screen_info);
    
    m_cursor_X = original_screen_info.dwCursorPosition.X;
    m_cursor_Y = original_screen_info.dwCursorPosition.Y;

    if (h_std_out == INVALID_HANDLE_VALUE || m_handle_screen_buff == INVALID_HANDLE_VALUE)
    {
        printf("CreateConsoleScreenBuffer failed - (%d)\n", GetLastError());
        return;
    }

    // Set the source rectangle. 
    read_rect.Top = original_screen_info.dwCursorPosition.Y - 1001;
    if (read_rect.Top < 0) read_rect.Top = 0;
    read_rect.Left = 0;
    read_rect.Bottom = original_screen_info.dwCursorPosition.Y; // bot. right: row 1, col 79 
    if (read_rect.Bottom <= 0) read_rect.Bottom = original_screen_info.dwSize.Y;
    read_rect.Right = original_screen_info.dwSize.X - 1;

    // The temporary buffer size
    coordBufSize.Y = 100;
    coordBufSize.X = read_rect.Right + 1;

    // The top left destination cell of the temporary buffer is 
    coordBufCoord.X = 0;
    coordBufCoord.Y = 0;

    // Copy the block from the screen buffer to the temp. buffer. 
    fSuccess = ReadConsoleOutput(
        h_std_out,        // screen buffer to read from 
        ch_info_buff,      // buffer to copy into 
        coordBufSize,   // col-row size of chiBuffer 
        coordBufCoord,  // top left dest. cell in chiBuffer 
        &read_rect); // screen buffer source rectangle 

    if (!fSuccess)
    {
        printf("ReadConsoleOutput failed - (%d)\n", GetLastError());
        return;
    }

    // Set the destination rectangle. 
    write_rect.Top = 0;    // top lt: row 10, col 0 
    write_rect.Left = 0;
    write_rect.Bottom = 99; // bot. rt: row 11, col 79 
    write_rect.Right = 119;

    // Copy from the temporary buffer to the new screen buffer. 

    fSuccess = WriteConsoleOutput(
        m_handle_screen_buff, // screen buffer to write to 
        ch_info_buff,        // buffer to copy from 
        coordBufSize,    // col-row size of chiBuffer 
        coordBufCoord,    // top left src cell in chiBuffer 
        &write_rect
    );  // dest. screen buffer rectangle 

    if (!fSuccess)
    {
        printf("WriteConsoleOutput failed - (%d)\n", GetLastError());
        return;
    }
}

void Console::get_char_buffer(CHAR_INFO *char_info_buff_out) {
    
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
        coord_buff_size,      // col-row size of chiBuffer 
        coord_destination,    // top left destination cell in char_buff_out 
        &read_rect
    );
}

/* size: set the screen size for this console. */
void Console::size(COORD screen_size) {
    if (SetConsoleScreenBufferSize(m_handle_screen_buff, screen_size)) {
        m_width = screen_size.X;
        m_height = screen_size.Y;
    };
}

/* size: set the screen size for this console. */
void Console::size(int width, int height) {
    COORD screen_size;
    screen_size.X = width;
    screen_size.Y = height;
    size(screen_size);
}

/* MoveCursorToXY: move the cursor to specified column and line number. */
void Console::MoveCursorToXY(int column, int line)
{
    COORD coord;
    m_cursor_X = coord.X = column;
    m_cursor_Y = coord.Y = line;
    HANDLE h_std_out = GetStdHandle(STD_OUTPUT_HANDLE);

    // Finally, call the SetConsoleCursorPosition function.
    if (!SetConsoleCursorPosition(h_std_out, coord))
    {
        // TODO: Call GetLastError() to get a more specific error code.
    }
}

/* MoveCursor: move the cursor in specified direction by specified amount. */
void Console::MoveCursor(DIRECTION direction, int distance) {

    int cx = m_cursor_X;
    int cy = m_cursor_Y;
    int cols = m_width;
    int rows = m_height;

    switch (direction) {
    case LEFT:
        if (editor_mode == NORMAL_MODE && cx >= 2 && m_line_numbers_on
            || cx >= 1 && !m_line_numbers_on) {
            MoveCursorToXY(--m_cursor_X, m_cursor_Y);
        }
        break;
    case UP:
        if (editor_mode == NORMAL_MODE && cy >= 1) {
            MoveCursorToXY(m_cursor_X, --m_cursor_Y);
        }
        break;
    case RIGHT:
        if (editor_mode == NORMAL_MODE && cx < (cols - 1) && m_line_numbers_on) {
            MoveCursorToXY(++m_cursor_X, m_cursor_Y);
        }
        break;
    case DOWN:
        if (editor_mode == NORMAL_MODE && (cy < rows - 1)) {
            MoveCursorToXY(m_cursor_X, ++m_cursor_Y);
        }
        break;

    default: break;
    }
}

Console::~Console()
{
}
