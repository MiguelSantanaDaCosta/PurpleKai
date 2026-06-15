#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include "../include/lexer.h"
#include "../include/parser.h"
#include "../include/interpreter.h"

// Instância global do interpretador para manter o estado entre as execuções do REPL
Interpreter interpreter;
bool hadError = false;
bool hadRuntimeError = false;

// Função principal que executa o código
void run(const std::string& source) {
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.scanTokens();

    Parser parser(tokens);
    auto program = parser.parse();

    if (hadError) return;

    // Converte unique_ptr<Declaration> para Stmt* (ignorando declarações não-Stmt, como classes)
    std::vector<Stmt*> stmts;
    for (auto& decl : program->declarations) {
        Stmt* stmt = dynamic_cast<Stmt*>(decl.get());
        if (stmt) stmts.push_back(stmt);
    }

    interpreter.interpret(stmts);
}

// Executa um arquivo .pk
void runFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Não foi possível abrir o arquivo: " << path << "\n";
        exit(74);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    run(buffer.str());

    if (hadError) exit(65);
    if (hadRuntimeError) exit(70);
}

// Executa o modo interativo (REPL)
void runPrompt() {
    std::string line;
    std::cout << "====================================\n";
    std::cout << " PurpleKai REPL - v0.1\n";
    std::cout << " Digite 'sair' para encerrar.\n";
    std::cout << "====================================\n";

    for (;;) {
        std::cout << "💜 > ";
        if (!std::getline(std::cin, line)) break;
        if (line == "sair" || line == "quit") break;
        
        run(line);
        hadError = false; // Reseta o erro para não travar o loop
    }
}

int main(int argc, char* argv[]) {
    if (argc > 2) {
        std::cout << "Uso: purplekai [script.pk]\n";
        return 64;
    } else if (argc == 2) {
        runFile(argv[1]);
    } else {
        runPrompt();
    }

    return 0;
}
