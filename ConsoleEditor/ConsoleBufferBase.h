#pragma once
#include "AppCommon.h"

class ConsoleBufferBase
{
public:
    ConsoleBufferBase();
    ~ConsoleBufferBase();

    /* get_char_buffer: read the char_info items into the output array. */
    void get_char_buffer(CHAR_INFO *char_info_buff_out);
protected:
    HANDLE m_handle_screen_buff;
    CHAR_INFO m_char_buffer;
    int m_width;
    int m_height;
};

