#ifndef CSV2CPP_H
#define CSV2CPP_H

#include <string>
#include <vector>

class csv2cpp
{
    public:
        csv2cpp(const std::string csv_path, const std::string cpp_folder, const std::string bin_folder);

        bool load_csv();
        bool gen_cpp();
        bool gen_bin();

        const std::string get_csv_error(int* error_line=NULL, int* error_num=NULL);

        void debug();

    private:
        bool fill_comments(const std::string& line);
        bool fill_variables(const std::string& line);
        bool fill_types(const std::string& line);
        bool fill_attrs(const std::string& line);
        bool fill_values(int n, const std::string& line);

        std::string header();
        std::string struct_config();
        std::string class_mgr();
        std::string footer();

        void write_value_bin(char*& buf, int& tail, const std::string& v, const std::string& type);
        void write_value_data(char*& buf, int& tail, const std::string& v, bool append_space=true);

    private:
        std::vector<std::string> comments;
        std::vector<std::string> variables;
        std::vector<std::string> types;
        std::vector<std::string> attrs;
        std::vector<std::vector<std::string> > values;

        const std::string csv_path;
        const std::string cpp_folder;
        const std::string bin_folder;
        std::string csv_name;

        //  0  - success
        //  -1 - variables count mismatch
        //  -2 - types count mismatch
        //  -3 - attrs count mismatch
        //  -4 - data count mismatch
        //  -5 - file not exist
        //  -6 - lines mismatch
        //  -7 - has empty string
        int csv_errno;
        int error_line;

        std::string date;
};

#endif
