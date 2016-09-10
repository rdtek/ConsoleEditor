#include "ConsoleBufferView.h"

/* ConsoleViewBuffer: constructor. */
// ConsoleViewBuffer::ConsoleViewBuffer() {}

/* ~ConsoleViewBuffer: destructor. */
ConsoleBufferView::~ConsoleBufferView() { }

void ConsoleBufferView::render(ConsoleBufferModel model_buffer) {
    
    HANDLE h_stdout = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screen_info;
    SMALL_RECT rect_src_read_area;
    SMALL_RECT rect_write_area;
    CHAR_INFO ch_info_temp_buff[100000]; // [1000][100]; 1000 lines * 100 characters 
    COORD coord_ch_temp_buff_size;
    COORD coord_start_temp_buff;
    CONSOLE_SCREEN_BUFFER_INFO src_screen_info;
    BOOL func_success;

    GetConsoleScreenBufferInfo(h_stdout, &src_screen_info);

    /* Set screen buffer size to match console window, this prevents mouse scrolling. */
    model_buffer.size(
        src_screen_info.srWindow.Right - src_screen_info.srWindow.Left + 1,
        src_screen_info.srWindow.Bottom - src_screen_info.srWindow.Top + 1
    );

    rect_src_read_area.Top      = src_screen_info.dwCursorPosition.Y - 1001;
    rect_src_read_area.Left     = 0;
    rect_src_read_area.Bottom   = src_screen_info.dwSize.Y - 1;
    rect_src_read_area.Right    = src_screen_info.dwSize.X - 1;
    
    coord_ch_temp_buff_size.Y   = 100;
    coord_ch_temp_buff_size.X   = rect_src_read_area.Right + 1;
    coord_start_temp_buff.X     = 0;
    coord_start_temp_buff.Y     = 0;

    if (rect_src_read_area.Top < 0) rect_src_read_area.Top = 0;
    if (rect_src_read_area.Bottom < 0) rect_src_read_area.Bottom = 0;

    //Read the character buffer from the model
    func_success = ReadConsoleOutput(
        model_buffer.h_screen_buff(), // screen buffer to read from 
        ch_info_temp_buff,            // buffer to copy into 
        coord_ch_temp_buff_size,      // col-row size of ch_info_buff 
        coord_start_temp_buff,        // top left dest. cell in ch_info_buff 
        &rect_src_read_area           // screen buffer source rectangle 
    ); 

    if (!func_success) {
        printf("ReadConsoleOutput failed - (%d)\n", GetLastError());
        return;
    }

    //TODO: Build new char info array inserting line numbers

    //Print the line numbers on the left hand side
    for (size_t i = 0; i < model_buffer.height(); i++) {
        printf("\n%i", i); //TODO: don't use printf, write to the console buffer instead.
    }
}

void ConsoleBufferView::write() {

    SMALL_RECT read_rect;
    SMALL_RECT rect_write_area;
    CHAR_INFO ch_info_temp_buff[100000]; // [1000][100]; 1000 lines * 100 characters 
    COORD coord_ch_temp_buff_size;
    COORD coord_start_temp_buff;
    CONSOLE_SCREEN_BUFFER_INFO src_screen_info;
    BOOL func_success;

    // Copy from the temporary buffer to the new screen buffer. 
    func_success = WriteConsoleOutput(
        m_handle_screen_buff,    // screen buffer to write to 
        ch_info_temp_buff,       // buffer to copy from 
        coord_ch_temp_buff_size, // col-row size of ch_info_temp_buff 
        coord_start_temp_buff,   // top left src cell in ch_info_temp_buff 
        &rect_write_area         // dest. screen buffer rectangle 
    );

    if (!func_success) {
        printf("WriteConsoleOutput failed - (%d)\n", GetLastError());
        return;
    }
}

void ConsoleBufferView::flush() {

}
