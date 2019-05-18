#include "filemanagment.h"

bool FileManagment::exists(const string& file){
    ifstream f(file.c_str());
    return f.good();
}
