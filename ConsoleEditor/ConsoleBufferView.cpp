#include "ConsoleBufferView.h"

/* ConsoleViewBuffer: constructor. */
// ConsoleViewBuffer::ConsoleViewBuffer() {}

/* ~ConsoleViewBuffer: destructor. */
ConsoleBufferView::~ConsoleBufferView() { }

void ConsoleBufferView::render(ConsoleBufferModel model_buffer, CONSOLE_SCREEN_BUFFER_INFO screen_info) {
    
    const int                   ch_buff_num_lines   = 1000;
    const int                   ch_buff_2d_size     = 100000;  // 1000 lines * 100 characters 
    COORD                       coord_ch_temp_buff_size;
    COORD                       coord_read_top_left;
    BOOL                        func_success;

    int screen_num_columns = screen_info.srWindow.Right - screen_info.srWindow.Left + 1;
    int screen_num_rows    = screen_info.srWindow.Bottom - screen_info.srWindow.Top + 1;
    int char_buff_size = screen_num_columns * screen_num_rows;
    CHAR_INFO * ch_info_view_buff = new CHAR_INFO[char_buff_size];

    //Build new char info array inserting line numbers
    int idx_line        = 0;
    int idx_model_char  = 0;
    int idx_view_char   = 0;

    //Add the line number chars and content chars into a vector
    int buffer_length = model_buffer.length();
    while (idx_line < screen_num_rows) {

        //Add the line number characters first
        string str_line_idx_label = to_string(idx_line);
        str_line_idx_label += ' ';
        for (int i = 0; i < str_line_idx_label.size(); i++) {
            CHAR_INFO ch_info_line_num;
            ch_info_line_num.Char.UnicodeChar = str_line_idx_label.at(i);
            ch_info_line_num.Attributes = FG_COLOR::CYAN;
            ch_info_view_buff[idx_view_char] = ch_info_line_num;
            idx_view_char++;
        }

        //Add the data model chars i.e. the real content
        int num_chars_this_line = screen_num_columns - str_line_idx_label.size();
        int count_line_chars = 0;
        
        for (size_t count_line_chars = 0; 
            count_line_chars < num_chars_this_line; count_line_chars++) {
            
            CHAR_INFO ch_info_content;

            if (idx_model_char < model_buffer.length()) {
                ch_info_content.Char.UnicodeChar = model_buffer.char_info_buffer()[idx_model_char].Char.UnicodeChar;
                idx_model_char++;
            } else {
                ch_info_content.Char.UnicodeChar = ' ';
                //ch_info_view_buff[idx_view_char] = model_buffer.char_info_buffer()[idx_model_char];
            }
           
            ch_info_content.Attributes = FG_COLOR::WHITE;
            ch_info_view_buff[idx_view_char] = ch_info_content;
            idx_view_char++;
        }

        idx_line++;
    }

    COORD coord_write_size;
    coord_write_size.Y = screen_num_rows;
    coord_write_size.X = screen_num_columns;
    
    COORD coord_write_top_left;
    coord_write_top_left.X = 0;
    coord_write_top_left.Y = 0;

    SMALL_RECT rect_write_area;
    rect_write_area.Top     = 0;
    rect_write_area.Left    = 0;
    rect_write_area.Bottom  = coord_write_size.Y - 1;
    rect_write_area.Right   = coord_write_size.X - 1;

    //m_handle_screen_buff = GetStdHandle(STD_OUTPUT_HANDLE);

    // Creates a handle to an empty console screen buffer
    m_handle_screen_buff = CreateConsoleScreenBuffer(
        GENERIC_READ |           // read/write access 
        GENERIC_WRITE,
        FILE_SHARE_READ |
        FILE_SHARE_WRITE,        // shared 
        NULL,                    // default security attributes 
        CONSOLE_TEXTMODE_BUFFER, // must be TEXTMODE 
        NULL);                   // reserved; must be NULL 


    if (m_handle_screen_buff == INVALID_HANDLE_VALUE) {
        log_wstr(L"\nDEBUG: ", L"Model m_handle_screen_buff is invalid.\n");
    }

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

    if (!SetConsoleActiveScreenBuffer(m_handle_screen_buff)) {
        printf("SetConsoleActiveScreenBuffer failed - (%d)\n", GetLastError());
        return;
    }

    move_cursor_to(1, 0, m_handle_screen_buff);

    delete[] ch_info_view_buff;
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
