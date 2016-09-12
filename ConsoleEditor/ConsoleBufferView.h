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
    void ConsoleBufferView::render(ConsoleBufferModel model_buffer, int screen_num_cols, int screen_num_rows);
private:
    HANDLE m_handle_screen_buff;
};

