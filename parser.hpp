#pragma once
#include "stdafx.hpp"
#include "tokenization.hpp"
#include "oblate.hpp"
#include "error.hpp"

enum class NodeType{
    ADD,
    SUB,
    MUL,
    DIV,
    NUM,
};
struct Node{
public:
    NodeType type;
    my::oblate<Node> lhs, rhs;
    Node(){}
    Node(NodeType type, const Token& token, my::oblate<Node> lhs, my::oblate<Node> rhs):type(type), lhs(lhs), rhs(rhs), begin(token.begin), end(token.end){}
    inline void output(){
        std::cout << NAMEOF_ENUM(type) << " :\'"<< std::string(begin, end) << "\'"<< std::endl;
        if(lhs.has_value()) lhs.value().output();
        if(rhs.has_value()) rhs.value().output();
    }
    std::string::const_iterator begin;
    std::string::const_iterator end;

};
struct NodeInteger32 : public Node{
    static std::string* _integer32_0; // = new std::string("0");
    static my::oblate<Node> NodeInteger32_0(){
        return Node(NodeType::NUM, Token{.type = TokenType::NUM, .begin = _integer32_0->begin(), .end = _integer32_0->end()},
                    my::oblate<Node>::nullopt(), my::oblate<Node>::nullopt());
    }
    NodeInteger32(const Token& token) :Node(NodeType::NUM, token, my::oblate<Node>::nullopt(), my::oblate<Node>::nullopt()) {}
};
struct NodeStmt{
    my::oblate<Node> var;
};
struct NodeProg{
    std::vector<NodeStmt> stmts;
};

class Parser{
public:
    inline Parser(const std::vector<Token>& tokens, const Error& err) : m_tokens(tokens), m_err(err){}
    my::oblate<NodeProg> parse();
private:
    my::oblate<NodeStmt> parse_stmt();
    my::oblate<Node> parse_expr();
    my::oblate<Node> parse_mul();
    my::oblate<Node> parse_unary();
    my::oblate<Node> parse_primary();

    inline std::optional<Token> expect(std::string str){
        if(peak().has_value() && peak().value().str() == str ){
            return consume();
        }else{
            std::string::const_iterator e = peak().has_value()? peak().value().begin:m_tokens[m_tokens.size() - 1].end + 1;
            m_err.pthrow(e,"Expect: \'%s\'", str.c_str());
            exit(EXIT_FAILURE);
        }
    }
    inline std::optional<Token> consume(){
        return m_tokens[m_index++];
    }
    inline std::optional<Token> peak(int ahead = 1){
        if(m_index + ahead > m_tokens.size()){
            return std::nullopt;
        }else{
            return m_tokens[m_index];
        }
    }

    const Error& m_err;
    std::size_t m_index = 0;
    std::vector<Token> m_tokens;
};
