#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string.h>
#include "common.h"
#include "convert.h"
#include "csv2cpp.h"

using namespace std;

csv2cpp::csv2cpp(const string csv_path, const string cpp_folder, const string bin_folder)
    : csv_path(csv_path), cpp_folder(cpp_folder+"/"), bin_folder(bin_folder+"/"),
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
    for(int i=1; getline(csv, line); ++i) {
        if((i==1 && !fill_comments(line)) ||
                (i==2 && !fill_variables(line)) ||
                (i==3 && !fill_types(line)) ||
                (i==4 && !fill_attrs(line)) ||
                (i>4 && !fill_values(i, line)))
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

bool csv2cpp::gen_bin()
{
    string bin_path = bin_folder+csv_name+".bin";
    string data_path = bin_folder+csv_name+".data";
    ofstream bin;
    ofstream data;
    bin.open(bin_path.c_str());
//    data.open(data_path.c_str());
    if(bin.fail() || data.fail())
        return false;

    int bin_size = 0;
    int data_size = 0;
    char* bin_buf = new char[1024*1000];
    memset(bin_buf, 0, 1024*1000);
    char* data_buf = new char[1024*1000];
    memset(data_buf, 0, 1024*1000);

    size_t cols = comments.size();
    for(vector<vector<string> >::const_iterator it=values.begin(); it!=values.end(); ++it) {
        for(size_t i=0; i<cols; ++i) {
            write_value_bin(bin_buf, bin_size, (*it)[i], types[i]);
            write_value_data(data_buf, data_size, (*it)[i]);
        }
    }

    bin.write(bin_buf, bin_size);
    //data.write(data_buf, data_size);

    bin.close();
 //   data.close();
    return true;
}

const string csv2cpp::get_csv_error(int* el, int* en)
{
    if(el != NULL)
        *el = error_line;
    if(en != NULL)
        *en = csv_errno;
    string strerr;
    switch( csv_errno ) {
        case 0:
            strerr = "success";
            break;
        case -1:
            strerr = "variables cols mismatch";
            break;
        case -2:
            strerr = "types cols mismatch";
            break;
        case -3:
            strerr = "attrs cols mismatch";
            break;
        case -4:
            strerr = "data cols mismatch";
            break;
        case -5:
            strerr = "file not exist";
            break;
        case -6:
            strerr = "lines mismatch";
            break;
        case -7:
            strerr = "has empty string";
            break;
        default:
            strerr = "unknown";
            break;
    }
    return strerr;
}

bool csv2cpp::fill_comments(const string& line)
{
    comments = str_split(line, -1, ',');
    if( is_vector_has_empty_string(comments) ) {
        error_line = 1;
        return false;
    }

    string& last = comments[comments.size()-1];
    str_trim(last, "\r");
    str_trim(last, "\n");
    return true;
}

bool csv2cpp::fill_variables(const string& line)
{
    variables = str_split(line, -1, ',');
    if( is_vector_has_empty_string(variables) ) {
        error_line = 2;
        return false;
    }

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
    if( is_vector_has_empty_string(types) ) {
        error_line = 3;
        return false;
    }

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
    if( is_vector_has_empty_string(attrs) ) {
        error_line = 4;
        return false;
    }

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

bool csv2cpp::fill_values(int n, const string& line)
{
    values.push_back(str_split(line, -1, ','));
    if(values.back().size() != comments.size()) {
        csv_errno = -4;
        error_line = n;
        return false;
    }

    string& last = values.back()[comments.size()-1];
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

void csv2cpp::write_value_bin(char*& buf, int& tail, const string& v, const string& type)
{
    if(type == "int8") {
        signed char cv = to_int8(v.c_str());
        memcpy(buf+tail, &cv, sizeof(cv));
        tail += sizeof(cv);
    } else if(type == "int16") {
        signed short cv = to_int16(v.c_str());
        memcpy(buf+tail, &cv, sizeof(cv));
        tail += sizeof(cv);
    } else if(type == "int32") {
        signed int cv = to_int32(v.c_str());
        memcpy(buf+tail, &cv, sizeof(cv));
        tail += sizeof(cv);
    } else if(type == "int64") {
        signed long long cv = to_int64(v.c_str());
        memcpy(buf+tail, &cv, sizeof(cv));
        tail += sizeof(cv);
    } else if(type == "uint8") {
        unsigned char cv = to_uint8(v.c_str());
        memcpy(buf+tail, &cv, sizeof(cv));
        tail += sizeof(cv);
    } else if(type == "uint16") {
        unsigned short cv = to_uint16(v.c_str());
        memcpy(buf+tail, &cv, sizeof(cv));
        tail += sizeof(cv);
    } else if(type == "uint32") {
        unsigned int cv = to_uint32(v.c_str());
        memcpy(buf+tail, &cv, sizeof(cv));
        tail += sizeof(cv);
    } else if(type == "uint64") {
        unsigned long long cv = to_uint64(v.c_str());
        memcpy(buf+tail, &cv, sizeof(cv));
        tail += sizeof(cv);
    } else if(type == "string") {
        short len = v.size();
        memcpy(buf+tail, &len, sizeof(len));
        tail += sizeof(len);
        memcpy(buf+tail, v.c_str(), len);
        tail += len;
    }
}

void csv2cpp::write_value_data(char*& buf, int& tail, const string& v, bool append_space)
{
    memcpy(buf+tail, v.c_str(), v.size());
    tail += v.size();
    if( append_space )
        buf[tail++] = ' ';
}

bool csv2cpp::is_vector_has_empty_string(const vector<string>& vec)
{
    for(vector<string>::const_iterator it=vec.begin(); it!=vec.end(); ++it) {
        if( (*it).empty() ) {
            csv_errno = -7;
            return true;
        }
    }
    return false;
}

