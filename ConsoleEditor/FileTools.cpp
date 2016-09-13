#include "FileTools.h"

FileTools::FileTools(){}

FileTools::~FileTools(){}

bool FileTools::file_exists(const string& name) {
    if (FILE *file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    }
    else {
        return false;
    }
}