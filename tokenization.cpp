#include "tokenization.hpp"


std::vector<Token> Tokenizer::tokenize(){
    std::vector<Token> tokens;
    while(peak().has_value()){
        //space
        if(std::isspace(*peak().value())){
            consume();
            continue;
        //alnum
        }else if(std::isalpha(*peak().value())){
            std::string::const_iterator begin = consume().value();
            while(peak().has_value() && std::isalnum(*peak().value())){
                consume();
            }
            tokens.push_back(Token{.type = TokenType::ALNUM, .begin = begin, .end = at()});
            continue;
        // reserved
        }else if(std::string("+-*/()").find(*peak().value()) != std::string::npos){
            tokens.push_back(Token{.type = TokenType::RESERVED, .begin = consume().value(), .end = peak().value()});
            continue;
        }else if(std::isdigit(*peak().value())){
            std::string::const_iterator begin = consume().value();
            while(peak().has_value() && std::isdigit(*peak().value())){
                consume();
            }
            tokens.push_back(Token{.type = TokenType::NUM, .begin = begin, .end = at() });
            continue;
        }
        std::cerr << "Invalid character." << std::endl;
        exit(EXIT_FAILURE);
    }
    return tokens;
}
