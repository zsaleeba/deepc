#ifndef UTIL_H
#define UTIL_H

bool ReadFile(char **content, char **errMsg, const char *fileName);
bool ReadLine(char **line, char **nextLine);
void AllocSprintf(char **strp, char *fmt, ...);
void FreeStr(char **str);
void StringStripEnd(char *str);
void SkipWhitespace(char **pos);

#endif // UTIL_H
