# Simple Makefile for 6.837 particle system assignment
# for g++ on linux platforms

# ===============================================================

SRCS 	 	= main.C \
		  matrix.C \
		  glCanvas.C \
		  system.C \
		  particle.C \
		  parser.C 

# ===============================================================
# NOTE:  On athena, the Mesa GL libraries are currently located in the
#        mesa_v501 locker.  Type "add mesa_v501" to access these files.

LIBS            = -lm -lGL -lGLU -lglut -L/usr/X11R6/lib -lXmu -lXi \
		  -Xlinker -rpath -Xlinker /mit/mesa_v501/lib/ -L/mit/mesa_v501/lib/
INCLUDE 	= -I/mit/mesa_v501/Mesa-5.0.1/include/
CC		= g++
EXE	  	= particle_system
OBJS =  $(SRCS:.C=.o)

# ===============================================================
# targets

.PHONY: all depend clean

all: depend $(EXE)

depend:
	$(CC) $(INCLUDE) -E -M $(SRCS) > Makefile.depend

clean: 
	-rm -f *~ *bak *.o  core.* Makefile.depend $(EXE) 

# ===============================================================
# compilation rules

$(EXE): $(OBJS)
	$(CC) $(INCLUDE) -o $@ $(OBJS) $(LIBS)

.C.o:
	$(CC) $(INCLUDE) $< -c -o $@

# ===============================================================

-include Makefile.depend