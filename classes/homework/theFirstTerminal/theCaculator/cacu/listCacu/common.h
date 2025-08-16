#ifndef COMMON_H
#define COMMON_H

typedef enum {ERROR = 0, OK = 1} Status;
typedef enum {FALSE = 0, TRUE = 1} Bool;

// Debugging function prototype
void debugPrint(const char* format, ...);

#endif // COMMON_H
