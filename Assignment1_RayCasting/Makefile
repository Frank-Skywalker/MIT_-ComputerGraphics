# Simple Makefile for 6.837 raycasting assignment # for g++ on linux platforms # =============================================================== 
#SRCS = main.C \ matrix.C \ image.C \ scene_parser.C \ sphere.C \ raytracer.C \ camera.C \ group.C 
# =============================================================== LIBS = -lm -L/usr/X11R6/lib CC = g++ EXE = raytracer OBJS = $(SRCS:.C=.o) 
# =============================================================== # targets .PHONY: all depend clean all: depend $(EXE) depend: $(CC) -E -M $(SRCS) > Makefile.depend clean: -rm -f *~ *bak *.o core.* Makefile.depend $(EXE)
# =============================================================== # compilation rules $(EXE): $(OBJS) $(CC) -o $@ $(OBJS) $(LIBS) .C.o: $(CC) $< -c -o $@ .cpp.o: $(CC) $< -c -o $@ 
# =============================================================== -include Makefile.depend

raycaster: main.o 

main.o: scene_parser.o image.o hit.o ray.o raytracer.o

scene_parser.o: vectors.o matrix.o camera.o material.o object3d.o group.o sphere.o orthographiccamera.o

image.o: vectors.o

hit.o: vectors.o ray.o

ray.o: vectors.o

raytracer.o: scene_parser.o image.o hit.o camera.o group.o

vectors.o:

camera.o: ray.o

group.o: object3d.o

object3d.o: material.o ray.o hit.o

material.o: vectors.o

orthographiccamera.o:camera.o vectors.o

sphere.o: 