#ifndef DEEPC_FAIL_H
#define DEEPC_FAIL_H

#include <stdarg.h>


namespace deepC
{


// Forward declarations.
class SourcePos;


//
// printf-style error message formatting.
//

void failf(const char *format, ...);
void failf(const SourcePos &pos, const char *format, ...);


} // namespace deepC

#endif // DEEPC_FAIL_H
