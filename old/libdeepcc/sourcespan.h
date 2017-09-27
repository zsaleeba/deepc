/**************************************************************************
 ***                                                                    ***
 ***                          DeepC compiler                            ***
 ***                                                                    ***
 ***                       Parse tree generator                         ***
 ***                                                                    ***
 ***                                         - Zik Saleeba 2017         ***
 ***                                                                    ***
 **************************************************************************/

#ifndef DEEPC_SOURCESPAN_H
#define DEEPC_SOURCESPAN_H

#include "sourcepos.h"


namespace deepC
{

//
// SourceSpan represents a range of text in a source file.
//

class SourceSpan
{
private:
    SourcePos from_;
    SourcePos to_;

public:
    // Constructors.
    SourceSpan(const SourcePos &from, const SourcePos &to) : from_(from), to_(to) {}
    SourceSpan(int fromLine, int fromColumn, int toLine, int toColumn) : from_(fromLine, fromColumn), to_(toLine, toColumn) {}

    // Accessors.
    const SourcePos &getFrom() const       { return from_; }
    const SourcePos &getTo() const         { return to_; }
    int              getFromLine() const   { return from_.getLine(); }
    int              getFromColumn() const { return from_.getColumn(); }
    int              getToLine() const     { return to_.getLine(); }
    int              getToColumn() const   { return to_.getColumn(); }

    // Source code alteration alters source positions.
    void add(const SourceSpan &at);
    void remove(const SourceSpan &at);
};

}  // namespace deepC

#endif // DEEPC_SOURCESPAN_H
