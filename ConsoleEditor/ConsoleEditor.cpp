#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <thread>
#include <chrono>

//See http://stackoverflow.com/questions/24708700/c-detect-when-user-presses-arrow-key
//And ReadConsoleInput() function https://msdn.microsoft.com/en-us/library/windows/desktop/ms684961(v=vs.85).aspx

HANDLE h_std_out;

void GoToXY(int column, int line)
{
    COORD coord;
    coord.X = column;
    coord.Y = line;

    HANDLE h_console = GetStdHandle(STD_OUTPUT_HANDLE);

    // Finally, call the SetConsoleCursorPosition function.
    if (!SetConsoleCursorPosition(h_console, coord))
    {
        // TODO: Call GetLastError() to get a more specific error code.
    }
}

void GetKeyPress()
{
    std::chrono::milliseconds THREAD_WAIT(100);

    int keypress = -1;

    while (1)
    {
        for (int i = 8; i <= 190; i++)
        {
            int k = GetAsyncKeyState(i);
            if (k != -1) keypress = k;
        }
        
        if (keypress > 0) {
            printf("%i", keypress);
            break; //Use this only if you have td.join() below
        }

        //char s[] = "Hello\n";
        //unsigned long cChars;
        //printf("%i", keypress);
        //WriteConsole(h_std_out, s, 6, &cChars, NULL);

        std::this_thread::sleep_for(THREAD_WAIT);
    }
}

int main(int argc, char *argv[]) {

    system("cls");

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns, rows;
    h_std_out = GetStdHandle(STD_OUTPUT_HANDLE);

    GetConsoleScreenBufferInfo(h_std_out, &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    /* In old-school VIM the screen buffer size is the size
    of the console window, this prevents mouse scrolling. */
    COORD coord_screen_size;
    coord_screen_size.X = columns;
    coord_screen_size.Y = rows;
    SetConsoleScreenBufferSize(h_std_out, coord_screen_size);

    printf("columns: %d\n", columns);

    for (size_t i = 0; i < rows; i++)
    {
        // printf("rows: %d\n", rows);
        printf("\n%i", i);
    }

    GoToXY(0, 1);

    std::thread thread_key_press(GetKeyPress);
    thread_key_press.join();

    //std::thread thread_app_close()

    return 0;
}