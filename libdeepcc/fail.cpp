#include <cstdarg>
#include <cstdio>
#include <iostream>

#include "fail.h"
#include "sourcepos.h"


namespace deepC
{


// The maximum length of an error output line.
static const int maxErrorLine = 4096;


//
// Generic function to show an error message.
//

static void msgfv(const SourcePos &pos, const char *format, va_list args)
{
    if (pos.exists())
    {
        std::cout << "::: ";
    }
    else
    {
        std::cout << pos.fileName() << ":" << pos.line() << ":" << pos.column() << ": ";
    }

    char line[maxErrorLine];

    vsnprintf(line, maxErrorLine, format, args);
    std::cout << line << std::endl;
}


//
// Report failure with no specific location.
//

void failf(const char *format, ...)
{
    SourcePos noPos;
    va_list args;
    va_start(args, format);
    msgfv(noPos, format, args);
    std::exit(EXIT_FAILURE);
}


//
// Report failure with a source file location.
//

void failf(const SourcePos &pos, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    msgfv(pos, format, args);
    std::exit(EXIT_FAILURE);
}


} // namespace deepC
