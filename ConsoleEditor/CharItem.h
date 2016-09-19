#pragma once
#include "CharLine.h"

class CharItem
{
public:
    CharItem(char the_chr);
    ~CharItem();

    CharLine line() const { return m_line; }
    void line(CharLine line) { m_line = line; }

    char chr() const { return m_char; }
    void chr(char chr) { m_char = chr; }

    size_t index() const { return m_index; }
    void index(size_t index) { m_index = index; }

    bool is_line_start() const { return m_is_line_start; }
    void is_line_start(bool is_line_start) { m_is_line_start = is_line_start; }

private:
    CharLine m_line;
    char m_char;
    size_t m_index;
    bool m_is_line_start;
};

