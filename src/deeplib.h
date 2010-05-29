#ifndef DEEPLIB_H
#define DEEPLIB_H

#include "parser.h"

/* lib.c */
void ProgramFail(Parser &p, const char *Format, ...);
void ProgramErrorMessage(Parser &p, const char *Format, ...);
//void *CheckedAlloc(int Size);

#endif /* DEEPLIB_H */
