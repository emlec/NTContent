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
#include "printNTContent.h"

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
    fprintf(out, " --region         | -r STR        Region of interest [FROM:TO]\n");
    fprintf(out, " --plot           | -p            Create a gnuplot script\n");
    fprintf(out, " --depth          | -d            Combine 'samtools depth' results with NTContent in the gnuplot script\n");
    fprintf(out, " --help           | -h            Get help (this screen)\n");
    fprintf(out, "\n");
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
        {"plot", no_argument, 0, 'p'},
        {"depth", no_argument, 0, 'd'},
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
    while ((c = getopt_long(argc, argv,"o:s:w:n:r:pdh", long_options, &option_index)) != -1)
    {
        switch (c)
        {
            case 'o': app->out = openOutFile(optarg); break;                            // Program output
            case 's': app->step = (size_t) strtol(optarg, NULL, 10); break;             // Step
            case 'w': app->windowSize = (size_t) strtol(optarg, NULL, 10); break;       // Window Size
            case 'n': app->nucleotide = parseNucl(app, optarg); break;                  // Nucleotide(s) to look for
            case 'r': parseRegion(app->sequence, optarg); app->region = 1; break;       // Region of interest
            case 'p': app->plot = 1; break;                                             // Create a gnuplot script file
            case 'd': app->depth = 1; break;                                            // Create a gnuplot script file with depth
            case 'h': usage(stderr); clearApp(app); return EXIT_SUCCESS; break;         // Help
            case ':': fprintf(stderr, "Option -%c requires an argument\n", optopt); return EXIT_FAILURE; break;
            case '?': fprintf(stderr, "Option -%c is undefined\n", optopt); return EXIT_FAILURE; break;
        }
    }

    if (app->depth && !app->plot)
        ERROR("To create a plot, -p need to be used", app->depth = 0);

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
    if (app->plot) printPlotScript(app);

    clearApp(app);
    return EXIT_SUCCESS;
}
