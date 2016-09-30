#include "Logging.h"

const char* LOG_FILE_NAME = "C:\\temp\\console_editor_log.txt";
vector<string> m_vect_log_buff;

void log_int(const char* note, int intVal) {

    FILE *pfile = NULL;
    pfile = fopen(LOG_FILE_NAME, "a");

    if (pfile == NULL) {
        printf("Error opening %s for writing.", LOG_FILE_NAME);
    }
    else {
        fputs("\n", pfile);
        fputs(note, pfile);
        fputs(" ", pfile);
        fprintf(pfile, "%d", intVal);
    }
    fclose(pfile);
}

void log_long(const char* note, long longVal) {

    FILE *pfile = NULL;
    pfile = fopen(LOG_FILE_NAME, "a");

    if (pfile == NULL) {
        printf("Error opening %s for writing.", LOG_FILE_NAME);
    }
    else {
        fputs("\n", pfile);
        fputs(note, pfile);
        fputs(" ", pfile);
        fprintf(pfile, "%ld", longVal);
    }
    fclose(pfile);
}

void log_float(const char* note, float floatVal) {

    FILE *pfile = NULL;
    pfile = fopen(LOG_FILE_NAME, "a");

    if (pfile == NULL) {
        printf("Error opening %s for writing.", LOG_FILE_NAME);
    }
    else {
        fputs("\n", pfile);
        fputs(note, pfile);
        fputs(" ", pfile);
        fprintf(pfile, "%f", floatVal);
    }
    fclose(pfile);
}

void log_dbl(const char* note, double doubleVal) {

    FILE *pfile = NULL;
    pfile = fopen(LOG_FILE_NAME, "a");

    if (pfile == NULL) {
        printf("Error opening %s for writing.", LOG_FILE_NAME);
    }
    else {
        fputs("\n", pfile);
        fputs(note, pfile);
        fputs(" ", pfile);
        fprintf(pfile, "%f", doubleVal);
    }
    fclose(pfile);
}

void log_wchar(const wchar_t* note, wchar_t wcharVal) {
    FILE *pfile = NULL;
    pfile = fopen(LOG_FILE_NAME, "a");

    if (pfile == NULL) {
        printf("Error opening %s for writing.", LOG_FILE_NAME);
    }
    else {
        fputws(L"\n", pfile);
        fputws(note, pfile);
        fputws(L"[", pfile);
        fputwc(wcharVal, pfile);
        fputws(L"]", pfile);
    }
    fclose(pfile);
}

void log_cstr(const char* note, const char* cstrVal) {

    FILE *pfile = NULL;
    pfile = fopen(LOG_FILE_NAME, "a");

    if (pfile == NULL) {
        printf("Error opening %s for writing.", LOG_FILE_NAME);
    }
    else {
        fputs("\n", pfile);
        fputs(note, pfile);
        fputs(" ", pfile);
        fprintf(pfile, "%s", cstrVal);
    }
    fclose(pfile);
}

void log_wstr(const wchar_t* note, const wchar_t* strVal) {

    FILE *pfile = NULL;
    pfile = fopen(LOG_FILE_NAME, "a");

    if (pfile == NULL) {
        printf("Error opening %s for writing.", LOG_FILE_NAME);
    }
    else {
        fputws(L"\n", pfile);
        fputws(note, pfile);
        fputws(L" ", pfile);
        fputws(strVal, pfile);
    }
    fclose(pfile);
}

void log_time(const char* note) {

    ostringstream str_stream;
    SYSTEMTIME sys_time;

    GetLocalTime(&sys_time);

    str_stream 
        << sys_time.wYear << "-" << sys_time.wMonth << "-" << sys_time.wDay << " "
        << sys_time.wHour << ":" << sys_time.wMinute << "::" << sys_time.wSecond 
        << "." << sys_time.wMilliseconds;

    //TODO: check if need to flush log buffer
    //flush_log_buff()
}

void flush_log_buff() {
    
    FILE *pfile = NULL;
    pfile = fopen(LOG_FILE_NAME, "a");
    //TODO: write all the log strings from the vector to the log file

    if (m_vect_log_buff.size() < 20) return;

    if (pfile == NULL) {
        printf("Error opening %s for writing.", LOG_FILE_NAME);
    }
    else {
        //TODO loop through the log buffer vector and print each string to file
        //fprintf(pfile,"\n%s ");
        //fputs(note, pfile);
    }
    fclose(pfile);
}