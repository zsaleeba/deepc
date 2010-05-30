#ifndef DEEPLIB_H
#define DEEPLIB_H

class Parser;

/* lib.c */
void ProgramFail(Parser &p, const char *Format, ...);
void ProgramErrorMessage(Parser &p, const char *Format, ...);
//void *CheckedAlloc(int Size);

#endif /* DEEPLIB_H */
