#include "ConsoleBuffer.h"

ConsoleBuffer::ConsoleBuffer() {}

ConsoleBuffer::~ConsoleBuffer() {}

int ConsoleBuffer::length(){
    return m_model_lines.size(); 
}

//load_file: Load first 1000 lines into buffer
void ConsoleBuffer::load_file(const string& filename) {
    
    ifstream in(filename.c_str());
    m_model_lines.clear();

    for (int i = 0; i < 1000; ++i) {
        string str_line;
        getline(in, str_line);
        m_model_lines.push_back(str_line);
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
    int num_chars_read = 
        (rect_src_read_area.Right + 1 - rect_src_read_area.Left)
        * (rect_src_read_area.Bottom + 1 - rect_src_read_area.Top);

    m_view_char_info_buff.reset();
    m_view_char_info_buff.append(ch_info_temp_buff, 0, num_chars_read);

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

void ConsoleBuffer::build_line_num_string(size_t line_num, string& str_line_num_out) {
    
    str_line_num_out = to_string(line_num);
    if (str_line_num_out.size() == 1)
        str_line_num_out.insert(0, 1, ' ');
    str_line_num_out += ' ';
}

void ConsoleBuffer::build_line_content_string(size_t idx_line, size_t required_line_length, string& str_line_out) {

    size_t total_lines = m_model_lines.size();
    str_line_out = m_model_lines[idx_line];

    //Pad right to fill up to line length
    if(str_line_out.size() < required_line_length)
    {
        int num_fill_chars = 
            required_line_length - str_line_out.size();
        if (num_fill_chars >= 1) 
            str_line_out.append(num_fill_chars, ' ');
    } 
    else if(str_line_out.size() > required_line_length){
        //TODO: deal with too big lines, spill onto line below
    }
}

void ConsoleBuffer::render(CONSOLE_SCREEN_BUFFER_INFO screen_info) {

    const int   ch_buff_num_lines = 1000;
    const int   ch_buff_2d_size = 100000;  // 1000 lines * 100 characters 
    COORD       coord_ch_temp_buff_size;
    COORD       coord_read_top_left;

    int screen_num_columns = screen_info.srWindow.Right - screen_info.srWindow.Left + 1;
    int screen_num_rows    = screen_info.srWindow.Bottom - screen_info.srWindow.Top + 1;

    //Build new char info array inserting line numbers if necessary
    size_t idx_line        = 0;
    size_t total_screen_lines = screen_num_rows >= 0 ? screen_num_rows : 0;

    //Build char buffer line by line
    while (idx_line < total_screen_lines) {
        
        //Append line number to char info view buffer
        string str_line_num;
        this->build_line_num_string(idx_line, str_line_num);
        m_view_char_info_buff.append(str_line_num, FG_COLOR::CYAN);

        //Append line content to char info view buffer
        string str_line_content;
        size_t line_content_length = screen_num_columns - str_line_num.size();
        this->build_line_content_string(idx_line, line_content_length, str_line_content);
        m_view_char_info_buff.append(str_line_content, FG_COLOR::WHITE);

        idx_line++;
    }

    //Update the console display
    this->display(screen_info);
}

void ConsoleBuffer::display(CONSOLE_SCREEN_BUFFER_INFO screen) {

    BOOL func_success;
    int  screen_ncols = screen.srWindow.Right - screen.srWindow.Left + 1;
    int  screen_nrows = screen.srWindow.Bottom - screen.srWindow.Top + 1;
    int  char_buff_size = screen_ncols * screen_nrows;
    
    //Set buffer large enough to hold characters for one big screen
    //TODO: declare a dynamic array size.
    CHAR_INFO  ch_info_view_arr[20000]; 

    for (size_t i = 0; i < m_view_char_info_buff.size(); i++) {
        CHAR_INFO ch_info = m_view_char_info_buff.at(i);
        ch_info_view_arr[i] = ch_info;
    }

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
    coord_write_size.Y = screen.srWindow.Bottom - screen.srWindow.Top + 1;
    coord_write_size.X = screen.srWindow.Right - screen.srWindow.Left + 1;

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
        ch_info_view_arr,    // Buffer to copy from 
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

    move_cursor_to(3, 0, m_handle_screen_buff);
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
        if (m_editor_mode == NORMAL_MODE && curs_x >= 2 && m_line_numbers_on
            || curs_x >= 1 && !m_line_numbers_on) {
            move_cursor_to(--m_cursor_X, m_cursor_Y, h_screen_buff);
        }
        break;
    case UP:
        if (m_editor_mode == NORMAL_MODE && curs_y >= 1) {
            move_cursor_to(m_cursor_X, --m_cursor_Y, h_screen_buff);
        }
        break;
    case RIGHT:
        if (m_editor_mode == NORMAL_MODE && curs_x < (cols - 1) && m_line_numbers_on) {
            move_cursor_to(++m_cursor_X, m_cursor_Y, h_screen_buff);
        }
        break;
    case DOWN:
        if (m_editor_mode == NORMAL_MODE && (curs_y < rows - 1)) {
            move_cursor_to(m_cursor_X, ++m_cursor_Y, h_screen_buff);
        }
        break;

    default: break;
    }
}
