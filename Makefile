
CPP    = /usr/bin/g++
YACC   = /usr/bin/bison -d
LEX    = /usr/bin/flex
PURIFY = /usr/bin/purify
QUANT  = /usr/bin/quantify

#CPPFLAGS = -g -Wall
CPPFLAGS = -O3

PROG     = booksim

CPP_SRCS = main.cpp \
	config_utils.cpp \
	booksim_config.cpp \
	module.cpp \
	router.cpp \
	iq_router.cpp \
	event_router.cpp \
	vc.cpp \
	routefunc.cpp \
	traffic.cpp \
	allocator.cpp \
	maxsize.cpp \
	network.cpp \
	singlenet.cpp \
	kncube.cpp \
	fly.cpp \
	trafficmanager.cpp \
	random_utils.cpp \
	buffer_state.cpp \
	stats.cpp \
	pim.cpp \
	islip.cpp \
	loa.cpp \
	wavefront.cpp \
	misc_utils.cpp \
	credit.cpp \
	outputset.cpp \
	flit.cpp \
	selalloc.cpp \
	arbiter.cpp \
	injection.cpp \
	rng_wrapper.cpp \
	rng_double_wrapper.cpp

LEX_OBJS  = configlex.o
YACC_OBJS = config_tab.o

#--- Make rules ---

OBJS = $(CPP_SRCS:.cpp=.o) $(LEX_OBJS) $(YACC_OBJS)

.PHONY: clean
.PRECIOUS: %_tab.cpp %_tab.hpp %lex.cpp

$(PROG): $(OBJS)
	$(CPP) $(OBJS) -o $(PROG)

purify: $(OBJS)
	$(PURIFY) -always-use-cache-dir $(CPP) $(OBJS) -o $(PROG) -L/usr/lib

quantify: $(OBJS)
	$(QUANT) -always-use-cache-dir $(CPP) $(OBJS) -o $(PROG) -L/usr/lib

%lex.o: %lex.cpp %_tab.hpp
	$(CPP) $(CPPFLAGS) -c $< -o $@

%.o: %.cpp
	$(CPP) $(CPPFLAGS) -c $< -o $@

%.o: %.c
	$(CPP) $(CPPFLAGS) $(VCSFLAGS) -c $< -o $@

%_tab.cpp: %.y
	$(YACC) -b$* -p$* $<
	cp -f $*.tab.c $*_tab.cpp

%_tab.hpp: %_tab.cpp
	cp -f $*.tab.h $*_tab.hpp

%lex.cpp: %.l
	$(LEX) -P$* -o$@ $<

clean:
	rm -f $(OBJS) *_tab.cpp *_tab.hpp *.tab.c *.tab.h *lex.cpp
	rm -f $(PROG)
