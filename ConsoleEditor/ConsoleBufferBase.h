#pragma once
#include "AppCommon.h"
#include <vector>

using namespace std;

class ConsoleBufferBase
{
public:
    ConsoleBufferBase();
    ~ConsoleBufferBase();

    /* get_char_buffer: read the char_info items into the output array. */
    void get_char_buffer(CHAR_INFO *char_info_buff_out);
    int length() const { return m_vect_char_buffer.size(); }
protected:
    HANDLE m_handle_screen_buff;
    vector<CHAR_INFO> m_vect_char_buffer;
    int m_width;
    int m_height;
};

