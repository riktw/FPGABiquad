CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-g $( --cflags)
CPPFLAGS +=  -std=c++11
CPPFLAGS += -I/usr/include/python2.7
LDFLAGS=-g $( --ldflags)
LDFLAGS += -lpython2.7
LDLIBS=$( --libs)

SRCS=main.cpp BiQuad_int.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

all: biquad

biquad: $(OBJS)
	$(CXX) $(LDFLAGS) -o biquad $(OBJS) $(LDLIBS)

depend: .depend

.depend: $(SRCS)
	$(RM) ./.depend
	$(CXX) $(CPPFLAGS) -MM $^>>./.depend;

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) *~ .depend

include .depend
