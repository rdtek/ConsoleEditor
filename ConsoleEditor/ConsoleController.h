#pragma once
#include "ConsoleBufferModel.h"
#include "ConsoleBufferView.h"

class ConsoleController
{
public:
    ConsoleController();
    ~ConsoleController();

    void run_event_loop();
    void move_cursor(DIRECTION direction, int distance);
    void activate_original_view(ConsoleModelBuffer original_console);
    void activate_editor_view(ConsoleModelBuffer editor_console);
private:
    ConsoleModelBuffer m_current_console_buff;
};

