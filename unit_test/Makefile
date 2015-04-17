BINDIR:= ./bin
OUTDIR:= ./out
CXX := g++
CXXFLAGS := -Wall -Wconversion -Wfatal-errors -Wextra -std=c++11
CXXFLAGS_RELEASE := -O3 -s -DNDEBUG  -DARMA_NO_DEBUG
CXXFLAGS_DEBUG := -g 
#--coverage
CXXFLAGS_BUILD=$(CXXFLAGS_RELEASE)
LIBS:= -lboost_filesystem -lboost_system -lcrypto -lrt
UNIT1:= test_dp5_lorenz-attractor-50000
UNIT2:= test_ck54_lorenz-attractor-50000
UNIT3:= test_dp5_compare_v1
UNIT4:= test_ck54_compare_v1

.PHONY: all release buildonly runonly clean

all: buildonly runonly

release: buildonly runonly

buildonly:
	@echo "cleaning..."
	trash $(BINDIR) $(OUTDIR)
	mkdir -p $(BINDIR) $(OUTDIR)
	@echo "generating unit-tests..."
	$(CXX) $(CXXFLAGS) $(CXXFLAGS_BUILD) $(UNIT1).cpp -o $(BINDIR)/$(UNIT1) $(LIBS)
	$(CXX) $(CXXFLAGS) $(CXXFLAGS_BUILD) $(UNIT2).cpp -o $(BINDIR)/$(UNIT2) $(LIBS)
	$(CXX) $(CXXFLAGS) $(CXXFLAGS_BUILD) $(UNIT3).cpp -o $(BINDIR)/$(UNIT3) $(LIBS)
	$(CXX) $(CXXFLAGS) $(CXXFLAGS_BUILD) $(UNIT4).cpp -o $(BINDIR)/$(UNIT4) $(LIBS)

runonly:
	@bash ../scripts/separator.sh
	@echo "runing unit-tests..."
	@$(BINDIR)/$(UNIT1)
	@$(BINDIR)/$(UNIT2)
	@$(BINDIR)/$(UNIT3)
	@$(BINDIR)/$(UNIT4)
	@echo "done"

clean:
	trash $(BINDIR)/*
