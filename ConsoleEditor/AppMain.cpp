#include "AppMain.h"
#include "FileTools.h"

/* main - application entry point.*/
int main(int argc, char *argv[]) {

    string              str_filename;
    HANDLE              h_std_out = GetStdHandle(STD_OUTPUT_HANDLE);
    ConsoleController   console_controller;
    ConsoleBuffer       original_console;
    ConsoleBuffer       editor_console;

    //Load text from a file if valid filename argument provided
    if (argc >= 2) {
        str_filename = str_filename.append(argv[1]);
        if (FileTools::file_exists(str_filename)) {
            editor_console.load_file(str_filename);
        }
    }

#ifdef _DEBUG
    //Debug testing, output some chars to the console so we can read them 
    printf("abcdefg\ntest");
#endif
    original_console.load_buffer(h_std_out);

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