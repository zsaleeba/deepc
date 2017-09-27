#include <iostream>
#include <getopt.h>

#include "compileargs.h"
#include "compiler.h"


//
// The main program.
//

int main(int argc, char *argv[])
{
    static struct option long_options[] =
    {
        {"optimize",      required_argument, nullptr,  'O' },
        {"compile-only",  no_argument,       nullptr,  'c' },
        {"output-file",   required_argument, nullptr,  'o' },
        {"debug-symbols", no_argument,       nullptr,  'g' },
        {"include",       required_argument, nullptr,  'I' },
        {"define",        required_argument, nullptr,  'D' },
        {"warning",       required_argument, nullptr,  'W' },
        {0,               0,                 0,        0   }
    };

    int  longInd = 0;
    CompileArgs args;
    int flag;

    do
    {
        flag = getopt_long(argc, argv, "O:co:gI:W:", longOpts, &longInd);
        if (flag >= 0)
        {
            switch (flag)
            {
            case 'O':
                args.optimisationLevel_ = checkedAtoi(optarg);
                break;

            case 'c':
                performLink = false;
                break;

            case 'o':
                outputFile = optarg;
                break;

            case 'g':
                outputDebugSymbols = true;
                break;

            case 'I':
                includePath.push_back(optarg);
                break;

            case 'D':
                defines.push_back(optarg);
                break;

            case 'W':
                warnings.push_back(optarg);
                break;
            }
        }
    } while (flag >= 0);

    // Get file args.
    if (optind == argc)
    {
        failf("no files provided");
    }

    // Compile each of the file arguments.
    Compiler comp;
    while (optind < argc)
    {
        comp.compile(args, argv[optind]);
        optind++;
    }

    return 0;
}
