#include "ConsoleEditor.h"

EDITOR_MODE editor_mode = NORMAL_MODE;
HANDLE h_std_out;
HANDLE h_std_input;
HANDLE h_orig_console_buff;

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
    
    int cx = cursor_x;
    int cy = cursor_y;
    int cols = console_num_cols;
    int rows = console_num_rows;

    switch (direction) {
        case LEFT:   
            if (editor_mode == NORMAL_MODE && cx >= 2 && b_line_numbers_on
                || cx >= 1 && !b_line_numbers_on) {
                MoveCursorToXY(--cursor_x, cursor_y);
            }
            break;
        case UP:
            if (editor_mode == NORMAL_MODE && cy >= 1) {
                MoveCursorToXY(cursor_x, --cursor_y);
            }
            break;
        case RIGHT:  
            if (editor_mode == NORMAL_MODE && cx < (cols - 1) && b_line_numbers_on) {
                MoveCursorToXY(++cursor_x, cursor_y);
            }
            break;
        case DOWN:
            if (editor_mode == NORMAL_MODE &&  (cy < rows - 1)) {
                MoveCursorToXY(cursor_x, ++cursor_y);
            }
            break;

        default: break;
    }
}

void RunEventLoop() {
    
    DWORD num_events = 0;
    DWORD num_events_read = 0;

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

int SaveConsoleBuffer(HANDLE h_saved_console_buff) {

    HANDLE hStdOut;
    CONSOLE_SCREEN_BUFFER_INFO screen_buff_size;
    SMALL_RECT read_rect;
    SMALL_RECT write_rect;
    CHAR_INFO ch_info_buff[100000]; // [1000][100]; 1000 lines * 100 characters 
    COORD coordBufSize;
    COORD coordBufCoord;
    BOOL fSuccess;

    // Get a handle to the STDOUT screen buffer to copy from and 
    // create a new screen buffer to copy to. 

    hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hStdOut, &screen_buff_size);
    
    if (hStdOut == INVALID_HANDLE_VALUE ||
        h_saved_console_buff == INVALID_HANDLE_VALUE)
    {
        printf("CreateConsoleScreenBuffer failed - (%d)\n", GetLastError());
        return 1;
    }

    // Set the source rectangle. 
    read_rect.Top = screen_buff_size.dwCursorPosition.Y - 1001;
    if (read_rect.Top < 0) read_rect.Top = 0;
    read_rect.Left = 0;
    read_rect.Bottom = screen_buff_size.dwCursorPosition.Y; // bot. right: row 1, col 79 
    if (read_rect.Bottom <= 0) read_rect.Bottom = 1;
    read_rect.Right = screen_buff_size.dwSize.X - 1;

    // The temporary buffer size is 2 rows x 80 columns. 
    coordBufSize.Y = 1000;
    coordBufSize.X = read_rect.Right + 1;

    // The top left destination cell of the temporary buffer is 
    // row 0, col 0. 
    coordBufCoord.X = 0;
    coordBufCoord.Y = 0;

    // Copy the block from the screen buffer to the temp. buffer. 
    //TEST output some chars to the console so we can read them 
    printf("abcdefg\nhijklmno");

    fSuccess = ReadConsoleOutput(
        hStdOut,        // screen buffer to read from 
        ch_info_buff,      // buffer to copy into 
        coordBufSize,   // col-row size of chiBuffer 
        coordBufCoord,  // top left dest. cell in chiBuffer 
        &read_rect); // screen buffer source rectangle 
    
    if (!fSuccess)
    {
        printf("ReadConsoleOutput failed - (%d)\n", GetLastError());
        return 1;
    }

    // Set the destination rectangle. 
    write_rect.Top = 10;    // top lt: row 10, col 0 
    write_rect.Left = 0;
    write_rect.Bottom = read_rect.Bottom - read_rect.Top; // bot. rt: row 11, col 79 
    write_rect.Right = 79;

    // Copy from the temporary buffer to the new screen buffer. 

    fSuccess = WriteConsoleOutput(
        h_saved_console_buff, // screen buffer to write to 
        ch_info_buff,        // buffer to copy from 
        coordBufSize,     // col-row size of chiBuffer 
        coordBufCoord,    // top left src cell in chiBuffer 
        &write_rect);  // dest. screen buffer rectangle 
    if (!fSuccess)
    {
        printf("WriteConsoleOutput failed - (%d)\n", GetLastError());
        return 1;
    }
}

int RestoreConsoleBuffer(HANDLE h_console_buff) {
    if (!SetConsoleActiveScreenBuffer(h_console_buff))
    {
        printf("SetConsoleActiveScreenBuffer failed - (%d)\n", GetLastError());
        return 1;
    }
    return 0;
}

int main(int argc, char *argv[]) {

    h_std_out = GetStdHandle(STD_OUTPUT_HANDLE);
    h_std_input = GetStdHandle(STD_INPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screen_buff_size;

    h_orig_console_buff = CreateConsoleScreenBuffer(
        GENERIC_READ |           // read/write access 
        GENERIC_WRITE,
        FILE_SHARE_READ |
        FILE_SHARE_WRITE,        // shared 
        NULL,                    // default security attributes 
        CONSOLE_TEXTMODE_BUFFER, // must be TEXTMODE 
        NULL);                   // reserved; must be NULL 

    GetConsoleScreenBufferInfo(h_std_out, &screen_buff_size);

    SaveConsoleBuffer(h_orig_console_buff);

    SetScreenSize(screen_buff_size);

    //Print the line numbers on the left hand side
    /*for (size_t i = 0; i < console_num_rows; i++) {
        printf("\n%i", i);
    }*/

    MoveCursorToXY(1, 0);
    
    RunEventLoop();

    RestoreConsoleBuffer(h_orig_console_buff);
    system("pause");

    return 0;
}