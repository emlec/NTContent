/*
The MIT License (MIT)

Copyright (c) 2016 Aurelien Guy-Duche

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

#define DEBUG(FORMAT, ARGS...) \
    do { \
    fprintf(stderr, "[%s:%d]: ", __FILE__, __LINE__); \
    fprintf(stderr, FORMAT, ARGS); \
    fputc('\n', stderr); \
    } while(0)

#define ERROR(MESSAGE, ENDING) \
    do { \
    DEBUG("%s", MESSAGE); \
    ENDING; \
    } while(0)

void* safeMalloc(size_t size);
void* safeCalloc(size_t number, size_t size);
void* safeRealloc(void* ptr, size_t size);
char* safeStrdup(char* s);
char* safeStrAppend(char* x, const char* y);
gzFile safeGzOpen(char* filename, char* mode);
FILE* safeFOpen(char* filename, char* mode);

#endif // UTILS_H_INCLUDED
