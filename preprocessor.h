#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include <string>
#include "compileargs.h"
#include "programdb.h"


//
//
//

class Preprocessor
{
    // Inputs for this preprocessing operation.
    std::string        sourceFileName_;
    const CompileArgs &args_;
    ProgramDb         &pdb_;

    // Results of preprocessing.
    std::string        preProcText_;    // The preprocessed source text.

public:
    Preprocessor(const std::string &sourceFileName, const CompileArgs &args, ProgramDb &pdb);
    const std::string &getPreprocessedText() { return preProcText_; }
};

#endif // PREPROCESSOR_H
