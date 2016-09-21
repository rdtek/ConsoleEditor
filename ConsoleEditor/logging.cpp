#include "Logging.h"

const char* LOG_FILE_NAME = "C:\\temp\\console_editor_log.txt";

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

void log_time(const char* note) {

    FILE *pfile = NULL;
    pfile = fopen(LOG_FILE_NAME, "a");

    unsigned long total_ms      = round(system_clock::now().time_since_epoch() / milliseconds(1));
    unsigned long ms            = total_ms % 1000;

    unsigned long total_seconds = round(total_ms / 1000);
    unsigned long seconds       = total_seconds % 60;
    
    unsigned long total_minutes = round(total_seconds / 60);
    unsigned long minutes       = total_minutes % 60;
    
    unsigned long total_hours   = round(total_minutes / 60);
    unsigned long hours         = total_hours % 24;

    if (pfile == NULL) {
        printf("Error opening %s for writing.", LOG_FILE_NAME);
    }
    else {
        fputs("\n", pfile);
        fputs(note, pfile);
        fputs(" ", pfile);
        fprintf(pfile, "%lu:%lu:%lu.%lu", hours, minutes, seconds, ms);
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