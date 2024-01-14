#pragma once

#include "stdafx.hpp"

class Clopt{
public:
    Clopt(int argc, char* argv[]){
        if(argc < 1){
            std::cerr << "Compiler Option Error.";
            exit(EXIT_FAILURE);
        }
        for(int i = 1; i < argc; i++){
            std::string arg = std::string(argv[i]);
            if(arg[0] == '-'){
                if(std::optional<std::string> opt = opEqual(arg.begin() + 1, {"out"})){
                    std::string::iterator f = std::next(arg.begin(), 1 + opt.value().size());
                    if(opt.value() == "out"){
                        for(; f < arg.end(); f++){
                            if(!std::isspace(*f)) break;
                        }
                        if(f == arg.end()){
                            if(i + 1 >= argc) {
                                std::cerr << "Compiler Option Error.";
                                exit(EXIT_FAILURE);
                            }
                            outexe = std::string(argv[++i]);
                        }else{
                            outexe = std::string(f, arg.end() );
                        }
                    }
                }else{
                    std::cerr << "Compiler Option Error.";
                    exit(EXIT_FAILURE);
                }
                continue;
            }
            std::filesystem::path p = arg;
            infile.push_back(p.string());
            outasm.push_back(p.replace_extension(".asm").string());
            outobj.push_back(p.parent_path().string() + std::string("./.tmp/") + p.filename().replace_extension(".obj").string());
        }
        if(outexe.size() == 0){
            outexe = std::filesystem::path(infile[0]).replace_extension(".exe").string();
        }
        for(std::string str : infile){
            std::cout << str << std::endl;
        }
        for(std::string str : outasm){
            std::cout << str << std::endl;
        }
        for(std::string str : outobj){
            std::cout << str << std::endl;
        }
        std::cout << outexe << std::endl;

    }
    inline void NASM(){
        for(std::string nasm : getNASM()){
            system(nasm.c_str());
        }
    }
    inline void LLD(){
        system(getLLD().c_str());
    }
private:
    inline std::vector<std::string> getNASM(){
        std::vector<std::string> ret;
        std::stringstream output;
        for(int i = 0; i < outasm.size(); i++ ){
            output << nasm << ' ';
            output << outasm[i] << ' ';
            output << nasm_opt << " -o " << outobj[i];
            ret.push_back(output.str());
            output.clear();
        }
        return ret;
    }
    inline std::string getLLD(){
        std::stringstream output;
        output << lld << ' ';
        for(std::string obj : outobj){
            output << obj << ' ';
        }
        output << "/OUT:" << outexe << ' ';
        output << lld_opt;
        return output.str();
    }
    inline std::optional<std::string> opEqual(const std::string::const_iterator first, const std::vector<std::string> strs){
        for(std::string str : strs){
            for(int i = 0; i < str.size(); i++){
                if(*(first + i) != *(str.begin() + i)) return std::nullopt;
            }
            return str;
        }
        return std::nullopt;
    }
    std::vector<std::string> infile;
    std::vector<std::string> outasm;
    std::vector<std::string> outobj;
    std::string lld = "lld-link";
    std::string lld_opt = "/ENTRY:main /SUBSYSTEM:CONSOLE /MACHINE:X64";
    std::string nasm = "nasm";
    std::string nasm_opt = "-fwin64";
    std::string outexe = "";

};
