INCFLAGS  = -I /usr/include/GL
INCFLAGS += -I /usr/include/vecmath
# INCFLAGS += -I /mit/6.837/public/include/vecmath

LINKFLAGS = -lglut -lGL -lGLU
LINKFLAGS += -L /usr/lib -lvecmath
# LINKFLAGS += -L /mit/6.837/public/lib -lvecmath
LINKFLAGS += -L /usr/lib64 -lfltk -lfltk_gl
# LINKFLAGS += -L/usr/lib64/libfltk.so.1.3
# LINKFLAGS += -L/usr/lib64/lfltk_gl.so.1.3
# LINKFLAGS += -L/usr/lib64/lfltk_forms.so.1.3
# LINKFLAGS += -L/usr/lib64/lfltk_images.so.1.3

# LIBS = -lfltk -lfltk_images -ljpeg -lstdc++ -lXfixes -lXext -lpthread -ldl -lm -lX11

CFLAGS    = -g
CFLAGS    += -DSOLN -I/home/dom/modules3/811COMP-Graphics/MIT_Assignments/2-Hierarchical-Modeling/distrib/
CC        = g++
SRCS      = bitmap.cpp camera.cpp MatrixStack.cpp modelerapp.cpp modelerui.cpp ModelerView.cpp Joint.cpp SkeletalModel.cpp Mesh.cpp main.cpp
OBJS      = $(SRCS:.cpp=.o)
PROG      = a2

all: $(SRCS) $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LINKFLAGS) $(LIBS)

.cpp.o:
	$(CC) $(CFLAGS) $< -c -o $@ $(INCFLAGS)

depend:
	makedepend $(INCFLAGS) -Y $(SRCS)

clean:
	rm $(OBJS) $(PROG)

bitmap.o: bitmap.h
camera.o: camera.h
Mesh.o: Mesh.h
MatrixStack.o: MatrixStack.h
modelerapp.o: modelerapp.h ModelerView.h modelerui.h bitmap.h camera.h
modelerui.o: modelerui.h ModelerView.h bitmap.h camera.h modelerapp.h
ModelerView.o: ModelerView.h camera.h
SkeletalModel.o: MatrixStack.h ModelerView.h Joint.h modelerapp.h
