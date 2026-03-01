#include <cstring>
#include <string>
#include <vector>
#include <cstdint>
#include <cstdlib>
#include <array>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

typedef int64_t section_t;

std::vector<std::wstring> nspace;
section_t section = 0;

bool is_numeric(const char* text) {
    for (const char* i = text; *i != NULL; i++) {
        if (*i >= '0' && * i <= '9') {
            ;
        }
        else
            return false;
    }
    return true;
}

std::wstring join(const std::vector<std::wstring> &strings, std::wstring connector) {
    std::wstring result = L"";
    for (uint64_t i = 0; i < strings.size(); i++) {
        result += strings[i];
        if (i == strings.size() - 1)
            break;
        result += connector;
    }
    return result;
}

std::array<std::wstring, 1> get_paths() {
    std::wstring path1(L"/usr/docs/");
    path1 += join(nspace, L"/") + L".";
    path1 += std::to_wstring(section);
    return {path1};
}

void parse_args(int argc, char* argv[]) {
    for (int i = 1; i < argc; i++) {
        char* arg = argv[i];
        if (arg[0] == '-') {
            if (is_numeric(arg + 1)) {
                if (section != 0) {
                    fprintf(stderr, "Error: Repeated section argument: %s (repeated since -%lli)\n", arg, section);
                    abort();
                }
                section = atoll(arg + 1);
            } else {
                fprintf(stderr, "Error: non-numeric section argument: %s\n", arg);
                abort();
            }
            continue;
        }
        std::wstring string_arg(&arg[0], &arg[strlen(arg)]);
        nspace.push_back(string_arg);
    }
}

inline bool exists(const std::string& name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
}

std::string wstring_to_string(const std::wstring& wstr)
{
    std::string result;
    result.reserve(wstr.size());
    for (wchar_t wc : wstr) {
        if (wc < 0 || wc > 127) {
            abort();  // Malformed input (not pure ASCII)
        }
        result.push_back(static_cast<char>(wc));
    }
    return result;
}

int main(int argc, char* argv[]) {
    parse_args(argc, argv);
    auto paths = get_paths();
    for (std::wstring path : paths) {
        std::string spath = wstring_to_string(path);
        if (exists(spath)) {
            std::cout << "Path found: ";
            std::wcout << path;
            std::cout << std::endl;
            execvp("man", (char* const[]){"man", spath.data(), NULL});
        }
        std::cout << "Path not found: ";
        std::wcout << path;
        std::cout << "\n";
    }
}
