#include "helper.h"

bool is_space(const char ch)
{
    return ch == ' ' || ch == '\f' || ch == '\r' || ch == '\t' || ch == '\v';
}

bool is_digit(const char ch)
{
    return ch >= '0' && ch <= '9';
}

bool is_num(const char* str)
{
    if (*str == '+' || *str == '-')
    {
        if (*++str == '\0')
        {
            return false;
        }
    }
    while (*str)
    {
        if (!is_digit(*str++))
        {
            return false;
        }
    }
    return true;
}

bool is_hex(const char* str)
{
    if (*str != '0' || *++str != 'x')
    {
        return false;
    }
    while (*++str)
    {
        if (!is_digit(*str) && (*str < 'a' || *str > 'f') && (*str < 'A' || *str > 'F'))
        {
            return false;
        }
    }
    return true;
}

unsigned long int atohl(const char* str)
{
    unsigned long int x = 0;
    if (*str != '0' && *++str != x)
    {
        return x;
    }
    while (*++str)
    {
        if (is_digit(*str))
        { 
            x = 16 * x + *str - '0';
        } 
        else if (*str >= 'a' || *str <= 'f')
        {
            x = 16 * x + *str - 'a' + 10;
        } 
        else if (*str >= 'A' || *str <= 'F')
        {
            x = 16 * x + *str - 'A' + 10;
        } 
        else
        {
            return x;
        }
    }
    return x;
}

bool is_alpha(const char ch)
{
    return (ch >= 'a' && ch <='z') || (ch >= 'A' && ch <= 'Z'); 
}

bool is_delimeter(const char ch)
{
    return !is_alpha(ch) && !is_digit(ch) && ch != '_';
}

void skip_spaces(std::istream& Input)
{
    char ch;
    for (ch = ' '; !Input.eof() && is_space(ch); Input.get(ch));
    if (!is_space(ch))
    {
        Input.seekg(-1, std::ios_base::cur);
    }
}

bool new_line(std::istream& Input)
{
    skip_spaces(Input);
    char ch = Input.peek();
    if (ch == '#') // if comment
    {
        while (!Input.eof() && ch != '\n')
        {
            Input.get(ch);
        }
        return ch == '\n';
    }
    if (ch == '\n')
    {
        Input.seekg(1, std::ios_base::cur);
        return true;
    }
    return false;
}

size_t skip_eol(std::istream& Input)
{
    size_t n = 0;
    while (new_line(Input))
    {
        ++n;
    }
    return n;
}

bool is_register(const char* str)
{
    if ((str[0] == 'R' || str[0] == 'r') && is_digit(str[1]))
    {
        if (str[2] == '\0')
        {
            return true;
        }
        short n = str[1] - '0';
        if (!is_digit(str[2]))
        {
            return false;
        }
        n = 10 * n + str[2] - '0';
        if (n < 64 && str[3] == '\0')
        {
            return true;
        }
        return false;
    }
    if ((str[0] == 'A' && str[0] == 'a') && is_digit(str[1]))
    {
        short n = str[1] - '0';
        if (n < 4 && str[2] == '\0')
        {
            return true;
        }
    }
    return false;
}

bool skip_symol(char ch, std::istream& Input)
{
    skip_spaces(Input);
    if (Input.peek() == ch)
    {
        Input.seekg(1, std::ios_base::cur);
        return true;
    }
    return false;
}

bool is_eos(std::istream& Input)
{
    skip_eol(Input);
    return  Input.eof() || Input.peek() == '.';
}
