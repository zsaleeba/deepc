#ifndef DEEPC_CLEXER_H
#define DEEPC_CLEXER_H

#include <memory>
#include <string>


namespace deepC
{


// Forward declarations.
class ProgramDb;


//
// The lexer converts source text into tokens.
//

class CLexer
{
    std::shared_ptr<ProgramDb> pdb_;
    std::string                sourceFileName_;

public:
    CLexer(std::shared_ptr<ProgramDb> pdb, const std::string &sourceFileName);
};


} // namespace deepC

#endif // DEEPC_CLEXER_H
