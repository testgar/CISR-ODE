BINDIR:= ./bin
OUTDIR:= ./out
CXX := g++
CXXFLAGS := -Wall -Wconversion -Wfatal-errors -Wextra -std=c++11
GCC_VERSION:=$(subst ., ,$(shell gcc -dumpversion))
GCC_MAJOR:=$(word 1,$(GCC_VERSION))
GCC_MINOR:=$(word 2,$(GCC_VERSION))
ifeq ($(shell expr $(GCC_MAJOR).$(GCC_MINOR) '>=' 4.9),1)
    export GCC_COLORS='error=01;31:warning=01;33:note=01;36:caret=01;32:locus=01;32:quote=0'
    CXXFLAGS +=-fdiagnostics-color=always
endif
CXXFLAGS_RELEASE := -O3 -s -DNDEBUG  -DARMA_NO_DEBUG
CXXFLAGS_DEBUG := -g 
#--coverage
CXXFLAGS_BUILD=$(CXXFLAGS_RELEASE)
LIBS:= -lboost_filesystem -lboost_system -lcrypto -lrt
UNIT1:= test_dp5_compare_odeint
UNIT2:= test_ck54_compare_odeint
UNIT3:= test_f78_compare_odeint
UNIT4:= test_dp5_compare_v1
UNIT5:= test_ck54_compare_v1
UNIT6:= test_f78_compare_v1

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
	$(CXX) $(CXXFLAGS) $(CXXFLAGS_BUILD) $(UNIT5).cpp -o $(BINDIR)/$(UNIT5) $(LIBS)
	$(CXX) $(CXXFLAGS) $(CXXFLAGS_BUILD) $(UNIT6).cpp -o $(BINDIR)/$(UNIT6) $(LIBS)

runonly:
	@bash ../scripts/separator.sh
	@echo "runing unit-tests..."
	@$(BINDIR)/$(UNIT1)
	@$(BINDIR)/$(UNIT2)
	@$(BINDIR)/$(UNIT3)
	@$(BINDIR)/$(UNIT4)
	@$(BINDIR)/$(UNIT5)
	@$(BINDIR)/$(UNIT6)
	@echo "done"

clean:
	trash $(BINDIR)/*
