EXE := mp3info
LIBNAME := libmp3info

BINDIR := bin
LIBDIR := lib

CXX := g++
AR := ar
RANLIB := ranlib
CXXFLAGS := -std=c++17 -pthread -O3 -Wall -c -fmessage-length=0 -fPIC -I./src
LKRFLAGS := 


LIB := $(addprefix $(LIBDIR)/,$(LIBNAME).so $(LIBNAME).a)

OBJ := $(patsubst %.cpp,%.o,$(filter %.cpp, $(shell find ./src -name '*.cpp')))
LIBOBJ := $(filter-out %mp3info.o,$(OBJ))

MKDIR := mkdir -p
RM := rm
CP := cp


.PHONY: all
all: app lib

.PHONY: app
app: prepare $(EXE)

.PHONY: lib
lib: prepare $(LIB)

.PHONY: prepare
prepare:
	$(MKDIR) $(BINDIR)
	$(MKDIR) $(LIBDIR)


clean:
	$(RM) $(OBJ)

.PHONY: distclean
distclean: clean
	$(RM) $(BINDIR)/${EXE}
	
.PHONY: list
list:
	$(info OBJS are  $(OBJ))
	$(info LIB-OBJS are $(LIBOBJ))
	$(info EXE is $(EXE))
	$(info LIB is $(LIB))

	

$(EXE): $(OBJ)
	$(CXX) $(LKRFLAGS) $^ -o $(BINDIR)/$@ 
	
$(LIB): $(LIBOBJ)
	$(AR) cr $@ $^
	$(RANLIB) $@
	
%.o: %.c
	$(CXX) $(CXXFLAGS) -c $@ $<
