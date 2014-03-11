#include <stdlib.h>
#include <fstream>
#include "csv2cpp.h"

csv2cpp::csv2cpp(const std::string csv_path, const std::string cpp_folder)
    : csv_path(csv_path), cpp_folder(cpp_folder),
    csv_name(csv_path), error_line(-1)
{
    size_t pos = csv_name.find_last_of("/\\");
    csv_name.assign(csv_name.substr(pos+1));
    pos = csv_name.find_last_of(".");
    csv_name.assign(csv_name.substr(0, pos));
}

bool csv2cpp::load_csv()
{
    int csv_errno = get_csv_error();
    if(csv_errno != 0)
        return false;
    return true;
}

bool csv2cpp::gen_cpp()
{
    return true;
}

// check whether csv is valid, save the error line in error_line, and
// return the errno
//
// return:
//  0  - success
//  -1 - variables count mismatch
//  -2 - types count mismatch
//  -3 - attrs count mismatch
//  -4 - data count mismatch
int csv2cpp::get_csv_error()
{
    error_line = -1;
    ifstream csv();

    return 0;
}
