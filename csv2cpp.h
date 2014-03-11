#ifndef CSV2CPP_H
#define CSV2CPP_H

#include <string>
#include <vector>

class csv2cpp
{
    public:
        csv2cpp(const std::string csv_path, const std::string cpp_folder);

        bool load_csv();
        bool gen_cpp();

    private:
        int get_csv_error();

    private:
        std::vector<std::string> comments;
        std::vector<std::string> variables;
        std::vector<std::string> types;
        std::vector<std::string> attrs;

        const std::string csv_path;
        const std::string cpp_folder;
        std::string csv_name;
        int error_line;
};

#endif
