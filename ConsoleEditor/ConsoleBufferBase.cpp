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

ConsoleBufferBase::~ConsoleBufferBase()
{
}
