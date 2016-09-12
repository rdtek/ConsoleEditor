#pragma once
#include "AppCommon.h"
#include <stdio.h>
#include <string>
#include "ConsoleBufferBase.h"

using namespace std;

class ConsoleBufferModel : public ConsoleBufferBase
{
public:
    ConsoleBufferModel() : ConsoleBufferBase() {};
    ~ConsoleBufferModel();

    void copy_from_buffer(HANDLE h_source_buffer);
    
    HANDLE h_screen_buff() const { return m_handle_screen_buff; }
    void h_screen_buff(HANDLE h_screen_buff) { m_handle_screen_buff = h_screen_buff; }
};

