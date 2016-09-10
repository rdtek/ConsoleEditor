#include "ConsoleBufferBase.h"

ConsoleBufferBase::ConsoleBufferBase()
{
    // Creates a handle to an empty console screen buffer
    m_handle_screen_buff = CreateConsoleScreenBuffer(
        GENERIC_READ |           // read/write access 
        GENERIC_WRITE,
        FILE_SHARE_READ |
        FILE_SHARE_WRITE,        // shared 
        NULL,                    // default security attributes 
        CONSOLE_TEXTMODE_BUFFER, // must be TEXTMODE 
        NULL);                   // reserved; must be NULL 

    if (m_handle_screen_buff == INVALID_HANDLE_VALUE) {
        //TODO: log error
    }
}

ConsoleBufferBase::~ConsoleBufferBase() {}

void ConsoleBufferBase::get_char_buffer(CHAR_INFO *char_info_buff_out) {

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
