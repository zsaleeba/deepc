/**************************************************************************
 ***                                                                    ***
 ***                          DeepC compiler                            ***
 ***                                                                    ***
 ***                    Source positions and spans                      ***
 ***                                                                    ***
 ***                                   - Zik Saleeba 2017-03-16         ***
 ***                                                                    ***
 **************************************************************************/

#include "sourcespan.h"


namespace deepC
{
    
//
// Having added a new span of characters to the source, adjust our
// span if necessary.
//

void SourceSpan::add(const SourceSpan &at)
{
    from_.add(at);
    to_.add(at);
}


//
// Having removed a span of characters in the source, adjust our
// span if necessary.
//

void SourceSpan::remove(const SourceSpan &at)
{
    from_.remove(at);
    to_.remove(at);
}

}  // namespace deepC
