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

#ifndef PARAMETERS_H_INCLUDED
#define PARAMETERS_H_INCLUDED

#include "utils.h"

#define ADENINE            0x1
#define CYTOSINE           0x2
#define GUANINE            0x4
#define THYMINE_URACIL     0x8

typedef struct Sequence
{
    int from;
    int to;
    size_t length;
    char* filename;
    char* seq;
    char* name;
    gzFile fp;
}Sequence, *SequencePtr;

typedef struct AppParam
{
    size_t step;
    size_t windowSize;
    int nucleotide;
    int plot;
    int depth;
    int region;
    float version;
    char* commandLine;
    char* nuclInput;
    FILE* out;
    SequencePtr sequence;
}AppParam, *AppParamPtr;

AppParamPtr initApp(float version);
void clearApp(AppParamPtr app);
FILE* openOutFile(char* filename);
int parseNucl(AppParamPtr app, char* input);
void parseRegion(SequencePtr seq, char* input);
void autoParam(AppParamPtr app);

#endif // PARAMETERS_H_INCLUDED
