#pragma once
#include "stdafx.hpp"
#include "tokenization.hpp"
#include "oblate.hpp"

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
    inline Parser(const std::vector<Token>& tokens) : m_tokens(tokens){}
    my::oblate<NodeProg> parse();
private:
    my::oblate<NodeStmt> parse_stmt();
    my::oblate<Node> parse_expr();
    my::oblate<Node> parse_mul();
    my::oblate<Node> parse_primary();

    inline std::optional<Token> expect(std::string str){
        if(peak().has_value() && peak().value().str() == str ){
            return consume();
        }else{
            std::cerr << "Expect: \'" << str << "\'" << std::endl;
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

    std::size_t m_index = 0;
    std::vector<Token> m_tokens;
};
