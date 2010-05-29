/* preproc.cpp */
#include "preproc.h"
#include "deeplib.h"


/* size of the read buffer */
const int PreProcessor::ReadBufSize;


//
// NAME:        PreProcessor::PreProcessor
// ACTION:      Constructor to initialise the preprocessor
//
 
PreProcessor::PreProcessor() :
    mInputStream(NULL), 
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
        mInputStream = NULL;
    }
    else
    {
        /* start reading a file */
        mInputStream = fopen(FileName.c_str(), "r");
        if (mInputStream == NULL)
            ProgramFail(Parser, "can't open file '%S'", FileName);
    
        mSourceFileName = FileName;
    }
}


//
// NAME:        Close
// ACTION:      Release any resources used by the preprocessor
//
 
void PreProcessor::Close()
{
    if (!mIsInteractive && mInputStream != NULL)
    {
        fclose(mInputStream);
        mInputStream = NULL;
    }
}


//
// NAME:        PreProcessor::Read
// ACTION:      Called by the lexer to get pre-processed input from the pre-processor
// RETURNS:     string - the data read or empty string at EOF
//
 
string PreProcessor::Read()
{
    char ReadBuf[ReadBufSize+1];
    
    if (mIsInteractive)
    {
        /* read a line from the console */
        ReadBuf[ReadBufSize] = '\0';
        if (fgets(ReadBuf, ReadBufSize, stdin) == NULL)
            return "";
        else
            return string(ReadBuf);
    }
    else
    {
        /* read a block of data from the input file */
        int BytesRead = fread(ReadBuf, 1, MaxSize, mInputStream);
        if (BytesRead < 0)
            return "";
        else
            return string(ReadBuf, BytesRead);
    }
}
