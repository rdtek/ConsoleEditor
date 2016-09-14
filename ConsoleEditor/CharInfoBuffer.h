#pragma once
#include <vector>
#include "AppCommon.h"
#include "ConsoleColors.h"

using namespace std;

class CharInfoBuffer
{
public:
    CharInfoBuffer();
    ~CharInfoBuffer();
    int size();
    CHAR_INFO at(size_t index);
    void append(CHAR_INFO& char_info);
    void append(CHAR_INFO* char_info_arr, size_t begin, size_t length);
    void append(const string& str_line_content, FG_COLOR foreground_color);
    void to_array(CHAR_INFO* ptr_char_info_arr_out);
    void reset();
private:
    vector<CHAR_INFO> m_vect_char_info_buff;
};