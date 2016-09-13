#pragma once
#include "AppCommon.h"
#include <vector>
#include "CharInfoBuffer.h"
#include "ConsoleColors.h"

using namespace std;

class CharInfoBuffer
{
public:
    CharInfoBuffer();
    ~CharInfoBuffer();
    int size();
    void append(CHAR_INFO& char_info);
    void set_range(const char chr, size_t idx_start, size_t length);
    void insert(size_t idx_insert, CHAR_INFO* ptr_char_info_array_start, size_t length);
    void CharInfoBuffer::to_char_info_array(CHAR_INFO* ptr_char_info_arr_out);
private:
    vector<CHAR_INFO> m_vect_char_info_buff;
}