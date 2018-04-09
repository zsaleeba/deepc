#ifndef DEEPC_COMPILEARGS_H
#define DEEPC_COMPILEARGS_H

#include <string>
#include <vector>


namespace deepC
{


//
// Arguments for a compile session.
//

class CompileArgs
{
private:
    int  optimisationLevel_;
    bool performLink_;
    std::string outputFileName_;
    bool outputDebugSymbols_;
    std::vector<std::string> includePath_;
    std::vector<std::string> defines_;
    std::vector<std::string> warnings_;
    std::string programDbFileName_;
    std::string target_;

private:
    // Do substitutions on a single string.
    void substituteStr(std::string *str);

public:
    CompileArgs();

    // Perform variable substitutions on all the arguments.
    void substituteVariables();

    // Accessors.
    int  getOptimisationLevel() const                   { return optimisationLevel_; }
    void setOptimisationLevel(int optimisationLevel)    { optimisationLevel_ = optimisationLevel; }
    bool getPerformLink() const                         { return performLink_; }
    void setPerformLink(bool performLink)               { performLink_ = performLink; }
    std::string getOutputFileName() const               { return outputFileName_; }
    void setOutputFileName(const std::string &outputFileName) { outputFileName_ = outputFileName; }
    bool getOutputDebugSymbols() const                  { return outputDebugSymbols_; }
    void setOutputDebugSymbols(bool outputDebugSymbols) { outputDebugSymbols_ = outputDebugSymbols; }
    std::vector<std::string> getIncludePath() const     { return includePath_; }
    void setIncludePath(const std::vector<std::string> &includePath) { includePath_ = includePath; }
    void addIncludePath(const std::string &includePath) { includePath_.push_back(includePath); }
    std::vector<std::string> getDefines() const         { return defines_; }
    void setDefines(const std::vector<std::string> &defines) { defines_ = defines; }
    void addDefines(const std::string &define)          { defines_.push_back(define); }
    std::vector<std::string> getWarnings() const        { return warnings_; }
    void setWarnings(const std::vector<std::string> &warnings) { warnings_ = warnings; }
    void addWarning(const std::string &warning)         { warnings_.push_back(warning); }
    std::string getProgramDbFileName() const               { return programDbFileName_; }
    void setProgramDbFileName(const std::string &programDbFileName) { programDbFileName_ = programDbFileName; }
    std::string target() const   { return target_; }
    void setTarget(const std::string &target) { target_ = target; }
};


} // namespace deepC

#endif // DEEPC_COMPILEARGS_H
