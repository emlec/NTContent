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

#include <getopt.h>
#include "sequence.h"

#define VERSION 0.1

static void usage(FILE* out)
{
    fprintf(out, "NTContent. Last compilation: %s at %s.\n\n", __DATE__, __TIME__);
    fprintf(out, "Usage: NTContent [options] <Query sequence>\n");
    fprintf(out, "Options:\n");
    fprintf(out, " --output         | -o FILE       Output file (default: stdout)\n");
    fprintf(out, " --step           | -s INT        Step\n");
    fprintf(out, " --windowSize     | -w INT        Window Size\n");
    fprintf(out, " --nucleotide     | -n STR        Nucleotide(s) to look for [ACGTUWSMKRYBDHVNZ] (default: GC)\n");
    fprintf(out, " --region         | -r STR        Region of interest 'FROM:TO'\n");
    fprintf(out, " --help           | -h            Get help (this screen)\n");
    fprintf(out, "\n");
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

int main(int argc, char** argv)
{
    AppParamPtr app = NULL;
    int i = 0, c = 0, option_index = 0;
    static struct option long_options[] =
    {
        {"output", required_argument, 0, 'o'},
        {"step", required_argument, 0, 's'},
        {"windowSize", required_argument, 0, 'w'},
        {"nucleotide", required_argument, 0, 'n'},
        {"region", required_argument, 0, 'r'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };

    if (argc < 2)
    {
        usage(stderr);
        return EXIT_FAILURE;
    }

    // Create new AppParam
    app = initApp(VERSION);

    // Get program options
    while ((c = getopt_long(argc, argv,"o:s:w:n:r:h", long_options, &option_index)) != -1)
    {
        switch (c)
        {
            case 'o': app->out = openOutFile(optarg); break;                            // Program output
            case 's': app->step = (size_t) strtol(optarg, NULL, 10); break;             // Step
            case 'w': app->windowSize = (size_t) strtol(optarg, NULL, 10); break;       // Window Size
            case 'n': app->nucleotide = parseNucl(app, optarg); break;                  // Nucleotide(s) to look for
            case 'r': parseRegion(app->sequence, optarg); break;                        // Region of interest
            case 'h': usage(stderr); clearApp(app); return EXIT_SUCCESS; break;         // Help
            case ':': fprintf(stderr, "Option -%c requires an argument\n", optopt); return EXIT_FAILURE; break;
            case '?': fprintf(stderr, "Option -%c is undefined\n", optopt); return EXIT_FAILURE; break;
        }
    }

    // Set default values
    if (app->out == NULL) app->out = stdout;
    if (app->nuclInput == NULL) app->nucleotide = parseNucl(app, "GC");

    // Execution trace
    app->commandLine = safeStrdup("Command Line: ");
    for (i = 0; i < argc; i++)
    {
        safeStrAppend(app->commandLine, " ");
        safeStrAppend(app->commandLine, argv[i]);
    }

    if (argc < optind + 1)
    {
        clearApp(app);
        usage(stderr);
        return EXIT_FAILURE;
    }

    app->sequence->filename = argv[optind]; // Get the query sequence

    analyzeSeq(app->sequence);
    autoParam(app);
    printHeader(app);

    NTContent(app);

    clearApp(app);
    return EXIT_SUCCESS;
}
