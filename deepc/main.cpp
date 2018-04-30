#include <iostream>
#include <getopt.h>
#include <cctype>

#include "compileargs.h"
#include "compiler.h"
#include "fail.h"


using namespace deepC;


//
// The main program.
//

int main(int argc, char *argv[])
{
    // The available command line parameters.
    static struct option longOpts[] =
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
    std::vector<std::string> includePath;
    std::vector<std::string> defines;
    std::vector<std::string> warnings;

    // Read the command line parameters.
    int flag = 0;
    do
    {
        flag = getopt_long(argc, argv, "O:co:gI:W:", longOpts, &longInd);
        if (flag >= 0)
        {
            switch (flag)
            {
            case 'O':
                if (!std::isdigit(optarg[0]))
                {
                    failf("invalid optimisation level");
                }

                args.setOptimisationLevel(std::atoi(optarg));
                break;

            case 'c':
                args.setPerformLink(false);
                break;

            case 'o':
                args.setOutputFileName(optarg);
                break;

            case 'g':
                args.setOutputDebugSymbols(true);
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

    // Set the accumulated list parameters.
    args.setIncludePath(includePath);
    args.setDefines(defines);
    args.setWarnings(warnings);

    // Get file args.
    if (optind == argc)
    {
        failf("no files provided");
    }

    // Compile each of the file arguments.
    Compiler comp(args);
    while (optind < argc)
    {
        comp.compile(argv[optind]);
        optind++;
    }

    return 0;
}
