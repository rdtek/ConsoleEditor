#include "ConsoleEditor.h"
#include "Console.h"

HANDLE h_std_input;
boolean b_line_numbers_on = true;
bool b_running = true;
Console current_console;

void RunEventLoop() {
    
    HANDLE h_std_input = GetStdHandle(STD_INPUT_HANDLE);
    DWORD num_events = 0;
    DWORD num_events_read = 0;

    while (b_running) {

        GetNumberOfConsoleInputEvents(h_std_input, &num_events);

        //Handle key events
        if (num_events > 0) {
            
            //Process each event one by one
            INPUT_RECORD event_buff[1];
            ReadConsoleInput(h_std_input, event_buff, 1, &num_events_read);

            if (event_buff[0].EventType == KEY_EVENT && event_buff[0].Event.KeyEvent.bKeyDown) {

                switch (event_buff[0].Event.KeyEvent.wVirtualKeyCode) {
                    case VK_LEFT:    current_console.MoveCursor(LEFT, 1);   break;
                    case VK_RIGHT:   current_console.MoveCursor(RIGHT, 1);  break;
                    case VK_UP:      current_console.MoveCursor(UP, 1);     break;
                    case VK_DOWN:    current_console.MoveCursor(DOWN, 1);   break;

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

void init_editor_view(Console editor_console, CONSOLE_SCREEN_BUFFER_INFO screen_info) {
    
    //Clear any content that was present before
    system("cls");

    /* Set screen buffer size to match console window, this prevents mouse scrolling. */
    editor_console.size(
        screen_info.srWindow.Right - screen_info.srWindow.Left + 1, 
        screen_info.srWindow.Bottom - screen_info.srWindow.Top + 1
    );

    //Print the line numbers on the left hand side
    for (size_t i = 0; i < editor_console.height(); i++) {
        printf("\n%i", i);
    }
}

void activate_console(Console console) {
    
    //Set the provided console buffer as the active console buffer
    if (!SetConsoleActiveScreenBuffer(console.h_screen_buff())) {
        printf("SetConsoleActiveScreenBuffer failed - (%d)\n", GetLastError());
        return;
    }

    //Set the original screen size
    COORD coord_screen_size;
    coord_screen_size.X = console.width();
    coord_screen_size.Y = console.height();
    
    SetConsoleScreenBufferSize(console.h_screen_buff(), coord_screen_size);

    console.MoveCursorToXY(1, console.cursor_Y());
}

int main(int argc, char *argv[]) {

    HANDLE h_std_out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screen_info;

    //TEST output some chars to the console so we can read them 
    printf("abcdefg\nhijklmno");

    Console original_console(h_std_out);

    Console editor_console;

    //Activate and render the editor console
    activate_console(editor_console);
    GetConsoleScreenBufferInfo(h_std_out, &screen_info);
    init_editor_view(editor_console, screen_info);
    
    RunEventLoop();

    //Restore the original screen content that was there before we launched the editor
    activate_console(original_console);

    getchar();
    //system("pause");

    return 0;
}