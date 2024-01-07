#pragma once

#include "stdafx.hpp"
#include "tokenization.hpp"
#include "oblate.hpp"
class Error{
public:
    inline Error(const std::string& str): m_str(str){
        firsts.push_back(str.begin());
        for(std::string::const_iterator it = str.begin(); it <= str.end(); it++){
            if(*(it - 1) == '\n'){
                firsts.push_back(it);
            }
        }
    }
    inline void pthrow(const std::string::const_iterator c, const char *fmt, ...) const {
        va_list ap;
        va_start(ap, fmt);
        int row = 0;
        std::string::const_iterator first;
        for(int i = 0; i < firsts.size() - 1; i++){
            if(firsts[i] <= c && c < firsts[ i+ 1]){
                row = i + 1;
                first = firsts[i];
                break;
            }
        }
        int pos = std::distance(first, c);
        std::string str = std::string(first, firsts[row] - 1); // firsts[row] - 1: ignore return code
        std::string space = "";
        for(int i = 0; i < pos; i++){
            space += " ";
        }

        std::cerr << row << ", " << pos << std::endl;
        std::cerr << str << std::endl;
        std::cerr << space << "^" << std::endl;
        vfprintf(stderr, fmt, ap);
        std::cerr << std::endl;
        exit(EXIT_FAILURE);
    }
private:
    const std::string& m_str;
    std::vector<std::string::const_iterator> firsts;
};
