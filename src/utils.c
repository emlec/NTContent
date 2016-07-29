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

#include "utils.h"

void* safeMalloc(size_t size)
{
    void* p = malloc(size);
    if (p == NULL)
        ERROR("Failed memory allocation", exit(1));
    return p;
}

void* safeCalloc(size_t number, size_t size)
{
    void* p = calloc(number, size);
    if (p == NULL)
        ERROR("Failed memory allocation", exit(1));
    return p;
}

void* safeRealloc(void* ptr, size_t size)
{
    void* p = realloc(ptr, size);
    if (p == NULL)
        ERROR("Failed memory allocation", exit(1));
    return p;
}

char* safeStrdup(char* s)
{
    char* str = strdup(s);
    if (str == NULL)
        ERROR("Failed memory allocation", exit(1));
    return str;
}

char* safeStrAppend(char* x, const char* y)
{
    size_t len1 = strlen(x);
    size_t len2 = strlen(y);
    x = (char*) safeRealloc(x, (len1 + len2 + 1) * sizeof(char));
    return (char*) memcpy((void*) &x[len1], (void*) y, (len2 + 1));
}

gzFile safeGzOpen(char* filename, char* mode)
{
    gzFile fp = gzopen(filename, mode);
    if (fp == NULL)
        ERROR("Unable to open the file", exit(1));
    return fp;
}

FILE* safeFOpen(char* filename, char* mode)
{
    FILE* fp = fopen(filename, mode);
    if (fp == NULL)
        ERROR("Unable to open the file", exit(1));
    return fp;
}
