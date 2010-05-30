#ifndef PREPROC_H
#define PREPROC_H

#include <string>
#include <fstream>

using namespace std;

//
// This class encapsulates the preprocessor. What's read from
// this class is preprocessed source code.
//

class PreProcessor
{
    // size of the read buffer
    static const int ReadBufSize = 16384;
    
    // input
    ifstream mInputStream;
    bool mIsInteractive;
    string mSourceFileName;
        
public:
    //
    // NAME:        PreProcessor
    // ACTION:      Constructor to initialise the preprocessor
    //    
    PreProcessor();
    
    //
    // NAME:        ~PreProcessor
    // ACTION:      Release any resources used by the preprocessor
    //     
    ~PreProcessor();    
    
    // accessors
    bool IsInteractive() { return mIsInteractive; };
    
    //
    // NAME:        Open
    // ACTION:      Ready the preprocessor to read and process a file
    // PARAMETERS:  string FileName - the file to start parsing, empty for console input
    //
    void Open(string FileName);
    
    //
    // NAME:        Close
    // ACTION:      Release any resources used by the preprocessor
    //
    void Close();

    //
    // NAME:        Read
    // ACTION:      Called by the lexer to get pre-processed input from the pre-processor
    // PARAMETERS:  char *ReadBuf - where to place the data which has been read
    //              int Length - maximum data length to read
    // RETURNS:     int - number of bytes read or 0 at EOF
    //
    int Read(char *ReadBuf, int Length);
};

#endif /* PREPROC_H */
