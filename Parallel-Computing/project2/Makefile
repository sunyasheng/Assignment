CC = gcc-5
CLINKER = $(CC)
OPTFLAGS = -fopenmp -DAPPLE
LIBS = -lm -lstdc++

CFLAGS = $(OPTFLAGS)

OBJ=o
EXE=
RM=rm

EXES=hello$(EXE) link$(EXE) link_task$(EXE)

all: $(EXES)

hello$(EXE): src/hello.cpp
	$(CLINKER) $(OPTFLAGS) -o hello src/hello.cpp $(LIBS)

link$(EXE): src/link.cpp
	$(CLINKER) $(OPTFLAGS) -o link src/link.cpp $(LIBS)

link_task$(EXE): src/link_task.cpp
	$(CLINKER) $(OPTFLAGS) -o link_task src/link_task.cpp $(LIBS)

clean: 
	$(RM) $(EXES)
