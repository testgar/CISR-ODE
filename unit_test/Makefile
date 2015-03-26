BINDIR:= ./bin
CXX := g++
CXXFLAGS := -g -Wall -Wconversion -Wfatal-errors -Wextra -std=c++11
LIBS:= -lboost_filesystem -lboost_system -lcrypto
UNIT1:= dp5_lorenz-attractor-50000

.PHONY: all  clean

# default
all:
	@echo "cleaning..."
	trash $(BINDIR)
	mkdir -p $(BINDIR)	
	@echo "generating unit-tests..."
	$(CXX) $(CXXFLAGS) $(UNIT1).cpp -o $(BINDIR)/$(UNIT1) $(LIBS)
	@echo "runing unit-tests..."
	@$(BINDIR)/$(UNIT1)
	@echo "done"

clean:
	trash $(BINDIR)/*

