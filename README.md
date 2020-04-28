# NTContent

**NTContent** is a sliding window based tool used to assess the evolution of the percentage of specific nucleotides along a DNA/RNA sequence.

The input is a DNA/RNA sequence in fasta format.

The output is a tab-delimited text file showing for each window its central position and the percentage of the desired nucleotides.

Additionally, a gnuplot script can be created with the -p option.

# Requirements

- gcc
- zlib

# Compilation

```
$ make
```

# Usage

```bash
$ NTContent [options] sequence.fasta
```

# Options

<table>
<tr><th>Option</th><th>Description</th></tr>
<tr><th>-o FILE</th><td>File output. Default: stdout</td></tr>
<tr><th>-w INT</th><td>Window Size. Default: Sequence length / 25</td></tr>
<tr><th>-s INT</th><td>Step. Default: Window Size / 10</td></tr>
<tr><th>-n STR</th><td>Nucleotide(s) to look for [ACGTUWSMKRYBDHVNZ]. Default: GC</td></tr>
<tr><th>-r STR</th><td>Region of interest [FROM:TO]</td></tr>
<tr><th>-p</th><td>Create a gnuplot script</td></tr>
<tr><th>-d</th><td>Combine 'samtools depth' results with NTContent in the gnuplot script</td></tr>
<tr><th>-h</th><td>Help</td></tr>
</table>

# Example

### Command Line

```bash
$ NTContent -n GC -w 50 -s 10 hiv.fasta > GC.txt
```

### Output

```
(...)
25	54.00
35	56.00
45	56.00
55	52.00
65	48.00
75	48.00
85	46.00
95	46.00
105	52.00
115	48.00
125	52.00
135	48.00
145	46.00
155	42.00
165	54.00
175	54.00
185	54.00
195	56.00
205	60.00
(...)
```

# Author

Aurélien Guy-Duché

# Contribute

- Issue Tracker: https://github.com/guyduche/NTContent/issues
- Source Code: https://github.com/guyduche/NTContent

# License

The project is licensed under the MIT2 license.


