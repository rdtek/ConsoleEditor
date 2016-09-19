#include "CharLine.h"

CharLine::CharLine() {}

CharLine::~CharLine(){}

void CharLine::add_char_index(size_t index) { m_char_indexes.push_back(index); }

void CharLine::remove_char_index(size_t index) {
    size_t idx_erase;

    for (size_t i = 0; i < m_char_indexes.size(); i++)
    {
        if (m_char_indexes[i] == index) {
            idx_erase = i;
            break;
        }
    }

    m_char_indexes.erase(
        m_char_indexes.begin() + idx_erase,
        m_char_indexes.begin() + idx_erase + 1
    );
}
