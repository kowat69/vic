#include "generation.hpp"


my::oblate<std::string> Generator::generate(){
    m_output <<
        "bits 64\n"
        "default rel\n"
        "global main\n"
        "section .text\n"
        "main:\n"
        "push rbp\n"
        "mov rbp, rsp\n"
    ;
    if(m_program.has_value()){
        for(NodeStmt stmt : m_program.value().stmts){
            gen(stmt.var.value());
        }
    }
    m_output <<
        "mov rsp, rbp\n"
        "pop rbp\n"
        "ret\n"
        ;
    return m_output.str();
}
void Generator::gen(my::oblate<Node> node){
    if(!node.has_value()) return;
    if(node.value().type == NodeType::NUM){
        m_output << "push "<< std::string(node.value().begin, node.value().end) << "\n";
        return;
    }
    gen(node.value().lhs.value());
    gen(node.value().rhs.value());


    m_output <<
        "pop rcx\n"
        "pop rax\n"
        ;
    switch(node.value().type){
        case NodeType::ADD:
            m_output << "add rax, rcx\n";
        break;
        case NodeType::SUB:
            m_output << "sub rax, rcx\n";
        break;
        case NodeType::MUL:
            m_output << "imul rax, rcx\n";
        break;
        case NodeType::DIV:
            m_output << "cqo\n";
            m_output << "idiv rcx\n";
        default:
        break;
    }
    m_output << "push rax\n";
}
