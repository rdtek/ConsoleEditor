#pragma once
#include "AppCommon.h"
#include "ConsoleBufferModel.h"

class ConsoleViewBuffer
{
public:
    ConsoleViewBuffer();
    ~ConsoleViewBuffer();
    HANDLE h_screen_buff() const { return m_handle_screen_buff; }
    void write();
    void flush();
    void render(ConsoleModelBuffer model_buffer);
private:
    HANDLE m_handle_screen_buff;
};

