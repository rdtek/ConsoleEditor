#include "AppMain.h"

/* main - applicatin entry point.*/
int main(int argc, char *argv[]) {

    HANDLE h_std_out = GetStdHandle(STD_OUTPUT_HANDLE);

#ifdef _DEBUG
    //Debug testing, output some chars to the console so we can read them 
    printf("abcdefg\ntest");
#endif

    ConsoleController  console_controller;
    ConsoleBufferModel original_console;
    ConsoleBufferModel editor_console;

    original_console.copy_from_buffer(h_std_out);

    //Render the editor console
    console_controller.activate_editor_view(editor_console);
    
    //Listen for keypress input events, loop until user exits editor
    console_controller.run_event_loop();

    //Restore the original screen content
    console_controller.activate_original_view(original_console);

#ifdef _DEBUG
    //Keep terminal open.
    getchar(); 
#endif

    return 0;
}