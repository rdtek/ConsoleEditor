#include "ConsoleBufferView.h"

/* ConsoleViewBuffer: constructor. */
// ConsoleViewBuffer::ConsoleViewBuffer() {}

/* ~ConsoleViewBuffer: destructor. */
ConsoleBufferView::~ConsoleBufferView() { }

void ConsoleBufferView::render(ConsoleBufferModel model_buffer) {
    
    HANDLE                      h_stdout = GetStdHandle(STD_OUTPUT_HANDLE);
    SMALL_RECT                  rect_area_read;
    const int                   ch_buff_num_lines   = 1000;
    const int                   chars_per_line      = 100;
    const int                   ch_buff_2d_size     = 100000;  // 1000 lines * 100 characters 
    CHAR_INFO                   ch_info_model_buff[ch_buff_2d_size];
    CHAR_INFO                   ch_info_view_buff[ch_buff_2d_size];
    std::vector<CHAR_INFO>      v_chars_view;
    COORD                       coord_ch_temp_buff_size;
    COORD                       coord_read_top_left;
    CONSOLE_SCREEN_BUFFER_INFO  screen_info;
    BOOL                        func_success;

    GetConsoleScreenBufferInfo(h_stdout, &screen_info);

    /* Set screen buffer size to match console window, this prevents mouse scrolling. */
    //model_buffer.size(
    //    screen_info.srWindow.Right - screen_info.srWindow.Left + 1,
    //    screen_info.srWindow.Bottom - screen_info.srWindow.Top + 1
    //);

    rect_area_read.Top      = screen_info.dwCursorPosition.Y - 1001;
    rect_area_read.Left     = 0;
    rect_area_read.Bottom   = screen_info.dwSize.Y - 1;
    rect_area_read.Right    = screen_info.dwSize.X - 1;
    
    coord_ch_temp_buff_size.Y   = rect_area_read.Bottom;
    coord_ch_temp_buff_size.X   = rect_area_read.Right + 1;
    coord_read_top_left.X       = 0;
    coord_read_top_left.Y       = 0;

    if (rect_area_read.Top < 0) rect_area_read.Top = 0;
    if (rect_area_read.Bottom < 0) rect_area_read.Bottom = 0;

    //Read the character buffer from the model
    func_success = ReadConsoleOutput(
        model_buffer.h_screen_buff(), // Screen buffer to read from. 
        ch_info_model_buff,           // CHAR INFO buffer to copy into. 
        coord_ch_temp_buff_size,      // Column-row size of CHAR INFO buffer. 
        coord_read_top_left,          // Top left cell to read from. 
        &rect_area_read               // Source read area rectangle.
    ); 

    if (!func_success) {
        printf("ReadConsoleOutput failed - (%d)\n", GetLastError());
        return;
    }

    //Build new char info array inserting line numbers
    int idx_line = 0;
    int idx_model_char = 0;
    int idx_view_char = 0;

    //Add the line number chars and content chars into a vector
    int length_chars = (rect_area_read.Bottom + 1) * (rect_area_read.Right + 1);
    while (idx_model_char < length_chars) {

        //Add the line number characters first
        string str_line_idx = to_string(idx_line);
        for (int i = 0; i < str_line_idx.size(); i++) {
            CHAR_INFO ch_info_line_num;
            ch_info_line_num.Char.AsciiChar = str_line_idx.at(i);
            ch_info_view_buff[idx_view_char] = ch_info_line_num;
            idx_view_char++;
        }

        //Add the data model chars i.e. the real content
        int num_chars_this_line = chars_per_line - str_line_idx.size();
        int count_line_chars = 0;
        while (count_line_chars < num_chars_this_line) {
            ch_info_view_buff[idx_view_char] = ch_info_model_buff[idx_model_char];
            idx_view_char++;
            count_line_chars++;
            idx_model_char++;
        }

        idx_line++;
    }

    //TODO: write the vector of chars to the screen buffer for this view object
    size_t v_chars_size = v_chars_view.size();
    COORD coord_write_size;
    coord_write_size.Y = v_chars_size / screen_info.dwSize.X;
    coord_write_size.X = screen_info.dwSize.X;
    
    COORD coord_write_top_left;
    coord_write_top_left.X = 0;
    coord_write_top_left.Y = 0;

    SMALL_RECT rect_write_area;
    rect_write_area.Top = 0;
    rect_write_area.Left = 0;
    rect_write_area.Bottom = screen_info.dwSize.Y - 1;
    rect_write_area.Right = screen_info.dwSize.X - 1;

    func_success = WriteConsoleOutput(
        m_handle_screen_buff,    // Screen buffer to write to 
        ch_info_view_buff,        // Buffer to copy from 
        coord_write_size,        // Column-row size of char info buffer 
        coord_write_top_left,    // Top left position 
        &rect_write_area         // Destination screen buffer rectangle 
    );

    if (!func_success) {
        printf("WriteConsoleOutput failed - (%d)\n", GetLastError());
        return;
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
