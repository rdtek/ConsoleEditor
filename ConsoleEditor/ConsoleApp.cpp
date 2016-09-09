#include "ConsoleApp.h"
#include "ConsoleBuffer.h"

ConsoleBuffer current_console;

void RunEventLoop() {
    
    HANDLE h_std_input = GetStdHandle(STD_INPUT_HANDLE);
    DWORD num_events = 0;
    DWORD num_events_read = 0;
    bool b_running = true;

    while (b_running) {

        GetNumberOfConsoleInputEvents(h_std_input, &num_events);

        //Handle key events
        if (num_events > 0) {
            
            //Process each event one by one
            INPUT_RECORD event_buff[1];
            ReadConsoleInput(h_std_input, event_buff, 1, &num_events_read);

            if (event_buff[0].EventType == KEY_EVENT && event_buff[0].Event.KeyEvent.bKeyDown) {

                switch (event_buff[0].Event.KeyEvent.wVirtualKeyCode) {
                    case VK_LEFT:    current_console.move_cursor(LEFT, 1);   break;
                    case VK_RIGHT:   current_console.move_cursor(RIGHT, 1);  break;
                    case VK_UP:      current_console.move_cursor(UP, 1);     break;
                    case VK_DOWN:    current_console.move_cursor(DOWN, 1);   break;

                    case VK_ESCAPE:
                        std::cout << "End program.\n";
                        b_running = false;
                        break;

                    default: break;
                }
            }
        }
    }
}

void activate_editor_view(ConsoleBuffer editor_console) {
    
    HANDLE h_std_out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screen_info;
    
    GetConsoleScreenBufferInfo(h_std_out, &screen_info);

    //Clear any content that was present before
    system("cls");

    /* Set screen buffer size to match console window, this prevents mouse scrolling. */
    editor_console.size(
        screen_info.srWindow.Right - screen_info.srWindow.Left + 1, 
        screen_info.srWindow.Bottom - screen_info.srWindow.Top + 1
    );

    //Print the line numbers on the left hand side
    for (size_t i = 0; i < editor_console.height(); i++) {
        printf("\n%i", i); //TODO: don't use printf, write to the console buffer instead.
    }

    //Set the provided console buffer as the active console buffer
    if (!SetConsoleActiveScreenBuffer(editor_console.h_screen_buff())) {
        printf("SetConsoleActiveScreenBuffer failed - (%d)\n", GetLastError());
        return;
    }

    editor_console.move_cursor_to(1, 0);
}

void activate_original_view(ConsoleBuffer original_console) {

    HANDLE h_std_out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screen_info;

    GetConsoleScreenBufferInfo(h_std_out, &screen_info);

    //Clear any content that was present before
    system("cls");

    original_console.move_cursor_to(original_console.cursor_X(), original_console.cursor_Y());

    //Set the provided console buffer as the active console buffer
    if (!SetConsoleActiveScreenBuffer(original_console.h_screen_buff())) {
        printf("SetConsoleActiveScreenBuffer failed - (%d)\n", GetLastError());
        return;
    }
}

/* main - applicatin entry point.*/
int main(int argc, char *argv[]) {

    HANDLE h_std_out = GetStdHandle(STD_OUTPUT_HANDLE);

#ifdef _DEBUG
    //Debug testing, output some chars to the console so we can read them 
    printf("abcdefg\nhijklmno");
#endif

    ConsoleBuffer original_console;
    ConsoleBuffer editor_console;

    original_console.copy_buffer(h_std_out);

    //Render the editor console
    activate_editor_view(editor_console);
    
    //Listen for keypress input events
    RunEventLoop();

    //Restore the original screen content
    activate_original_view(original_console);

#ifdef _DEBUG
    getchar(); //Keep terminal open.
#endif

    return 0;
}