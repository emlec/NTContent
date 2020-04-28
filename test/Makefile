.PHONY = all clean destroy
.SHELL = /bin/bash
GNUPLOT = gnuplot
BINDIR = ../bin
SRCDIR = ../src
SEQ =
NUCL = GC
WINDOWSIZE = 0
STEP = 0
DEPTH =
REGION =
REG = $(subst :,-,${REGION})
REGDEPTH = $(subst :, ,.${REGION})
FROM = $(patsubst .%,%,$(word 1, ${REGDEPTH}))
TO = $(word 2, ${REGDEPTH})

ifdef REGION
    ifdef DEPTH
all: $(notdir ${SEQ}).${NUCL}_content.depth_${REG}.png
$(notdir ${SEQ}).${NUCL}_content.depth_${REG}.svg: ${SEQ} ${BINDIR}/NTContent ${DEPTH}_${REG}.txt
	${BINDIR}/NTContent -p -d -w ${WINDOWSIZE} -s ${STEP} -n ${NUCL} -r ${REGION} $< > $(basename $@).txt
	${GNUPLOT} -c $(basename $@).plt $@ $(basename $@).txt ${DEPTH}_${REG}.txt
	rm -f ${DEPTH}_${REG}.txt

    else
all: $(notdir ${SEQ}).${NUCL}_content_${REG}.png
$(notdir ${SEQ}).${NUCL}_content_${REG}.svg: ${SEQ} ${BINDIR}/NTContent
	${BINDIR}/NTContent -p -w ${WINDOWSIZE} -s ${STEP} -n ${NUCL} -r ${REGION} $< > $(basename $@).txt
	${GNUPLOT} -c $(basename $@).plt $@ $(basename $@).txt

    endif

else
    ifdef DEPTH
all: $(notdir ${SEQ}).${NUCL}_content.depth.png
$(notdir ${SEQ}).${NUCL}_content.depth.svg: ${SEQ} ${BINDIR}/NTContent ${DEPTH}
	${BINDIR}/NTContent -p -d -w ${WINDOWSIZE} -s ${STEP} -n ${NUCL} $< > $(basename $@).txt
	${GNUPLOT} -c $(basename $@).plt $@ $(basename $@).txt ${DEPTH}

    else
all: $(notdir ${SEQ}).${NUCL}_content.png
$(notdir ${SEQ}).${NUCL}_content.svg: ${SEQ} ${BINDIR}/NTContent
	${BINDIR}/NTContent -p -w ${WINDOWSIZE} -s ${STEP} -n ${NUCL} $< > $(basename $@).txt
	${GNUPLOT} -c $(basename $@).plt $@ $(basename $@).txt

    endif
endif

%.png: %.svg
	inkscape -z -y 1.0 -e $@ $<
	rm -f $<

ifneq ($(FROM),)
    ifneq ($(TO),)
${DEPTH}_${REG}.txt: ${DEPTH}
	head -n ${TO} $< | tail -n +${FROM} > $@

    else
${DEPTH}_${REG}.txt: ${DEPTH}
	tail -n +${FROM} $< > $@

    endif
else
    ifneq ($(TO),)
${DEPTH}_${REG}.txt: ${DEPTH}
	head -n ${TO} $< > $@

    else
${DEPTH}_${REG}.txt: ${DEPTH}
	cp $< $@

    endif
endif

${BINDIR}/NTContent:
	(cd ${SRCDIR}; ${MAKE})

clean:
	rm -f *.txt *.plt *.png

destroy: clean
	(cd ${SRCDIR}; ${MAKE} clean)
