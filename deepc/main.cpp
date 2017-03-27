#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string>

#include "CLexer.h"
#include "CParser.h"
#include "antlr4-runtime.h"
#include "cparserlistener.h"


static struct option longOptions[] =
{
    {"output-file", required_argument, 0,  0 }
};


//
// NAME:        compile
// ACTION:
//

bool compile(std::string srcFileName)
{
    antlr4::ANTLRFileStream fs(srcFileName);

    CLexer lexer(&fs);
    antlr4::CommonTokenStream tokens(&lexer);
    CParser parser(&tokens);
    CParserListener parserListener;
    antlr4::ConsoleErrorListener errorListener;
    parser.addParseListener(&parserListener);
    parser.addErrorListener(&errorListener);
    antlr4::tree::ParseTree *parseTree = parser.compilationUnit();
    std::cout << parseTree->toStringTree(&parser) << std::endl;

    return true;
}


int main(int argc, char **argv)
{
    std::cout << "deepc v0.01\n";

    // Gather arguments.
    int optCh = 0;
    std::string outFileName;

    while (optCh != -1)
    {
        int optionIndex = 0;
        optCh = getopt_long(argc, argv, "o:", longOptions, &optionIndex);
        switch (optCh)
        {
        case 'o':
            outFileName = optarg;
            break;
        }
    }

    // Compile source files.
    while (optind < argc)
    {
        if (!compile(argv[optind]))
            return EXIT_FAILURE;

        optind++;
    }

    return EXIT_SUCCESS;
}
