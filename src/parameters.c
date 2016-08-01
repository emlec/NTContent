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

#include "parameters.h"

AppParamPtr initApp(float version)
{
    AppParamPtr app = (AppParamPtr) safeCalloc(1, sizeof(AppParam));
    app->version = version;
    app->sequence = (SequencePtr) safeCalloc(1, sizeof(Sequence));
    return app;
}

void clearApp(AppParamPtr app)
{
    if (app->sequence->name != NULL) free(app->sequence->name);
    if (app->sequence->seq != NULL) free(app->sequence->seq);
    free(app->sequence);
    if (app->commandLine != NULL) free(app->commandLine);
    if (app->out != NULL && app->out != stdout && app->out != stderr)
        fclose(app->out);
    free(app);
}

FILE* openOutFile(char* filename)
{
    FILE* out = NULL;
    if (!strcmp(filename, "stdout")) out = stdout;
    else if (!strcmp(filename, "stderr")) out = stderr;
    else if (!strcmp(filename, "stdin")) out = stdout;
    else out = safeFOpen(filename, "w");
    return out;
}

int parseNucl(AppParamPtr app, char* input)
{
    int nucl = 0, i = 0;
    size_t length = 0;

    app->nuclInput = input;
    length = strlen(input);
    for (i = 0; i < length; i++)
    {
        switch (input[i])
        {
            case 'A': case 'a': nucl |= ADENINE; break;
            case 'C': case 'c': nucl |= CYTOSINE; break;
            case 'G': case 'g': nucl |= GUANINE; break;
            case 'T': case 't': nucl |= THYMINE_URACIL; break;
            case 'U': case 'u': nucl |= THYMINE_URACIL; break;
            case 'W': case 'w': nucl |= (ADENINE | THYMINE_URACIL); break;
            case 'S': case 's': nucl |= (CYTOSINE | GUANINE); break;
            case 'M': case 'm': nucl |= (ADENINE | CYTOSINE); break;
            case 'K': case 'k': nucl |= (GUANINE | THYMINE_URACIL); break;
            case 'R': case 'r': nucl |= (ADENINE | GUANINE); break;
            case 'Y': case 'y': nucl |= (CYTOSINE | THYMINE_URACIL); break;
            case 'B': case 'b': nucl |= (CYTOSINE | GUANINE | THYMINE_URACIL); break;
            case 'D': case 'd': nucl |= (ADENINE | GUANINE | THYMINE_URACIL); break;
            case 'H': case 'h': nucl |= (ADENINE | CYTOSINE | THYMINE_URACIL); break;
            case 'V': case 'v': nucl |= (ADENINE | CYTOSINE | GUANINE); break;
            case 'N': case 'n': nucl |= (ADENINE | CYTOSINE | GUANINE | THYMINE_URACIL); break;
            case 'Z': case 'z': return 0; break;
            default: ERROR("Wrong nuclotide input", exit(1));
        }
    }
    return nucl;
}

void parseRegion(SequencePtr seq, char* input)
{
    char* p = NULL;
    size_t lenInput = strlen(input);
    char* str = (char*) safeCalloc(lenInput, sizeof(char));
    int temp = 0;

    if (input == NULL) ERROR("Wrong region parameter", exit(1));

    strncpy(str, input, lenInput);
    p = strpbrk(str,":");
    if (p == NULL) ERROR("Wrong region parameter", exit(1));

    *p = '\0';

    seq->from = (int) strtol(str, NULL, 10);
    if (strlen(str) != lenInput -1)
        seq->to = (int) strtol(p+1, NULL, 10);
    if (seq->to > 0 && seq->from > seq->to)
    {
        temp = seq->from;
        seq->from = seq->to;
        seq->to = temp;
    }
    free(str);
}

void autoParam(AppParamPtr app)
{
    if (!app->windowSize)
        app->windowSize = app->sequence->length / 50;
    if (app->windowSize < 1)
        app->windowSize = 1;
    if (app->windowSize > app->sequence->length)
        app->windowSize = app->sequence->length;

    if (!app->step)
        app->step = app->windowSize / 2;
    if (app->step < 1)
        app->step = 1;
}
