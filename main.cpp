#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "csv2cpp.h"

using namespace std;

int main(int argc, char** argv)
{
    if(argc < 4) {
        std::cout << "Usage: " << argv[0] << " csv_path cpp_folder bin_folder [err|all]" << std::endl;
        exit(-1);
    }

    bool all = (argc==4 || string(argv[4])!="err");

    const string csv_path(argv[1]);
    const string cpp_folder(argv[2]);
    const string bin_folder(argv[3]);

    csv2cpp c2c(csv_path, cpp_folder, bin_folder);
    c2c.load_csv();
    c2c.gen_cpp();
    c2c.gen_bin();

    int err_num;
    int err_line;
    string err_str = c2c.get_csv_error(&err_line, &err_num);
    if(err_num != 0) {
        cout << "[E] " << csv_path << " : " << err_str << "(" << err_line << ")" << endl;
        return -1;
    }

    if( all )
        cout << "[OK] " << csv_path << endl;

    //c2c.debug();

    return 0;
}
