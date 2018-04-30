#ifndef DEEPC_SOURCEPOS_H
#define DEEPC_SOURCEPOS_H

#include <string>


namespace deepC
{


class SourcePos
{
private:
    std::string fileName_;
    int         line_;
    int         column_;

public:
    explicit SourcePos() : line_(0), column_(0) {}
    explicit SourcePos(const std::string &fileName, int line, int column) : fileName_(fileName), line_(line), column_(column) {}

    const std::string &fileName() const { return fileName_; }
    int                line() const     { return line_; }
    int                column() const   { return column_; }
    bool               exists() const   { return !fileName_.empty(); }
};


} // namespace deepC

#endif // DEEPC_SOURCEPOS_H
