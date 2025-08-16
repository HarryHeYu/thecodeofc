#ifndef COMMON_H
#define COMMON_H

typedef enum {
    ERR_NONE,
    ERR_STACK_OVERFLOW,
    ERR_STACK_UNDERFLOW,
    ERR_MEMORY_ALLOCATION,
    ERR_INVALID_CHARACTER,
    ERR_INVALID_OPERATOR,
    ERR_DIVISION_BY_ZERO,
    ERR_INVALID_EXPRESSION,
    ERR_INVALID_FUNCTION
} ErrorCode;

typedef enum {ERROR = 0, OK = 1} Status;
typedef enum {FALSE = 0, TRUE = 1} Bool;

// Debugging function prototype
void debugPrint(const char* format, ...);

#endif // COMMON_H
