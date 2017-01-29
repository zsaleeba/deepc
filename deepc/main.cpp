#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "CLexer.h"
#include "CParser.h"
#include "antlr4-runtime.h"
//#include "ANTLRFileStream.h"
//#include "ConsoleErrorListener.h"
#include "cparserlistener.h"


int main(int, char **)
{
    printf("deepc v0.01\n");

    antlr4::ANTLRFileStream fs("example.c");

    CLexer lexer(&fs);
    antlr4::CommonTokenStream tokens(&lexer);
    CParser parser(&tokens);
    CParserListener parserListener;
    antlr4::ConsoleErrorListener errorListener;
    parser.addParseListener(&parserListener);
    parser.addErrorListener(&errorListener);
    antlr4::tree::ParseTree *parseTree = parser.compilationUnit();
    std::cout << parseTree->toStringTree(&parser) << std::endl;

    return 0;
}
