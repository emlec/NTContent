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

#include <time.h>
#include "printNTContent.h"

void printHeader(AppParamPtr app)
{
    char* timeStr = (char*) safeCalloc(129, sizeof(char));
    time_t rawtime = time(NULL);
    struct tm* date = gmtime(&rawtime);

    strftime(timeStr, 128, "%a %d %b %Y %X %Z", date);

    fprintf(app->out, "# %s\n", timeStr);
    fprintf(app->out, "# Program: NTContent %.1f\n", app->version);
    fprintf(app->out, "# %s\n", app->commandLine);
    fprintf(app->out, "# Query sequence: %s\t", app->sequence->name);
    fprintf(app->out, "%d:%d\tLength: %lu\n", app->sequence->from, app->sequence->to, app->sequence->length);
    fprintf(app->out, "# Query nucleotide(s): %s\n", app->nuclInput);
    fprintf(app->out, "# Parameters: window_size: %lu\tstep: %lu\n", app->windowSize, app->step);
    fprintf(app->out, "# Position\t%%%s\n", app->nuclInput);

    free(timeStr);
}

void NTContent(AppParamPtr app)
{
    int nt = 0, pos = 0, i = 0, windowCenter;

    windowCenter = app->windowSize < 2 ? 1 : app->windowSize / 2;

    for (pos = 0; pos < app->sequence->length - app->windowSize + 1; pos += app->step)
    {
        nt = 0;
        for (i = 0; i < app->windowSize; i++)
        {
            switch (app->sequence->seq[pos+i])
            {
                case 'A': case 'a': if (app->nucleotide & ADENINE) nt++; break;
                case 'C': case 'c': if (app->nucleotide & CYTOSINE) nt++; break;
                case 'G': case 'g': if (app->nucleotide & GUANINE) nt++; break;
                case 'T': case 't': if (app->nucleotide & THYMINE_URACIL) nt++; break;
                case 'U': case 'u': if (app->nucleotide & THYMINE_URACIL) nt++; break;
                default: ERROR("Unrecognized nucleotide in the sequence", exit(1));
            }
        }
        fprintf(app->out, "%d\t%.2f\n", pos + app->sequence->from + windowCenter -1, (float)nt * 100 / (float)app->windowSize);
    }
}

static char* compatibleRegionName(char* regionInput)
{
    char* p = strpbrk(regionInput, ":");
    *p = '-';
    return regionInput;
}

void printPlotScript(AppParamPtr app)
{
    FILE* plotScript = NULL;
    size_t seqNameLength = strlen(app->sequence->filename);
    size_t nuclInputLength = strlen(app->nuclInput);
    int fromLength = snprintf(NULL, 0, "%d", app->sequence->from);
    int toLength = snprintf(NULL, 0, "%d", app->sequence->to);
    char* filename = filename = (char*)safeCalloc(seqNameLength + nuclInputLength + fromLength + toLength + 22, sizeof(char));

    if (app->regionInput != NULL)
    {
        if (app->depth)
            sprintf(filename, "%s.%s_content.depth_%s.plt", app->sequence->filename, app->nuclInput, compatibleRegionName(app->regionInput));
        else
            sprintf(filename, "%s.%s_content_%s.plt", app->sequence->filename, app->nuclInput, compatibleRegionName(app->regionInput));
    }
    else
    {
        if (app->depth)
            sprintf(filename, "%s.%s_content.depth.plt", app->sequence->filename, app->nuclInput);
        else
            sprintf(filename, "%s.%s_content.plt", app->sequence->filename, app->nuclInput);
    }

    plotScript = safeFOpen(filename, "w");
    free(filename);

    fprintf(plotScript, "set term svg size 1024, 320\n");
    fprintf(plotScript, "set size 1, 1\n");
    fprintf(plotScript, "set xlabel \"position\"\n");
    fprintf(plotScript, "set ylabel \"%%%s\"\n", app->nuclInput);
    fprintf(plotScript, "set yrange [0:100]\n");
    fprintf(plotScript, "set style data lines\n");
    fprintf(plotScript, "set output ARG1\n");
    fprintf(plotScript, "set title \'%s %d:%d\' noenhanced\n", app->sequence->name, app->sequence->from, app->sequence->to);

    if (app->depth)
    {
        fprintf(plotScript, "set ytics nomirror\n");
        fprintf(plotScript, "set y2label \"depth\"\n");
        fprintf(plotScript, "set y2tics\n");
        fprintf(plotScript, "plot ARG2 using 1:2 axes x1y1 title \"%%%s\", \\\n", app->nuclInput);
        fprintf(plotScript, "\tARG3 using 2:3 axes x1y2 title \"coverage\"");
    }
    else
    {
        fprintf(plotScript, "unset key\n");
        fprintf(plotScript, "plot ARG2 using 1:2");
    }
    fclose(plotScript);
}
