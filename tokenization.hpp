#pragma once
#include "stdafx.hpp"
#define TAS(X) (X << 2)
enum class TokenType{
    RESERVED,
    ALNUM,
    NUM,
    _EOF,
};

struct Token{
    TokenType type;
    std::string::const_iterator begin;
    std::string::const_iterator end;
    inline std::string str() const{
        return std::string(begin, end);
    }
};


class Tokenizer{
public:
    inline Tokenizer(const std::string& str) : m_str(str){}
    std::vector<Token> tokenize();
    
private:
    inline std::optional<std::string::const_iterator> consume(){
        return std::next(m_str.begin(), m_index++);
    }
    inline std::optional<std::string::const_iterator> peak(int ahead = 1){
        if(m_index + ahead > m_str.length()){
            return std::nullopt;
        }else{
            return std::next(m_str.begin(), m_index);
        }
    }
    inline std::string::const_iterator at(){
        return std::next(m_str.begin(), m_index);
    }
    const std::string& m_str;
    size_t m_index = 0;
};
