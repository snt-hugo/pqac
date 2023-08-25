CC = gcc 
CFLAGS = -O3 
LD = gcc 
LIBS = -lmpfr -lgmp -lm -ldgs -lssl -lcrypto

BINDIR = bin
INCDIR = include
OBJDIR = obj
SRCDIR = src
DGS_INCDIR=/usr/local/include/dgs

SOURCES_FILENAMES = API.c benchmark.c benchmark_compression.c compression_rice.c extra_tools.c\
                    hash.c new_arithm_int.c new_arithm_poly.c params.c protocol.c samplers.c shake.c\
                    test_arithm_int.c test_arithm_poly.c test_compression.c test_protocol.c rng.c

API_OBJECTS_FILENAMES = API.o compression_rice.o extra_tools.o hash.o new_arithm_int.o\
                        new_arithm_poly.o params.o protocol.o samplers.o shake.o rng.o

HEADERS_FILENAMES = API.h compression_rice.h extra_tools.h hash.h new_arithm_int.h new_arithm_poly.h\
                    params.h protocol.h samplers.h shake.h pblc_hash.ac prvt_edward.ac prvt_emma.ac\
                    prvt_julian.ac prvt_phoebe.ac rng.h

SOURCES = $(SOURCES_FILENAMES:%=$(SRCDIR)/%)
HEADERS = $(HEADERS_FILENAMES:%=$(INCDIR)/%)
OBJECTS = $(SOURCES_FILENAMES:%.c=$(OBJDIR)/%.o)
API_OBJECTS = $(API_OBJECTS_FILENAMES:%=$(OBJDIR)/%)

INC_PARAMS = $(DGS_INCDIR) $(INCDIR)
INC = $(foreach d, $(INC_PARAMS), -I $d)

PROGRAM_FILENAMES = benchmark benchmark_compression test_arithm_int test_arithm_poly\
                    test_compression test_protocol
PROGRAMS = $(PROGRAM_FILENAMES:%=$(BINDIR)/%)

all: $(PROGRAMS)

$(PROGRAMS): $(BINDIR)/%: $(OBJDIR)/%.o $(API_OBJECTS)
	@mkdir -p $(BINDIR)
	$(LD) $(LDFLAGS) $(INC) -o $@ $< $(API_OBJECTS) $(LIBS) 

$(OBJECTS): $(OBJDIR)/%.o: $(SRCDIR)/%.c $(HEADERS)
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	-rm -f $(OBJDIR)/*.o $(BINDIR)/*
