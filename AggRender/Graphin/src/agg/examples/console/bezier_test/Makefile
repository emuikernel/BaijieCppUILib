include ../../../Makefile.common.in
PROGNAME = bezier_test
OUTNAME = $(PROGNAME)

CXXFLAGS= $(AGGCXXFLAGS) -I../../../include \
-L../../../src

LIBS = $(AGGLIBS)

SRC=\
$(PROGNAME).cpp

OBJ=$(SRC:.cpp=.o)

$(OUTNAME):	$(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $(OUTNAME) $(LIBS)

clean:
	rm -f $(PROGNAME) *.o

%.o:	%.cpp
	$(CXX) -c $(CXXFLAGS) $*.cpp -o $@

