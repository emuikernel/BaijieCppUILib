include ../../../Makefile.in.$(shell uname).SDL

PROGNAME=rounded_rect
OUTNAME=$(PROGNAME)
PLATFORM=sdl

CXXFLAGS= $(AGGCXXFLAGS) -I../../../include \
-L../../../src \
$(PIXFMT)

LIBS = $(AGGLIBS) -lm

SRC=\
../../$(PROGNAME).cpp \
../../../src/platform/$(PLATFORM)/agg_platform_support.cpp

OBJ= $(SRC:.cpp=.o)

$(OUTNAME):	$(OBJ)
	$(CXX) $(CXXFLAGS)  $^ -o $(OUTNAME) $(LIBS)

clean:
	rm -f $(OUTNAME)
	rm -f ../../$(PROGNAME).o
	rm -f ../../../src/platform/$(PLATFORM)/agg_platform_support.o
	rm -rf SunWS_cache
	rm -rf ../../SunWS_cache
	rm -rf ../../../src/platform/$(PLATFORM)/SunWS_cache

%.o:	%.cpp
	@echo \< $*.cpp \>
	$(CXX) -c $(CXXFLAGS) $*.cpp -o $@


