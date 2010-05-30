/* preproc.cpp */
#include "preproc.h"

#include <iostream>
#include <fstream>
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
// NAME:        PreProcessor::Read
// ACTION:      Called by the lexer to get pre-processed input from the pre-processor
// RETURNS:     string - the data read or empty string at EOF
//
 
string PreProcessor::Read()
{
    string Result;
    
    if (mIsInteractive)
    {
        /* read a line from the console */
        if (!cin.eof())
            getline(cin, Result);
    }
    else
    {
        /* read a large block of data from the input file */
        if (!mInputStream.eof())
        {
            char ReadBuf[ReadBufSize];
            
            mInputStream.read(&ReadBuf[0], ReadBufSize);
            Result.assign(ReadBuf, mInputStream.gcount());
        }
    }

    return Result;
}
