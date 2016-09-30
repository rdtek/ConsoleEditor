#pragma once
#include <Windows.h>
#include <vector>
#include <sstream>
#include <string>
#include <stdio.h>
#include <wchar.h>
#include <ctime>
#include <chrono>
#include <math.h> 

using namespace std;

void log_time   (const char*    note);
void log_int    (const char*    note,   int             intVal);
void log_long   (const char*    note,   long            longVal);
void log_float  (const char*    note,   float           floatVal);
void log_dbl    (const char*    note,   double          doubleVal);
void log_wchar  (const wchar_t* note,   wchar_t         wcharVal);
void log_cstr   (const char*    note,   const char*     cstrVal);
void log_wstr   (const wchar_t* note,   const wchar_t*  strVal);