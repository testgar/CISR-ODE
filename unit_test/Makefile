BINDIR:= ./bin
OUTDIR:= ./out
CXX := g++
CXXFLAGS := -Wall -Wconversion -Wfatal-errors -Wextra -std=c++11
CXXFLAGS_RELEASE := -O3 -s -DNDEBUG  -DARMA_NO_DEBUG
CXXFLAGS_DEBUG := -g --coverage
LIBS:= -lboost_filesystem -lboost_system -lcrypto -lrt
UNIT1:= dp5_lorenz-attractor-50000

.PHONY: all  clean

# default
all:
	@echo "cleaning..."
	trash $(BINDIR) $(OUTDIR)
	mkdir -p $(BINDIR) $(OUTDIR)
	@echo "generating unit-tests..."
	$(CXX) $(CXXFLAGS) $(CXXFLAGS_DEBUG) $(UNIT1).cpp -o $(BINDIR)/$(UNIT1) $(LIBS)
	@echo "runing unit-tests..."
	@$(BINDIR)/$(UNIT1)
	@echo "done"

release:
	@echo "cleaning..."
	trash $(BINDIR) $(OUTDIR)
	mkdir -p $(BINDIR) $(OUTDIR)
	@echo "generating unit-tests..."
	$(CXX) $(CXXFLAGS) $(CXXFLAGS_RELEASE) $(UNIT1).cpp -o $(BINDIR)/$(UNIT1) $(LIBS)
	@echo "runing unit-tests..."
	@$(BINDIR)/$(UNIT1)
	@echo "done"

clean:
	trash $(BINDIR)/*

