EXE := mp3info


BINDIR := bin

CXX := g++
AR := ar
RANLIB := ranlib
CXXFLAGS := -std=c++17 -pthread -O3 -Wall -c -fmessage-length=0 -fPIC -I./src
LKRFLAGS := -ltag


LIB := $(addprefix $(LIBDIR)/,$(LIBNAME).so $(LIBNAME).a)

OBJ := $(patsubst %.cpp,%.o,$(filter %.cpp, $(shell find ./src -name '*.cpp')))

MKDIR := mkdir -p
RM := rm -fr
CP := cp


.PHONY: all
all: prepare $(EXE)

.PHONY: prepare
prepare:
	$(MKDIR) $(BINDIR)


clean:
	$(RM) *.o

.PHONY: distclean
distclean: clean
	$(RM) $(BINDIR)/${EXE}

$(EXE): $(OBJ)
	$(CXX) $(LKRFLAGS) $^ -o $(BINDIR)/$@ 
	
%.o: %.c
	$(CXX) $(CXXFLAGS) -c $@ $<
