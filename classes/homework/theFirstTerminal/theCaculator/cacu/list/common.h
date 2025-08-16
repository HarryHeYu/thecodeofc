// common.h
#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>

typedef enum {ERROR = 0, OK = 1} Status;
typedef enum {FALSE = 0, TRUE = 1} Bool;

#define INITIAL_SIZE 100
#define PI 3.141592653589793

//DEBUG报错
#define DEBUG 1

//调试函数
void debugPrint(const char* format, ...);

#endif // COMMON_H

