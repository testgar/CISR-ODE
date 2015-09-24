SOURCES := application/main.cpp
OUTDIR:= ./out
BINDIR:= ./bin
OBJDIR:= ./bin/obj
DEPDIR:= ./bin/obj/application
OBJECTS:= $(addprefix $(OBJDIR)/,$(SOURCES:.cpp=.o))
DEPFILES:= $(OBJECTS:.o=.d)
CXX := g++
CXXFLAGS := -c -Wall -Wconversion -Wfatal-errors -Wextra -std=c++11 -MD -MP
GCC_VERSION:=$(subst ., ,$(shell gcc -dumpversion))
GCC_MAJOR:=$(word 1,$(GCC_VERSION))
GCC_MINOR:=$(word 2,$(GCC_VERSION))
ifeq ($(shell expr $(GCC_MAJOR).$(GCC_MINOR) '>=' 4.9),1)
    export GCC_COLORS='error=01;31:warning=01;33:note=01;36:caret=01;32:locus=01;32:quote=0'
    CXXFLAGS +=-fdiagnostics-color=always
endif
CXXFLAGS_RELEASE := -O3 -s -DNDEBUG  -DARMA_NO_DEBUG
CXXFLAGS_DEBUG := -g

CXXTestFLAGS := -g -Wall -Wconversion -Wfatal-errors -Wextra -std=c++11
#CXXFLAGS+= --cover
LIBS:= -lboost_filesystem -lboost_system -lcrypto -lrt
# -lboost_thread -larmadillo

.PHONY: all clean run rmresults separator $(MODELS_DEP) modelname autogenerate

# default
all: separator autogenerate $(BINDIR)/sim 
	@echo "done"

modelname:
	@g++ -std=c++11 -Wfatal-errors autogenerator/autogenerator.cpp -DSHOW_MODEL_NAME_ONLY $(LIBS) -o $(BINDIR)/modelname
	@$(BINDIR)/modelname
	@trash $(BINDIR)/modelname

autogenerate:
	@g++ -std=c++11 -g -Wall -Wfatal-errors autogenerator/autogenerator.cpp $(LIBS) -o $(BINDIR)/autogenerate
	@$(BINDIR)/autogenerate
	@trash $(BINDIR)/autogenerate


MODEL_%:
	@bash ./scripts/model_callscripts.sh $*

# Link the executable
$(BINDIR)/sim: $(OBJECTS)
	$(CXX) $(CXXFLAGS_DEBUG) $^ -o $@ $(LIBS)

run:
	$(BINDIR)/sim
 
# Build the test
test:
	$(CXX) $(CXXTestFLAGS) $(CXXFLAGS_DEBUG) test.cpp -o $(BINDIR)/test $(LIBS)

$(OBJDIR)/%.o: %.cpp
	@if [ ! -d "$(OBJDIR)" ]; then mkdir -p $(OBJDIR) && echo "$(OBJDIR) directory created: $(OBJDIR)";  fi
	@if [ ! -d "$(DEPDIR)" ]; then mkdir -p $(DEPDIR) && echo "$(DEPDIR) directory created: $(DEPDIR)";  fi
	$(CXX) $(CXXFLAGS) $(CXXFLAGS_DEBUG) -MF $(OBJDIR)/$*.d -o $@  $<

rmresults:
	trash $(OUTDIR)/data_201*

clean:
	trash $(OBJDIR)/*.o $(OBJDIR)/*.d $(BINDIR)/sim

separator:
	@bash ./scripts/separator.sh

#archive:
#	zip $(USER) *.c *.h readme.txt

-include $(DEPFILES)