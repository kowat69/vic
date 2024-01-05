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
    my::oblate<Node> node = parse_primary();
    while(peak().has_value() && peak().value().type == TokenType::RESERVED){
        if(peak().value().str() == "*") {
            Token token = consume().value();
            node = Node(NodeType::MUL, token, node, parse_primary());
        }else if(peak().value().str() == "/") {
            Token token = consume().value();
            node = Node(NodeType::DIV, token, node, parse_primary());
        }else {
            return node;
        }
    }
    return node;
}
my::oblate<Node> Parser::parse_primary(){
    if(peak().has_value()){
        const Token token = consume().value();
        if(token.type == TokenType::RESERVED && token.str() == "("){
            my::oblate<Node> node = parse_expr();
            expect(")");
            return node;
        }else if(token.type == TokenType::NUM){
            my::oblate<Node> node = NodeInteger32(token);
            return node;
        }else{
            //error
        }
    }
    std::cerr << "Invalid expression." << std::endl;
    exit(EXIT_FAILURE);
}

