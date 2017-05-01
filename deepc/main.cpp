#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string>

#include "sourcemonitor.h"


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
    std::cout << "compiling " << srcFileName << std::endl;
    return true;
}


int main(int argc, char **argv)
{
    std::cout << "deepc v0.02\n";

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
