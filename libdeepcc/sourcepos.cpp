/**************************************************************************
 ***                                                                    ***
 ***                          DeepC compiler                            ***
 ***                                                                    ***
 ***                    Source positions and spans                      ***
 ***                                                                    ***
 ***                                   - Zik Saleeba 2017-03-16         ***
 ***                                                                    ***
 **************************************************************************/

#include "sourcepos.h"
#include "sourcespan.h"


namespace DeepC
{

    //
    // Having added a new span of characters to the source, adjust our
    // pos if necessary.
    //
    
    void SourcePos::add(const SourceSpan &at)
    {
        if ( at.getFrom().getLine() > line_ ||
             ( at.getFrom().getLine() == line_ && at.getFrom().getColumn() > column_ ) )
        {
            // This pos is before the change.
            return;
        }
    
        line_ += at.getTo().line_ - at.getFrom().line_;
        column_ += at.getTo().column_ - at.getFrom().column_;
    }
    
    
    //
    // Having removed a span of characters in the source, adjust our
    // pos if necessary.
    //
    
    void SourcePos::remove(const SourceSpan &at)
    {
        if ( at.getFrom().getLine() > line_ ||
             ( at.getFrom().getLine() == line_ && at.getFrom().getColumn() > column_ ) )
        {
            // This pos is before the change.
            return;
        }
    
        line_ -= at.getTo().line_ - at.getFrom().line_;
        column_ -= at.getTo().column_ - at.getFrom().column_;
    }
    
    
}
