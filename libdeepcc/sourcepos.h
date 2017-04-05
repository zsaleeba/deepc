/**************************************************************************
 ***                                                                    ***
 ***                          DeepC compiler                            ***
 ***                                                                    ***
 ***                       Parse tree generator                         ***
 ***                                                                    ***
 ***                                         - Zik Saleeba 2017         ***
 ***                                                                    ***
 **************************************************************************/

#ifndef DEEPC_SOURCEPOS_H
#define DEEPC_SOURCEPOS_H

namespace DeepC
{

    // Forward declarations.
    class SourceSpan;
    
    
    //
    // SourcePos represents a location in a source file.
    //
    
    class SourcePos
    {
    private:
        int line_;
        int column_;
    
    public:
        // Constructor.
        SourcePos(int line, int column) : line_(line), column_(column) {}
    
        // Accessors.
        int getLine() const   { return line_; }
        int getColumn() const { return column_; }
    
        // Source code alteration alters source positions.
        void add(const SourceSpan &at);
        void remove(const SourceSpan &at);
    };

}

#endif // DEEPC_SOURCEPOS_H
