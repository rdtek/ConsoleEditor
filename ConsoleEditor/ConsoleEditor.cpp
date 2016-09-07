#include <windows.h>
#include <stdio.h>
#include <iostream>

//See http://stackoverflow.com/questions/24708700/c-detect-when-user-presses-arrow-key
//And ReadConsoleInput() function https://msdn.microsoft.com/en-us/library/windows/desktop/ms684961(v=vs.85).aspx

enum DIRECTION
{
    LEFT = 0,
    UP,
    RIGHT,
    DOWN
};

HANDLE h_std_out;
HANDLE h_std_input;
int console_num_cols;
int console_num_rows;
int cursor_x = 0;
int cursor_y = 0;
boolean b_line_numbers_on = true;
bool b_running = true;

void MoveCursorToXY(int column, int line)
{
    COORD coord;
    cursor_x = coord.X = column;
    cursor_y = coord.Y = line;
    h_std_out = GetStdHandle(STD_OUTPUT_HANDLE);

    // Finally, call the SetConsoleCursorPosition function.
    if (!SetConsoleCursorPosition(h_std_out, coord))
    {
        // TODO: Call GetLastError() to get a more specific error code.
    }
}

void SetScreenSize(CONSOLE_SCREEN_BUFFER_INFO csbi) {
    /* In old-school VIM the screen buffer size is the size
    of the console window, this prevents mouse scrolling. */
    COORD coord_screen_size;
    console_num_cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    console_num_rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    coord_screen_size.X = console_num_cols;
    coord_screen_size.Y = console_num_rows;
    SetConsoleScreenBufferSize(h_std_out, coord_screen_size);
}

void MoveCursor(DIRECTION direction, int distance) {
    switch (direction) {
        case LEFT:   
            if (cursor_x >= 2 && b_line_numbers_on
                || cursor_x >= 1 && !b_line_numbers_on) {
                MoveCursorToXY(--cursor_x, cursor_y);
            }
            break;
        case UP:
            if (cursor_y >= 1) {
                MoveCursorToXY(cursor_x, --cursor_y);
            }
            break;
        case RIGHT:  
            if (cursor_x < (console_num_cols - 1) && b_line_numbers_on) {
                MoveCursorToXY(++cursor_x, cursor_y);
            }
            break;
        case DOWN:
            if (cursor_y < console_num_rows - 1) {
                MoveCursorToXY(cursor_x, ++cursor_y);
            }
            break;

        default: break;
    }
}

void RunEventLoop() {

    while (b_running) {

        GetNumberOfConsoleInputEvents(h_std_input, &num_events);

        if (num_events > 0) {
            //WriteConsole();
            INPUT_RECORD event_buff[1];
            ReadConsoleInput(h_std_input, event_buff, 1, &num_events_read);

            if (event_buff[0].EventType == KEY_EVENT && event_buff[0].Event.KeyEvent.bKeyDown) {

                switch (event_buff[0].Event.KeyEvent.wVirtualKeyCode) {
                    case VK_LEFT:    MoveCursor(LEFT, 1);   break;
                    case VK_RIGHT:   MoveCursor(RIGHT, 1);  break;
                    case VK_UP:      MoveCursor(UP, 1);     break;
                    case VK_DOWN:    MoveCursor(DOWN, 1);   break;

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

int main(int argc, char *argv[]) {

    system("cls");

    h_std_out = GetStdHandle(STD_OUTPUT_HANDLE);
    h_std_input = GetStdHandle(STD_INPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screen_buff_size;
    DWORD num_events = 0;
    DWORD num_events_read = 0;

    GetConsoleScreenBufferInfo(h_std_out, &screen_buff_size);
    SetScreenSize(screen_buff_size);

    //Print the line numbers on the left hand side
    for (size_t i = 0; i < console_num_rows; i++) {
        printf("\n%i", i);
    }

    MoveCursorToXY(1, 0);
    RunEventLoop();

    return 0;
}