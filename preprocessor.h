#ifndef DEEPC_PREPROCESSOR_H
#define DEEPC_PREPROCESSOR_H

#include <string>
#include <memory>

#include "compileargs.h"
#include "programdb.h"


namespace deepC
{


//
// The preprocessor applies #includes and #defines.
//

//
// Preprocessor output is:
//  * macros it's dependent on.
//  * files it includes.
//  * macros it defines.
//  * preprocessed source output.
//

class Preprocessor
{
    // Inputs for this preprocessing operation.
    std::string                sourceFileName_;
    const CompileArgs         &args_;
    std::shared_ptr<ProgramDb> pdb_;

    // Results of preprocessing.
    std::string                preProcText_;    // The preprocessed source text.

public:
    Preprocessor(std::shared_ptr<ProgramDb> pdb, const CompileArgs &args, const std::string &sourceFileName);
    const std::string &getPreprocessedText() { return preProcText_; }
};


} // namespace deepC

#endif // DEEPC_PREPROCESSOR_H
