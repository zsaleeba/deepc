/* preproc.cpp */
#include "preproc.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdlib.h>


/* size of the read buffer */
const int PreProcessor::ReadBufSize;


//
// NAME:        PreProcessor::PreProcessor
// ACTION:      Constructor to initialise the preprocessor
//
 
PreProcessor::PreProcessor() :
    mIsInteractive(false)
{
}


//
// NAME:        PreProcessor::~PreProcessor
// ACTION:      Release any resources used by the preprocessor
//
 
PreProcessor::~PreProcessor()
{
    Close();
}


//
// NAME:        Open
// ACTION:      Ready the preprocessor to read and process a file
// PARAMETERS:  string FileName - the file to start parsing, empty for console input
//
 
void PreProcessor::Open(string FileName)
{
    if (FileName.empty())
    {
        /* start reading from the console */
        mIsInteractive = true;
    }
    else
    {
        /* start reading a file */
        mInputStream.open(FileName.c_str());
        if (!mInputStream.is_open())
        {
            cout << "can't open file '" << FileName << "'\n";
            exit(1);
        }
    
        mSourceFileName = FileName;
    }
}


//
// NAME:        Close
// ACTION:      Release any resources used by the preprocessor
//
 
void PreProcessor::Close()
{
    if (!mIsInteractive && mInputStream.is_open())
        mInputStream.close();
}


//
// NAME:        Read
// ACTION:      Called by the lexer to get pre-processed input from the pre-processor
// PARAMETERS:  char *ReadBuf - where to place the data which has been read
//              int Length - maximum data length to read
// RETURNS:     int - number of bytes read or 0 at EOF
//

int PreProcessor::Read(char *ReadBuf, int Length)
{
    if (mIsInteractive)
    {
        /* read a line from the console */
        if (cin.eof())
            return 0;
        else
        {
            string Result;
            
            getline(cin, Result);
            Result.copy(ReadBuf, Length);
            
            return min((int)Result.length(), Length);
        }
    }
    else
    {
        /* read a large block of data from the input file */
        if (mInputStream.eof())
            return 0;
        else
        {
            mInputStream.read(&ReadBuf[0], ReadBufSize);
            return (int)mInputStream.gcount();
        }
    }
}
