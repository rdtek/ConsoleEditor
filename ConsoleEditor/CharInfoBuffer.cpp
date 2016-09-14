#include "CharInfoBuffer.h"

CharInfoBuffer::CharInfoBuffer(){}

CharInfoBuffer::~CharInfoBuffer(){}

int CharInfoBuffer::size() {
    return m_vect_char_info_buff.size();
}

CHAR_INFO CharInfoBuffer::at(size_t index) {
    return m_vect_char_info_buff[index];
}

void CharInfoBuffer::append(CHAR_INFO& char_info) {
    m_vect_char_info_buff.push_back(char_info);
}

void CharInfoBuffer::append(CHAR_INFO* char_info_arr, size_t begin, size_t length) {
    for (size_t i = begin; i < begin + length; i++) {
        this->append(char_info_arr[i]);
    }
}

void CharInfoBuffer::append(const string& str_line_content, FG_COLOR foreground_color) {
    
    for (int i = 0; i < str_line_content.size(); i++) {
        CHAR_INFO char_info;
        char_info.Char.UnicodeChar = str_line_content.at(i);
        char_info.Attributes = foreground_color;
        this->append(char_info);
    }
}

/* to_char_info_array: fill the ouput array with the CHAR_INFO items. */
void CharInfoBuffer::to_array(CHAR_INFO* ptr_char_info_arr_out) {
    ptr_char_info_arr_out = &m_vect_char_info_buff[0];
    //TODO: copy the content from the vector to the output array
}

void CharInfoBuffer::reset() {
    //Clear and release the memory of the vector
    m_vect_char_info_buff.clear();
    vector<CHAR_INFO>().swap(m_vect_char_info_buff);
}

