IS_LINUX:=0

ifeq ($(OS),Windows_NT) #if windows
	CCFLAGS += -D WIN32
	ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
		CCFLAGS += -D AMD64
	endif
	ifeq ($(PROCESSOR_ARCHITECTURE),x86)
		CCFLAGS += -D IA32
	endif
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux) #if Linux
		CCFLAGS += -D LINUX
		IS_LINUX:=1
	endif
	ifeq ($(UNAME_S),Darwin) #if OSX
		CCFLAGS += -D OSX
	endif
	UNAME_P := $(shell uname -p)
	ifeq ($(UNAME_P),x86_64)
		CCFLAGS += -D AMD64
	endif
	ifneq ($(filter %86,$(UNAME_P)),)
		CCFLAGS += -D IA32
	endif
	ifneq ($(filter arm%,$(UNAME_P)),)
		CCFLAGS += -D ARM
	endif
endif

ifeq ($(IS_LINUX),1)
	trash_command:=trash
else
	trash_command:=rm
endif

GCC_VERSION:=$(subst ., ,$(shell gcc -dumpversion))

MACHINE:=$(subst -, ,$(shell gcc -dumpmachine))
MACHINE_3:=$(word 3,$($(subst -, ,MACHINE)))

IS_CYGWIN:=$(shell expr $(MACHINE_3) '==' cygwin)

SOURCES := application/main.cpp
OUTDIR:= ./out
BINDIR:= ./bin
OBJDIR:= ./bin/obj
DEPDIR:= ./bin/obj/application
OBJECTS:= $(addprefix $(OBJDIR)/,$(SOURCES:.cpp=.o))
DEPFILES:= $(OBJECTS:.o=.d)
CXX := g++
CXXFLAGS := -c -Wall -Wconversion -Wfatal-errors -Wextra -std=c++11 -MD -MP
CXXTestFLAGS := -g -Wall -Wconversion -Wfatal-errors -Wextra -std=c++11
MACHINE:=$(subst -, ,$(shell gcc -dumpmachine))
MACHINE_3:=$(word 3,$($(subst -, ,MACHINE)))
IS_CYGWIN:=$(shell expr $(MACHINE_3) '==' cygwin)
ADDITIONAL_PATHS= -iquotelibs
ifeq ($(IS_CYGWIN),1)
	ADDITIONAL_PATHS+= -L'C:\cygwin\usr\local\lib\'  -I'C:\cygwin\usr\local\include\' -I'C:\cygwin\usr\include'
else
	ADDITIONAL_PATHS+= # nothing
endif
GCC_VERSION:=$(subst ., ,$(shell gcc -dumpversion))
GCC_MAJOR:=$(word 1,$(GCC_VERSION))
GCC_MINOR:=$(word 2,$(GCC_VERSION))
ifeq ($(shell expr $(GCC_MAJOR).$(GCC_MINOR) '>=' 4.9),1)
    export GCC_COLORS='error=01;31:warning=01;33:note=01;36:caret=01;32:locus=01;32:quote=0'
    CXXFLAGS +=-fdiagnostics-color=always
    CXXTestFLAGS +=-fdiagnostics-color=always
endif
CXXFLAGS_RELEASE := -O3 -s -DNDEBUG  -DARMA_NO_DEBUG
CXXFLAGS_DEBUG := -g

#CXXFLAGS+= --cover
LIBS:= -lboost_filesystem -lboost_system -lcrypto -lrt -lboost_regex -lboost_wave -larmadillo
# -lboost_thread

.PHONY: all clean run test rmresults separator $(MODELS_DEP) modelname autogenerate

# default
all: separator autogenerate $(BINDIR)/sim 
	@echo "done"

modelname:
	@$(CXX) -std=c++11 -Wfatal-errors autogenerator/autogenerator.cpp -DSHOW_MODEL_NAME_ONLY $(LIBS) $(ADDITIONAL_PATHS) -o $(BINDIR)/modelname
	@$(BINDIR)/modelname
	@$(trash_command) $(BINDIR)/modelname

autogenerate:
	@mkdir -p $(BINDIR)
	@$(CXX) -std=c++11 -g -Wall -Wfatal-errors autogenerator/autogenerator.cpp $(LIBS) $(ADDITIONAL_PATHS) -o $(BINDIR)/autogenerate
	@$(BINDIR)/autogenerate
	@$(trash_command) $(BINDIR)/autogenerate

MODEL_%:
	@bash ./scripts/model_callscripts.sh $*

# Link the executable
$(BINDIR)/sim: $(OBJECTS)
	$(CXX) $(CXXFLAGS_DEBUG) $^ -o $@ $(LIBS) $(ADDITIONAL_PATHS)

run:
	$(BINDIR)/sim
 
# Build the test
test:
	$(CXX) $(CXXTestFLAGS) $(CXXFLAGS_DEBUG) test.cpp -o $(BINDIR)/test $(LIBS) $(ADDITIONAL_PATHS)

$(OBJDIR)/%.o: %.cpp
	@if [ ! -d "$(OBJDIR)" ]; then mkdir -p $(OBJDIR) && echo "$(OBJDIR) directory created: $(OBJDIR)";  fi
	@if [ ! -d "$(DEPDIR)" ]; then mkdir -p $(DEPDIR) && echo "$(DEPDIR) directory created: $(DEPDIR)";  fi
	$(CXX) $(CXXFLAGS) $(CXXFLAGS_DEBUG) $(ADDITIONAL_PATHS) -MF $(OBJDIR)/$*.d -o $@  $<

rmresults:
	$(trash_command) $(OUTDIR)/data_201*

clean:
	$(trash_command) $(OBJDIR)/*.o $(OBJDIR)/*.d $(BINDIR)/sim

separator:
	@bash ./scripts/separator.sh

#archive:
#	zip $(USER) *.c *.h readme.txt

-include $(DEPFILES)