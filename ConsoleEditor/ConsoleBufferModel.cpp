#include "ConsoleBufferModel.h"

/* ConsoleModelBuffer: constructor. */
// ConsoleModelBuffer::ConsoleModelBuffer() {}

//Console - Console object constructor, takes handle to console screen buffer
//          Saves the state of the console so it can be retrieved.
void ConsoleBufferModel::copy_from_buffer(HANDLE h_source_buffer)
{
    SMALL_RECT rect_src_read_area;
    CHAR_INFO ch_info_temp_buff[100000]; // [1000][100]; 1000 lines * 100 characters 
    COORD coord_ch_temp_buff_size;
    COORD coord_start_temp_buff;
    CONSOLE_SCREEN_BUFFER_INFO src_screen_info;
    BOOL func_success;

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
    rect_src_read_area.Top       = src_screen_info.dwCursorPosition.Y - 1001;
    rect_src_read_area.Left      = 0;
    rect_src_read_area.Bottom    = src_screen_info.dwSize.Y - 1;
    rect_src_read_area.Right     = src_screen_info.dwSize.X - 1;
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
    
    m_vect_char_buffer.insert(
        m_vect_char_buffer.end(), 
        &ch_info_temp_buff[0], 
        &ch_info_temp_buff[num_chars_read - 1]
    );

    size(src_screen_info.dwSize);
}

/* ~ConsoleModelBuffer: destructor. */
ConsoleBufferModel::~ConsoleBufferModel() {}


