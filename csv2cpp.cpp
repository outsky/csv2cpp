#include <iostream>
#include <stdlib.h>
#include <fstream>
#include "common.h"
#include "csv2cpp.h"

using namespace std;

csv2cpp::csv2cpp(const string csv_path, const string cpp_folder)
    : csv_path(csv_path), cpp_folder(cpp_folder),
    csv_name(csv_path), csv_errno(0), error_line(-1)
{
    size_t pos = csv_name.find_last_of("/\\");
    csv_name.assign(csv_name.substr(pos+1));
    pos = csv_name.find_last_of(".");
    csv_name.assign(csv_name.substr(0, pos));

    time_t now = time(NULL);
    date = ctime(&now);
}

bool csv2cpp::load_csv()
{
    ifstream csv;
    csv.open(csv_path.c_str());
    if( csv.fail() ) {
        csv_errno = -5;
        return false;
    }

    string line;
    for(int i=1; i<=4 && getline(csv, line); ++i) {
        if((i==1 && !fill_comments(line)) ||
                (i==2 && !fill_variables(line)) ||
                (i==3 && !fill_types(line)) ||
                (i==4 && !fill_attrs(line)))
            goto FAILED;
    }

    csv.close();
    return true;

FAILED:
    csv.close();
    return false;
}

bool csv2cpp::gen_cpp()
{
    string cpp_path = cpp_folder+csv_name+".h";
    ofstream cpp;
    cpp.open(cpp_path.c_str());
    if( cpp.fail() )
        return false;

    string ctx;
    ctx += header();
    ctx += struct_config();
    ctx += class_mgr();
    ctx += footer();

    cpp.write(ctx.c_str(), ctx.size());

    cpp.close();
    return true;
}

int csv2cpp::get_csv_error(int* el)
{
    if(el != NULL)
        *el= error_line;
    return csv_errno;
}

bool csv2cpp::fill_comments(const string& line)
{
    comments = str_split(line, -1, ',');
    string& last = comments[comments.size()-1];
    str_trim(last, "\r");
    str_trim(last, "\n");
    return true;
}

bool csv2cpp::fill_variables(const string& line)
{
    variables = str_split(line, -1, ',');
    if(variables.size() != comments.size()) {
        csv_errno = -1;
        error_line = 2;
        return false;
    }

    string& last = variables[variables.size()-1];
    str_trim(last, "\r");
    str_trim(last, "\n");

    return true;
}

bool csv2cpp::fill_types(const string& line)
{
    types = str_split(line, -1, ',');
    if(types.size() != comments.size()) {
        csv_errno = -2;
        error_line = 3;
        return false;
    }

    string& last = types[types.size()-1];
    str_trim(last, "\r");
    str_trim(last, "\n");

    return true;

}

bool csv2cpp::fill_attrs(const string& line)
{
    attrs = str_split(line, -1, ',');
    if(attrs.size() != comments.size()) {
        csv_errno = -3;
        error_line = 4;
        return false;
    }

    string& last = attrs[attrs.size()-1];
    str_trim(last, "\r");
    str_trim(last, "\n");

    return true;
}

void csv2cpp::debug()
{
    int size = comments.size();
    for(int i=0; i<size; ++i)
        cout << comments[i] << "\t";
    cout << endl;

    for(int i=0; i<size; ++i)
        cout << variables[i] << "\t";
    cout << endl;

    for(int i=0; i<size; ++i)
        cout << types[i] << "\t";
    cout << endl;

    for(int i=0; i<size; ++i)
        cout << attrs[i] << "\t";
    cout << endl;
}

string csv2cpp::header()
{
    return "// Auto generate by tool at " + date + "\n" +
        "#ifndef " + csv_name + "_H\n" +
        "#define " + csv_name + "_H\n" +
        "#include \"configdef.hpp\"\n\n";
}

string csv2cpp::struct_config()
{
    string ret = "struct " + csv_name + "Config\n" +
        "{\n";
    size_t size = comments.size();
    for(size_t i=0; i<size; ++i)
        ret += ("\t"+types[i]+" "+variables[i]+";\t"+"// "+comments[i]+"\n");
    ret += "};\n\n";
    return ret;
}

string csv2cpp::class_mgr()
{
    string ret = "class " + csv_name + "ConfigMgr\n" +
        "{\n" +
        "\ttypedef map<int, " + csv_name + "Config> MAP_" + csv_name + ";\n" +
        "public:\n" +
        "\tint LoadData(const char* filename)\n" +
        "\t{\n" +

        "\t\tifstream newfile ( filename, ios::binary);\n" +
        "\t\twhile (!newfile.eof())\n" +
        "\t\t{\n" +
        "\t\t\t" + csv_name + "Config config;\n" +
        "\t\t\tmemset(&config, 0, sizeof(config));\n";

    size_t size = variables.size();
    for(size_t i=0; i<size; ++i)
        ret += "\t\t\tnewfile.read((char*)&config." + variables[i] + ", sizeof(config." + variables[i] + "));\n";

    ret += "\t\t\tmap_" + csv_name + ".insert(MAP_" + csv_name + "::value_type(config." + variables[0] + ", config));\n" +
        "\t\t}\n" +
        "\t\tnewfile.close();\n" +
        "\t\tmap_" + csv_name + ".erase(0);\n" +
        "\t\treturn 0;\n" +
    "\t}\n\n" +

    "\t" + csv_name + "Config* GetConfig(uint32 key)\n" +
    "\t{\n" +
        "\t\tMAP_" + csv_name + "::iterator it = map_" + csv_name + ".find(key);\n" +
        "\t\tif (it != map_" + csv_name + ".end())\n" +
        "\t\t{\n" +
            "\t\t\treturn &it->second;\n" +
        "\t\t}\n" +
        "\t\treturn NULL;\n"
    "\t}\n\n" +

    "\t" + csv_name + "Config* GetFirst()\n" +
    "\t{\n" +
        "\t\tit = map_" + csv_name + ".begin();\n" +
        "\t\tif (it != map_" + csv_name + ".end())\n" +
        "\t\t{\n" +
            "\t\t\treturn &it->second;\n" +
        "\t\t}\n" +
        "\t\treturn NULL;\n" +
    "\t}\n\n" +

    "\t" + csv_name + "Config* GetNext()\n" +
    "\t{\n" +
        "\t\tit++;\n" +
        "\t\tif (it != map_" + csv_name + ".end())\n" +
        "\t\t{\n" +
            "\t\t\treturn &it->second;\n" +
        "\t\t}\n" +
        "\t\treturn NULL;\n" +
    "\t}\n\n" +
"private:\n" +
    "\tMAP_" + csv_name + "::iterator it;\n" +
    "\tMAP_" + csv_name + " map_" + csv_name + ";\n" +
"};\n\n";

    return ret;
}

string csv2cpp::footer()
{
    return "#endif // " + csv_name + "_H\n";
}
