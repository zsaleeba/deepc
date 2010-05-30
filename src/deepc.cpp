#include <iostream>
#include <stdlib.h>

#include "compiler.h"

using namespace std;


int main(int argc, char **argv)
{
    cout << "deepc v0.1\n";
    
    if (argc != 2)
    {
        cout << "Format: deepc <file.dc>\n";
        exit(1);
    }
    
    // work out the source and destination file name
    string SourceFileName = argv[1];
    string DestFileName = SourceFileName;
    if (DestFileName.rfind(".dc") == DestFileName.length()-3)
        DestFileName.erase(DestFileName.length()-3);
    
    // compile it
    bool Success = Compiler::instance().Compile(SourceFileName, DestFileName, CSExe);
        
    if (!Success)
        exit(1);
    
    return 0;
}
