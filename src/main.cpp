#include <iostream>
#include "cstdio"
#include "cstdlib"
#include "parse.hpp"
#include "editline/readline.h"

#ifdef _WIN32
#include <string.h>
static char buffer[2048];
char* readline(char* prompt) {
  fputs(prompt, stdout);
  fgets(buffer, 2048, stdin);
  char* cpy = malloc(strlen(buffer)+1);
  strcpy(cpy, buffer);
  cpy[strlen(cpy)-1] = '\0';
  return cpy;
}
void add_history(char* unused) {}
#else
#ifdef __MACH__
#include <editline/readline.h>
#else
#include <editline/history.h>
#endif
#endif

int main(int argc, char** argv) {
    std::printf("Welcome to ILisp\n\tPress C-c to Exit\n");

    while(true){
        char *_buf = readline("\033[1m\033[31m>\033[0m \033[32m");
        add_history(_buf);
        Stack<std::string> tokens= tokenize(_buf);
        tokens.reverse();
        std::printf("%.3f\n",List(tokens, new AST(nullptr,"_ret")).get<double>());
        free(_buf);
    }

    return 0;
}
