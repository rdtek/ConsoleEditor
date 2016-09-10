#include "ConsoleController.h"

/* ConsoleController: constructor. */
ConsoleController::ConsoleController() { }

/* ~ConsoleController: destructor. */
ConsoleController::~ConsoleController() { }

/* run_event_loop - listens for keypress input events */
void ConsoleController::run_event_loop() {

    HANDLE h_std_input = GetStdHandle(STD_INPUT_HANDLE);
    DWORD num_events = 0;
    DWORD num_events_read = 0;
    bool b_running = true;

    while (b_running) {

        GetNumberOfConsoleInputEvents(h_std_input, &num_events);

        //Handle keypress events
        if (num_events > 0) {

            //Process each event one by one
            INPUT_RECORD event_buff[1];
            ReadConsoleInput(h_std_input, event_buff, 1, &num_events_read);

            if (event_buff[0].EventType == KEY_EVENT && event_buff[0].Event.KeyEvent.bKeyDown) {

                switch (event_buff[0].Event.KeyEvent.wVirtualKeyCode) {
                case VK_LEFT:    move_cursor(LEFT, 1);   break;
                case VK_RIGHT:   move_cursor(RIGHT, 1);  break;
                case VK_UP:      move_cursor(UP, 1);     break;
                case VK_DOWN:    move_cursor(DOWN, 1);   break;
                case VK_ESCAPE:  b_running = false;                         break;
                default: break;
                }
            }
        }
    }
}

void ConsoleController::move_cursor(DIRECTION direction, int distance) {
    m_current_console_buff.move_cursor(direction, distance);
}

void ConsoleController::activate_editor_view(ConsoleBufferModel editor_model_buff) {

    ConsoleBufferView view_buffer;
    view_buffer.render(editor_model_buff);

    //Set the provided console buffer as the active console buffer
    if (!SetConsoleActiveScreenBuffer(editor_model_buff.h_screen_buff())) {
        printf("SetConsoleActiveScreenBuffer failed - (%d)\n", GetLastError());
        return;
    }

    editor_model_buff.move_cursor_to(1, 0);
}

void ConsoleController::activate_original_view(ConsoleBufferModel original_console) {

    original_console.refresh_cursor();

    //Set the provided console buffer as the active console buffer
    if (!SetConsoleActiveScreenBuffer(original_console.h_screen_buff())) {
        printf("SetConsoleActiveScreenBuffer failed - (%d)\n", GetLastError());
        return;
    }
}
