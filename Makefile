CC  = gcc
CXX = g++
RM  = rm -f

CXXFLAGS = -shared -fPIC -ggdb -std=c++14 -I.
CFLAGS   = -shared -fPIC -ggdb -I.

CXXSOURCES = asm32.cc binreader.cc binwriter.cc
CSOURCES   = vmexec.c

CXXOBJECTS  = asm32.o binreader.o binwriter.o vmexec.o
LDLIBS      = -L. -lasmjit -lasmtk -lstdc++

all: $(CXXOBJECTS) libstub32.so test

%.o: %.cc
	$(CXX) $(CXXFLAGS) $(CXXFLAGS) -c $<
    
%.o: %.c 
	$(CC) $(CFLAGS) $(CFLAGS) -c $<
    
libstub32.so: $(CXXOBJECTS) $(COBJECTS)
	$(CXX) $(CXXFLAGS) -o libstub32.so $(CXXOBJECTS) $(COBJECTS) $(LDLIBS)

test: stub32.cc
	$(CXX) -ggdb -o test stub32.cc -L. -l stub32 -lasmjit -lasmtk

depend: .dependxx .depend

.dependxx: $(CXXSOURCES)
	$(RM) ./.depend
	$(CXX) $(CXXFLAGS) -MM $^ >> ./.depend;
    
.depend: $(CSOURCES)
	$(CC)  $(CSOURCES) -MM $^ >> ./.depend;
    
clean:
	$(RM) $(CXXOBJECTS) stub32.o
	$(RM) libstub32.so
	$(RM) test
    
distclean: clean
	$(RM) ./.depend

include .depend
