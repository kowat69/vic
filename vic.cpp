#include "vic.hpp"



int main(int argc, char* argv[]){
    if (argc < 2){
        std::cerr << "Incorrect usage. Correct usage is..." << std::endl;
        std::cerr << "vic <input.vi>" << std::endl;
        return EXIT_FAILURE;
    }
    std::chrono::system_clock::time_point  start, end, _lld; // 型は auto で可
    start = std::chrono::system_clock::now(); // 計測開始時間


    std::string contents;
    {
        std::stringstream contents_stream;
        std::fstream input(argv[1], std::ios::in);
        contents_stream << input.rdbuf();
        contents = contents_stream.str();
    }
    Error err(contents);
    Tokenizer tokenizer(contents);
    std::vector<Token> tokens = tokenizer.tokenize();
    
    Parser parser(tokens, err);
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
    std::cout << "ASM END-----------" << std::endl;
    

// 処理
    std::filesystem::path p = argv[1];
    std::filesystem::path out_asm = p.replace_extension(".asm ");
    std::filesystem::path out_obj = p.replace_extension(".obj ");
    std::filesystem::path out_exe = p.replace_extension(".exe ");
    {
        std::fstream file(out_asm.string(), std::ios::out);
        file << assembly.value();
    }
    end = std::chrono::system_clock::now();  // 計測終了時間
//
    Clopt clopt(argc, argv);
    
    clopt.NASM();
    
    std::chrono::system_clock::time_point _nasm = std::chrono::system_clock::now();

    clopt.LLD();
//    
    _lld = std::chrono::system_clock::now();
    std::cout << "compile: "<<std::chrono::duration_cast<std::chrono::milliseconds>(_lld-start).count() << "ms" << std::endl; //処理に要した時間をミリ秒に変換
    std::cout << "nasm: "<<std::chrono::duration_cast<std::chrono::milliseconds>(_nasm-end).count() << "ms" << std::endl; //処理に要した時間をミリ秒に変換
    std::cout << "link: "<<std::chrono::duration_cast<std::chrono::milliseconds>(_lld-_nasm).count() << "ms" << std::endl; //処理に要した時間をミリ秒に変換

    return EXIT_SUCCESS;
}
