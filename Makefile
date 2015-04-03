MODELS:=$(shell cat models)
SOURCES := application/main.cpp
OUTDIR:= ./out
BINDIR:= ./bin
OBJDIR:= ./bin/obj
DEPDIR:= ./bin/obj/application
OBJECTS:= $(addprefix $(OBJDIR)/,$(SOURCES:.cpp=.o))
DEPFILES:= $(OBJECTS:.o=.d)
CXX := g++
CXXFLAGS := -c -Wall -Wconversion -Wfatal-errors -Wextra -std=c++11 -MD -MP
CXXFLAGS_RELEASE := -O3 -s -DNDEBUG  -DARMA_NO_DEBUG
CXXFLAGS_DEBUG := -g

CXXTestFLAGS := -g -Wall -Wconversion -Wfatal-errors -Wextra -std=c++11
#CXXFLAGS+= --cover
LIBS:= -lboost_filesystem -lboost_system -lcrypto -lrt
# -lboost_thread -larmadillo

.PHONY: all clean run rmresults separator $(MODELS_DEP) embed_models

# default
all: separator embed_models $(MODELS:%=MODEL_%) $(BINDIR)/sim 
	@echo "done"

MODEL_%:
	@bash ./scripts/model_callscripts.sh $*

# Link the executable
$(BINDIR)/sim: $(OBJECTS)
	$(CXX) $(LDFLAGS) $^ -o $@ $(LIBS)

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

embed_models:
	@bash ./scripts/embed_models.sh

#archive:
#	zip $(USER) *.c *.h readme.txt

-include $(DEPFILES)