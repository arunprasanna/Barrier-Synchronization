OMPFLAGS = -fopenmp
OMPLIBS = -lgomp

CC = gcc
CPPFLAGS = -g -Wall
LDFLAGS = -g -Wall
LDLIBS = $(OMPLIBS)

MPICC = mpicc
MPICH = /usr/lib64/openmpi/1.4-gcc
CFLAGS = -I$(MPICH)/include

all: hello_world communicate_world nash tournament dissemination kim kimd gta openmp_mpi

hello_world: hello_world.o
	$(MPICC) -o $@ $(LDFLAGS) $^ $(LDLIBS)

communicate_world: communicate_world.o
	$(MPICC) -o $@ $(LDFLAGS) $^ $(LDLIBS)

nash: nash.o
	$(MPICC) -o $@ $(LDFLAGS) $^ $(LDLIBS)

tournament: tournament.o
	$(MPICC) -o $@ $(LDFLAGS) $^ $(LDLIBS)

dissemination: dissemination.o
	$(MPICC) -o $@ $(LDFLAGS) $^ $(LDLIBS)

kim: kim.o
	$(MPICC) -o $@ $(LDFLAGS) $^ $(LDLIBS)

kimd: kimd.o
	$(MPICC) -o $@ $(LDFLAGS) $^ $(LDLIBS)

gta: gta.o
	$(MPICC) -o $@ $(LDFLAGS) $^ $(LDLIBS)


openmp_mpi: openmp_mpi.o
	$(MPICC) -o $@ $(LDFLAGS) $^ $(LDLIBS) $(OMPLIBS)

openmp_mpi.o: openmp_mpi.c
	$(CC) -c -o $@ $(CPPFLAGS) $(CFLAGS) $(OMPFLAGS) $^

clean:
	rm -f *.o hello_world communicate_world openmp_mpi
