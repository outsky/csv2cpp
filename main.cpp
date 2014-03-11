#include <iostream>
#include <stdlib.h>
#include "csv2cpp.h"

int main(int argc, char** argv)
{
    if(argc != 3) {
        std::cout << "Usage: " << argv[0] << " csv_path cpp_folder" << std::endl;
        exit(-1);
    }

    csv2cpp c2c(argv[1], argv[2]);
    if( !c2c.load_csv() ) {
        std::cout << "load " << argv[1] << " failed" << std::endl;
        exit(-1);
    }

    if( !c2c.gen_cpp() ) {
        std::cout << "generate " << argv[1] << " -> " << argv[2] << " failed" << std::endl;
        exit(-1);
    }

    c2c.debug();

    return 0;
}
