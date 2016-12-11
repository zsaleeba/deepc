#ifndef UTIL_H
#define UTIL_H

bool ReadFile(char **content, char **errMsg, const char *fileName);
void AllocSprintf(char **strp, char *fmt, ...);
void FreeStr(char **str);

#endif // UTIL_H
