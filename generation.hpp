#pragma once

#include "parser.hpp"

class Generator{
public:
    Generator(const my::oblate<NodeProg>& program) : m_program(program){}
    my::oblate<std::string> generate();
private:
    void gen(my::oblate<Node> node);
    std::stringstream m_output;
    const my::oblate<NodeProg>& m_program;
};
