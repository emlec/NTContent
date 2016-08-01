.PHONY = all clean destroy
.SHELL = /bin/bash
GNUPLOT = ~/Programs/gnuplot-5.0.3/src/gnuplot
BINDIR = ~/Programs/NTContent/bin
SRCDIR = ~/Programs/NTContent/src
SEQ =
NUCL = GC
DEPTH =
REGION =
REG = $(subst :,-,${REGION})

ifndef DEPTH
    ifndef REGION
all: $(notdir ${SEQ}).${NUCL}_content.png
$(notdir ${SEQ}).${NUCL}_content.svg: ${SEQ} ${BINDIR}/NTContent
	${BINDIR}/NTContent -p -n ${NUCL} $< > $(basename $@).txt
	${GNUPLOT} -c $(basename $@).plt $@ $(basename $@).txt

    else
all: $(notdir ${SEQ}).${NUCL}_content_${REG}.png
$(notdir ${SEQ}).${NUCL}_content_${REG}.svg: ${SEQ} ${BINDIR}/NTContent
	${BINDIR}/NTContent -p -n ${NUCL} -r ${REGION} $< > $(basename $@).txt
	${GNUPLOT} -c $(basename $@).plt $@ $(basename $@).txt

    endif

else
    ifndef REGION
all: $(notdir ${SEQ}).${NUCL}_content.depth.png
$(notdir ${SEQ}).${NUCL}_content.depth.svg: ${SEQ} ${BINDIR}/NTContent
	${BINDIR}/NTContent -p -d -n ${NUCL} $< > $(basename $@).txt
	${GNUPLOT} -c $(basename $@).plt $@ $(basename $@).txt ${DEPTH}

    else
all: $(notdir ${SEQ}).${NUCL}_content.depth_${REG}.png
$(notdir ${SEQ}).${NUCL}_content.depth_${REG}.svg: ${SEQ} ${BINDIR}/NTContent
	${BINDIR}/NTContent -p -d -n ${NUCL} -r ${REGION} $< > $(basename $@).txt
	${GNUPLOT} -c $(basename $@).plt $@ $(basename $@).txt ${DEPTH}

    endif
endif

%.png: %.svg
	inkscape -z -y 1.0 -e $@ $<
	rm -f $<

${BINDIR}/NTContent:
	(cd ${SRCDIR}; ${MAKE})

clean:
	rm -f *.txt *.plt *.png

destroy: clean
	(cd ${SRCDIR}; ${MAKE} clean)
