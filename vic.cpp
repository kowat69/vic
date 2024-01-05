#include "vic.hpp"

#include <chrono>


std::string token_to_asm(std::vector<Token> tokens){
    std::stringstream output;
    output <<
        "bits 64\n"
        "default rel\n"
        "global main\n"
        "section .text\n"
        "main:";
    return output.str();
}


int main(int argc, char* argv[]){
    if (argc != 2){
        std::cerr << "Incorrect usage. Correct usage is..." << std::endl;
        std::cerr << "vic <input.vi>" << std::endl;
        return EXIT_FAILURE;
    }
    std::chrono::system_clock::time_point  start, end, end2; // 型は auto で可
    start = std::chrono::system_clock::now(); // 計測開始時間


    std::string contents;
    {
        std::stringstream contents_stream;
        std::fstream input(argv[1], std::ios::in);
        contents_stream << input.rdbuf();
        contents = contents_stream.str();
    }
    Tokenizer tokenizer(contents);
    std::vector<Token> tokens = tokenizer.tokenize();
    
    Parser parser(tokens);
    my::oblate<NodeProg> program = parser.parse();

    Generator generator(program);
    my::oblate<std::string> assembly = generator.generate();

    std::cout << "Tokens----------" << std::endl;
    for (Token t : tokens){
        std::cout << NAMEOF_ENUM(t.type) << std::endl;
        std::cout << std::string(t.begin, t.end) << std::endl;
    }
    std::cout << "Node----------" << std::endl;
    for(NodeStmt stmt : program.value().stmts){
        stmt.var.value().output();
    }
    std::cout << "ASM-----------" << std::endl;
    std::cout << assembly.value();


// 処理
    {
        std::fstream file("bin/out.asm", std::ios::out);
        file << assembly.value();
    }
    end = std::chrono::system_clock::now();  // 計測終了時間
//
    system("nasm -f win64 bin/out.asm");
    std::chrono::system_clock::time_point _nasm = std::chrono::system_clock::now();
    system("lld-link bin/out.obj /OUT:bin/out.exe /ENTRY:main /SUBSYSTEM:CONSOLE /MACHINE:X64");
//    
    end2 = std::chrono::system_clock::now();
    std::cout << "compile: "<<std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count() << "ms" << std::endl; //処理に要した時間をミリ秒に変換
    std::cout << "link: "<<std::chrono::duration_cast<std::chrono::milliseconds>(end2-end).count() << "ms" << std::endl; //処理に要した時間をミリ秒に変換
    std::cout << "nasm: "<<std::chrono::duration_cast<std::chrono::milliseconds>(_nasm-end).count() << "ms" << std::endl; //処理に要した時間をミリ秒に変換

    return EXIT_SUCCESS;
}
