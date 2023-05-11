#ifndef HEADER_FILE_HELPER
#define HEADER_FILE_HELPER

#include <functional>
#include <iostream>

struct pair_hash {
    template <class T1, class T2>
    inline std::size_t operator () (const std::pair<T1,T2> &p) const {
        return std::hash<T1>{}(p.first) ^ std::hash<T2>{}(p.second);
    }
};

bool is_spaces(const char ch);

bool is_digit(const char ch);

bool is_num(const char* str);

bool is_hex(const char* str);

unsigned long int atohl(const char* str);

bool is_alpha(const char ch);

bool is_delimeter(const char ch);

void skip_spaces(std::istream& Input);

bool new_line(std::istream& Input);

size_t skip_eol(std::istream& Input);

bool is_register(const char* str);

bool skip_symol(char ch, std::istream& Input);

bool is_eos(std::istream& Input);

#endif // HEADER_FILE_HELPER
