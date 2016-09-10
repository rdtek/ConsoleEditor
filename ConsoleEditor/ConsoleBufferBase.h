#pragma once
#include "AppCommon.h"

class ConsoleBufferBase
{
public:
    ConsoleBufferBase();
    ~ConsoleBufferBase();
protected:
    HANDLE m_handle_screen_buff;
    CHAR_INFO m_char_buffer;
    int m_width;
    int m_height;
};

