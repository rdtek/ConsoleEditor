#pragma once
#include "AppCommon.h"
#include "ConsoleBufferBase.h"
#include "ConsoleBufferModel.h"
#include <vector>

using namespace std;

enum FG_COLOR {
    BLACK = 0,
    DARKBLUE = FOREGROUND_BLUE,
    DARKGREEN = FOREGROUND_GREEN,
    DARKCYAN = FOREGROUND_GREEN | FOREGROUND_BLUE,
    DARKRED = FOREGROUND_RED,
    DARKMAGENTA = FOREGROUND_RED | FOREGROUND_BLUE,
    DARKYELLOW = FOREGROUND_RED | FOREGROUND_GREEN,
    DARKGRAY = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
    GRAY = FOREGROUND_INTENSITY,
    BLUE = FOREGROUND_INTENSITY | FOREGROUND_BLUE,
    GREEN = FOREGROUND_INTENSITY | FOREGROUND_GREEN,
    CYAN = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,
    RED = FOREGROUND_INTENSITY | FOREGROUND_RED,
    MAGENTA = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,
    YELLOW = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
    WHITE = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
};

class ConsoleBufferView : public ConsoleBufferBase
{
public:
    ConsoleBufferView() : ConsoleBufferBase() {};
    ~ConsoleBufferView();
    HANDLE h_screen_buff() const { return m_handle_screen_buff; }
    void write();
    void flush();
    void ConsoleBufferView::render(ConsoleBufferModel model_buffer, CONSOLE_SCREEN_BUFFER_INFO screen_info);
};