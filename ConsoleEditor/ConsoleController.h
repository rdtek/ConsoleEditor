#pragma once
#include "ConsoleBuffer.h"

class ConsoleController
{
public:
    ConsoleController();
    ~ConsoleController();

    void run_event_loop();
    void move_cursor(DIRECTION direction, int distance);
    void activate_original_view(ConsoleBuffer original_console);
    void activate_editor_view(ConsoleBuffer editor_console);
private:
    ConsoleBuffer m_current_console_buff;
};

