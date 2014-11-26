include ../../../Makefile.in.$(shell uname).SDL

PROGNAME=alpha_mask3
OUTNAME=$(PROGNAME)
PLATFORM=sdl

CXXFLAGS= $(AGGCXXFLAGS) -I../../../include -I../../../gpc \
-L../../../src \
$(PIXFMT)

LIBS = $(AGGLIBS) -lm

SRC_CXX = \
../../$(PROGNAME).cpp \
../../../src/platform/$(PLATFORM)/agg_platform_support.cpp \
../../make_arrows.cpp \
../../make_gb_poly.cpp

OBJ = $(SRC_CXX:.cpp=.o)

$(PROGNAME):	$(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $(OUTNAME) $(LIBS)

clean:
	rm -f $(PROGNAME)
	rm -f *.o
	rm -f ../../*.o
	rm -f ../../../src/platform/$(PLATFORM)/agg_platform_support.o
	rm -rf SunWS_cache
	rm -rf ../../SunWS_cache
	rm -rf ../../../src/platform/$(PLATFORM)/SunWS_cache

%.o:	%.cpp
	@echo \< $*.cpp \>
	$(CXX) -c $(CXXFLAGS) $*.cpp -o $@

