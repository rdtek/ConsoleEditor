#pragma once
#include "AppCommon.h"
#include "ConsoleBufferBase.h"
#include "ConsoleBufferModel.h"
#include <vector>

using namespace std;

class ConsoleBufferView : public ConsoleBufferBase
{
public:
    ConsoleBufferView() : ConsoleBufferBase() {};
    ~ConsoleBufferView();
    HANDLE h_screen_buff() const { return m_handle_screen_buff; }
    void write();
    void flush();
    void render(ConsoleBufferModel model_buffer);
private:
    HANDLE m_handle_screen_buff;
};

