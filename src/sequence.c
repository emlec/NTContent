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

#include "sequence.h"

static char* seqName(gzFile fp)
{
    int c = 0;
    size_t sizeStr = 0;
    char* name = NULL;

    if (gzgetc(fp) != '>')
        ERROR("Wrong sequence file format", exit(1));

    c = gzgetc(fp);

    while (c != '\n' && c != '\r' && c != EOF)
    {
        sizeStr++;
        name = (char*) safeRealloc(name, sizeStr+1);
        name[sizeStr-1] = (char) c;
        c = gzgetc(fp);
    }

    name[sizeStr] = '\0';
    if (c == '\r') c = gzgetc(fp);
    if (gzeof(fp)) ERROR("No sequence in the file", exit(1));

    return name;
}

static void recordSeq(SequencePtr seq)
{
    int c = 0, pos = 0, undefinedEnd = 0;

    seq->name = seqName(seq->fp);

    if (seq->from < 1) seq->from = 1;
    if (seq->to < 1) undefinedEnd = seq->to = 1;

    while (pos < seq->to && !gzeof(seq->fp))
    {
        c = gzgetc(seq->fp);
        if (c == '>')
            ERROR("There cannot be more than one sequence", break);
        if (c != '\r' && c != '\n' && c != EOF)
        {
            pos++;
            if (undefinedEnd) seq->to = pos + 1;
            if (pos >= seq->from)
            {
                seq->length++;
                seq->seq = (char*) safeRealloc(seq->seq, seq->length+1);
                seq->seq[seq->length -1] = (char) c;
            }
        }
    }

    seq->seq[seq->length] = '\0';
    seq->to = seq->from + seq->length -1;

    if (seq->seq == NULL)
        ERROR("No sequence in the file", exit(1));
}

void analyzeSeq(SequencePtr seq)
{
    seq->fp = safeGzOpen(seq->filename, "r");
    recordSeq(seq);
    gzclose(seq->fp);
}
