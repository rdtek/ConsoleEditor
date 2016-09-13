#include "CharInfoBuffer.h"

CharInfoBuffer::CharInfoBuffer(){}

CharInfoBuffer::~CharInfoBuffer(){}

int CharInfoBuffer::size() {
    return m_vect_char_info_buff.size();
}

void CharInfoBuffer::insert(size_t idx_insert, 
    CHAR_INFO* ptr_char_info_array_start, size_t length) {
    
    m_vect_char_info_buff.insert(
        m_vect_char_info_buff.begin() + idx_insert, 
        ptr_char_info_array_start,
        ptr_char_info_array_start + length
    );
}

void CharInfoBuffer::append(CHAR_INFO& char_info) {
    m_vect_char_info_buff.push_back(char_info);
}

void CharInfoBuffer::set_range(const char chr, size_t idx_start, size_t length) {
    if (m_vect_char_info_buff.size() < idx_start) return;

    CHAR_INFO char_info;
    char_info.Char.UnicodeChar = chr;
    char_info.Attributes = FG_COLOR::WHITE;

    for (size_t i = idx_start; i < idx_start + length; i++) {
        if (m_vect_char_info_buff.size() < i) break;
        m_vect_char_info_buff[i] = char_info;
    }
}

/* to_char_info_array: fill the ouput array with the CHAR_INFO items. */
void CharInfoBuffer::to_char_info_array(CHAR_INFO* ptr_char_info_arr_out) {
    //TODO: copy the content from the vector to the output array
}
