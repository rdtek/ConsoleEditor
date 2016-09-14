#include "ConsoleBuffer.h"

ConsoleBuffer::ConsoleBuffer() {}

ConsoleBuffer::~ConsoleBuffer() {}

int ConsoleBuffer::length(){
    return m_model_char_info_buff.size(); 
}

void ConsoleBuffer::append(string& str) {

    for (string::iterator it = str.begin(); it < str.end(); ++it) {
        CHAR_INFO char_info;
        char_info.Char.UnicodeChar = *it;
        m_view_char_info_buff.append(char_info);
    }
}

//load_file: Load first 1000 lines into buffer
void ConsoleBuffer::load_file(const string& filename) {
    
    ifstream in(filename.c_str());
    
    for (int i = 0; i < 1000; ++i) {
        string str_line;
        string str_new_line("\n");
        getline(in, str_line);
        this->append(str_line);
        this->append(str_new_line);
    }
}

void ConsoleBuffer::load_buffer(HANDLE h_source_buffer)
{
    SMALL_RECT                  rect_src_read_area;
    CHAR_INFO                   ch_info_temp_buff[100000]; // [1000][100]; 1000 lines * 100 characters 
    COORD                       coord_ch_temp_buff_size;
    COORD                       coord_start_temp_buff;
    CONSOLE_SCREEN_BUFFER_INFO  src_screen_info;
    BOOL                        func_success;

    // Check source buffer is valid
    if (h_source_buffer == INVALID_HANDLE_VALUE) {
        printf("CreateConsoleScreenBuffer failed - (%d)\n", GetLastError());
        return;
    }

    // Get the cursor position from the source buffer
    GetConsoleScreenBufferInfo(h_source_buffer, &src_screen_info);
    m_cursor_X = src_screen_info.dwCursorPosition.X;
    m_cursor_Y = src_screen_info.dwCursorPosition.Y;

    // Set rectangle defining portion of source buffer to copy
    rect_src_read_area.Top = src_screen_info.dwCursorPosition.Y - 1001;
    rect_src_read_area.Left = 0;
    rect_src_read_area.Bottom = src_screen_info.dwSize.Y - 1;
    rect_src_read_area.Right = src_screen_info.dwSize.X - 1;
    if (rect_src_read_area.Top < 0) rect_src_read_area.Top = 0;
    if (rect_src_read_area.Bottom < 0) rect_src_read_area.Bottom = 0;

    // The temporary buffer size
    coord_ch_temp_buff_size.Y = 100;
    coord_ch_temp_buff_size.X = rect_src_read_area.Right + 1;

    // The top left destination cell of the temporary buffer is 
    coord_start_temp_buff.X = 0;
    coord_start_temp_buff.Y = 0;

    // Copy the block from the screen buffer to the temp buffer. 
    func_success = ReadConsoleOutput(
        h_source_buffer,          // screen buffer to read from 
        ch_info_temp_buff,        // buffer to copy into 
        coord_ch_temp_buff_size,  // col-row size of ch_info_buff 
        coord_start_temp_buff,    // top left dest. cell in ch_info_buff 
        &rect_src_read_area       // screen buffer source rectangle 
    );

    if (!func_success) {
        printf("ReadConsoleOutput failed - (%d)\n", GetLastError());
        return;
    }

    //Load the characters into a vector
    int num_chars_read = (rect_src_read_area.Right + 1 - rect_src_read_area.Left)
        * (rect_src_read_area.Bottom + 1 - rect_src_read_area.Top);

    m_view_char_info_buff.insert(0, &ch_info_temp_buff[0], num_chars_read - 1);

    size(src_screen_info.dwSize);
}

/* size: set the screen size for this console. */
void ConsoleBuffer::size(COORD screen_size) {
    if (SetConsoleScreenBufferSize(m_handle_screen_buff, screen_size)) {
        //m_width = screen_size.X;
        //m_height = screen_size.Y;
    };
}

/* size: set the screen size for this console. */
void ConsoleBuffer::size(int width, int height) {
    COORD screen_size;
    screen_size.X = width;
    screen_size.Y = height;
    size(screen_size);
}

int ConsoleBuffer::width() {
    CONSOLE_SCREEN_BUFFER_INFO  screen_info;
    GetConsoleScreenBufferInfo(m_handle_screen_buff, &screen_info);
    return screen_info.dwSize.X; 
}

int ConsoleBuffer::height() {
    CONSOLE_SCREEN_BUFFER_INFO  screen_info;
    GetConsoleScreenBufferInfo(m_handle_screen_buff, &screen_info);
    return  screen_info.dwSize.Y;
}

void ConsoleBuffer::char_buffer_array(CHAR_INFO *char_info_buff_out) {

    COORD coord_buff_size;
    COORD coord_destination;
    SMALL_RECT read_rect;

    read_rect.Top = 0;
    read_rect.Left = 0;
    read_rect.Right = coord_buff_size.X - 1;
    read_rect.Bottom = coord_buff_size.Y - 1;

    ReadConsoleOutput(
        m_handle_screen_buff, // screen buffer to read from 
        char_info_buff_out,   // buffer to copy into 
        coord_buff_size,      // col-row size of char_info_buff_out 
        coord_destination,    // top left destination cell in char_buff_out 
        &read_rect
    );
}

void ConsoleBuffer::insert_line_num(size_t line_num, size_t *line_num_size_out) {
   
    string str_line_idx_label = to_string(line_num);
    if (str_line_idx_label.size() == 1)
        str_line_idx_label.insert(0, 1, ' ');
    str_line_idx_label += ' ';
    *line_num_size_out = str_line_idx_label.size();

    for (int i = 0; i < str_line_idx_label.size(); i++) {
        CHAR_INFO ci_line_num;
        ci_line_num.Char.UnicodeChar = str_line_idx_label.at(i);
        ci_line_num.Attributes = FG_COLOR::CYAN;
        m_view_char_info_buff.append(ci_line_num);
    }
}

int ConsoleBuffer::insert_line_chars(size_t num_chars_this_line, size_t idx_model_buff_begin, size_t idx_model_buff_end) {

    int num_chars_inserted;
    int idx_view_char = 0;
    int count_line_chars = 0;
    int model_buff_max = this->length();

    //Get the range of chars from the model vector
    m_model_char_info_buff[idx_model_buff_begin];
    m_model_char_info_buff[idx_model_buff_begin + num_chars_this_line];

    vector<CharInfoBuffer>::const_iterator first = m_model_char_info_buff.begin() + idx_model_buff_begin;
    vector<CharInfoBuffer>::const_iterator last  = m_model_char_info_buff.begin() + idx_model_buff_begin + num_chars_this_line;
    //m_view_char_info_buff.insert()
    //a.insert(a.end(), b.begin(), b.end());

    //Insert this range of chars into the view vector
    //m_view_char_info_buff

    for (size_t count_line_chars = 0; count_line_chars < num_chars_this_line; count_line_chars++) {

        CHAR_INFO ch_info_view;

        //Get the char from the model buffer
        if (idx_model_buff < model_buff_max) {
            CHAR_INFO ch_info_temp;
            ch_info_temp.Char.UnicodeChar = m_model_char_info_buff[idx_model_char].Char.UnicodeChar;

            if (ch_info_temp.Char.UnicodeChar == '\n') {
                //Fill space to end of line
                ch_info_view_buff.set_range(' ', idx_view_char, num_chars_this_line - count_line_chars);
            }
            else {
                ch_info_view.Char.UnicodeChar = ch_info_temp.Char.UnicodeChar;
            }

            idx_model_char++;
        }
        else { /* Fill the remaining cells with blank spaces. */
            ch_info_view.Char.UnicodeChar = ' ';
        }

        ch_info_view.Attributes = FG_COLOR::WHITE;

        idx_view_char++;
    }

    return num_chars_inserted;
}

void ConsoleBuffer::render(CONSOLE_SCREEN_BUFFER_INFO screen_info) {

    const int   ch_buff_num_lines = 1000;
    const int   ch_buff_2d_size = 100000;  // 1000 lines * 100 characters 
    COORD       coord_ch_temp_buff_size;
    COORD       coord_read_top_left;

    int screen_num_columns  = screen_info.srWindow.Right - screen_info.srWindow.Left + 1;
    int screen_num_rows     = screen_info.srWindow.Bottom - screen_info.srWindow.Top + 1;
    int char_buff_size      = screen_num_columns * screen_num_rows;
    
    CHAR_INFO * ptr_ch_info_view_arr = new CHAR_INFO[char_buff_size];

    //Build new char info array inserting line numbers if necessary
    int idx_line        = 0;
    while (idx_line < screen_num_rows) {
        size_t line_num_size_out;
        this->insert_line_num(idx_line, &line_num_size_out);
        this->insert_line_chars(screen_num_columns - line_num_size_out);
        idx_line++;
    }

    //Update the console display
    m_view_char_info_buff.to_array(ptr_ch_info_view_arr);
    this->display(screen_info, ptr_ch_info_view_arr);

    delete[] ptr_ch_info_view_arr;
}

void ConsoleBuffer::display(CONSOLE_SCREEN_BUFFER_INFO screen_info, CHAR_INFO* ptr_char_info_arr) {

    BOOL        func_success;

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

    COORD coord_write_size;
    coord_write_size.Y = screen_info.dwSize.Y;
    coord_write_size.X = screen_info.dwSize.X;

    COORD coord_write_top_left;
    coord_write_top_left.X = 0;
    coord_write_top_left.Y = 0;

    SMALL_RECT rect_write_area;
    rect_write_area.Top = 0;
    rect_write_area.Left = 0;
    rect_write_area.Bottom = coord_write_size.Y - 1;
    rect_write_area.Right = coord_write_size.X - 1;

    func_success = WriteConsoleOutput(
        m_handle_screen_buff,    // Screen buffer to write to 
        ptr_char_info_arr,        // Buffer to copy from 
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
}

/* move_cursor_to: move the cursor to specified column and line number. */
void ConsoleBuffer::move_cursor_to(int column, int line, HANDLE h_screen_buff)
{
    COORD coord;
    m_cursor_X = coord.X = column;
    m_cursor_Y = coord.Y = line;

    if (!SetConsoleCursorPosition(h_screen_buff, coord))
    {
        // TODO: Call GetLastError() to get a more specific error code.
        log_wstr(L"move_cursor_to(): ", L"Error setting cursor position.");
    }
}

/* refresh_cursor: cause the cursor to be shown in current position. */
void ConsoleBuffer::refresh_cursor() {
    if (cursor_X() >= 0 && cursor_Y() >= 0) {
        move_cursor_to(cursor_X(), cursor_Y(), m_handle_screen_buff);
    }
}

/* move_cursor: move the cursor in specified direction by specified amount. */
void ConsoleBuffer::move_cursor(DIRECTION direction, int distance) {

    int curs_x = m_cursor_X;
    int curs_y = m_cursor_Y;
    int cols = width();
    int rows = height();
    HANDLE h_screen_buff = m_handle_screen_buff;

    switch (direction) {
    case LEFT:
        if (editor_mode == NORMAL_MODE && curs_x >= 2 && m_line_numbers_on
            || curs_x >= 1 && !m_line_numbers_on) {
            move_cursor_to(--m_cursor_X, m_cursor_Y, h_screen_buff);
        }
        break;
    case UP:
        if (editor_mode == NORMAL_MODE && curs_y >= 1) {
            move_cursor_to(m_cursor_X, --m_cursor_Y, h_screen_buff);
        }
        break;
    case RIGHT:
        if (editor_mode == NORMAL_MODE && curs_x < (cols - 1) && m_line_numbers_on) {
            move_cursor_to(++m_cursor_X, m_cursor_Y, h_screen_buff);
        }
        break;
    case DOWN:
        if (editor_mode == NORMAL_MODE && (curs_y < rows - 1)) {
            move_cursor_to(m_cursor_X, ++m_cursor_Y, h_screen_buff);
        }
        break;

    default: break;
    }
}
