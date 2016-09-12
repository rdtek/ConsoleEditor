#include "ConsoleBufferView.h"

/* ConsoleViewBuffer: constructor. */
// ConsoleViewBuffer::ConsoleViewBuffer() {}

/* ~ConsoleViewBuffer: destructor. */
ConsoleBufferView::~ConsoleBufferView() { }

void ConsoleBufferView::render(ConsoleBufferModel model_buffer, int screen_num_cols, int screen_num_rows) {
    
    const int                   ch_buff_num_lines   = 1000;
    const int                   chars_per_line      = 100;
    const int                   ch_buff_2d_size     = 100000;  // 1000 lines * 100 characters 
    CHAR_INFO                   ch_info_model_buff[ch_buff_2d_size];
    CHAR_INFO                   ch_info_view_buff[ch_buff_2d_size];
    std::vector<CHAR_INFO>      v_chars_view;
    COORD                       coord_ch_temp_buff_size;
    COORD                       coord_read_top_left;
    BOOL                        func_success;

    //Build new char info array inserting line numbers
    int idx_line = 0;
    int idx_model_char = 0;
    int idx_view_char = 0;

    //Add the line number chars and content chars into a vector
    while (idx_model_char < model_buffer.length()) {

        //Add the line number characters first
        string str_line_idx_label = to_string(idx_line);

        for (int i = 0; i < str_line_idx_label.size(); i++) {
            CHAR_INFO ch_info_line_num;
            ch_info_line_num.Char.AsciiChar = str_line_idx_label.at(i);
            ch_info_view_buff[idx_view_char] = ch_info_line_num;
            idx_view_char++;
        }

        //Add the data model chars i.e. the real content
        int num_chars_this_line = chars_per_line - str_line_idx_label.size();
        int count_line_chars = 0;
        
        for (size_t count_line_chars = 0; 
            count_line_chars < num_chars_this_line; count_line_chars++) {

            ch_info_view_buff[idx_view_char] = ch_info_model_buff[idx_model_char];
            idx_view_char++;
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
