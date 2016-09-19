#pragma once
#include <vector>

using namespace std;

class CharLine
{
public:
    CharLine();
    ~CharLine();

    size_t line_index() const { return m_line_index; }
    void line_index(size_t index) { m_line_index = index; }
    void add_char_index(size_t index);
    void remove_char_index(size_t index);

private:
    size_t m_line_index;
    vector<size_t> m_char_indexes;
};

