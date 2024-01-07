#include "parser.hpp"

my::oblate<NodeProg> Parser::parse(){
    my::oblate<NodeProg> program;
    program.alloc();
    while(peak().has_value()){
        program.value().stmts.push_back( parse_stmt().value());
    }
    m_index = 0;
    return program;
}


my::oblate<NodeStmt> Parser::parse_stmt(){
    return NodeStmt{.var = parse_expr()};
}

my::oblate<Node> Parser::parse_expr(){
    my::oblate<Node> node = parse_mul();
    while(peak().has_value() && peak().value().type == TokenType::RESERVED){
        if(peak().value().str() == "+"){
            Token token = consume().value();
            node = Node(NodeType::ADD, token, node, parse_mul());
        }else if(peak().value().str() == "-"){
            Token token = consume().value();
            node = Node(NodeType::SUB, token, node, parse_mul());
        }else{
            return node;
        }
    }
    return node;
}
my::oblate<Node> Parser::parse_mul(){
    my::oblate<Node> node = parse_unary();
    while(peak().has_value() && peak().value().type == TokenType::RESERVED){
        if(peak().value().str() == "*") {
            Token token = consume().value();
            node = Node(NodeType::MUL, token, node, parse_unary());
        }else if(peak().value().str() == "/") {
            Token token = consume().value();
            node = Node(NodeType::DIV, token, node, parse_unary());
        }else {
            return node;
        }
    }
    return node;
}
my::oblate<Node> Parser::parse_unary(){
    if(peak().has_value()){
        std::string str = peak().value().str();
        if(str == "+"){
            consume();
            return parse_primary();
        }else if(str == "-"){
            Token t = consume().value();
            return Node(NodeType::SUB, t, NodeInteger32::NodeInteger32_0(), parse_primary());
        }
    }
    return parse_primary();
}
my::oblate<Node> Parser::parse_primary(){
    if(peak().has_value()){
        const Token token = consume().value();
        if(token.type == TokenType::RESERVED && token.str() == "("){
            my::oblate<Node> node = parse_expr();
            expect(")");
            return node;
        }else if(token.type == TokenType::NUM){
            return NodeInteger32(token);
        }else{
            m_err.pthrow(token.begin, "Invalid expression.");
            exit(EXIT_FAILURE);
        }
    }
    //トークンの最後に到達したのに、何もない時起こる
    m_err.pthrow(m_tokens[m_tokens.size() - 1].end - 1, "Invalid expression.");// point m_str.end()
    exit(EXIT_FAILURE);
}
std::string* NodeInteger32::_integer32_0 = new std::string("0");

