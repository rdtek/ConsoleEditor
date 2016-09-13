#pragma once
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class FileTools
{
public:
    FileTools();
    ~FileTools();

    static bool file_exists(const string& name);
};

