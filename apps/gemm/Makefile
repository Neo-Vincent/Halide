.DEFAULT_GOAL := default
include ../support/Makefile.inc
include ../support/autoscheduler.inc

BIN ?= bin

$(BIN)/gemm.generator: demo_generator.cpp  $(GENERATOR_DEPS)
	@-mkdir -p $(BIN)
	$(CXX) $(CXXFLAGS) -g $(filter-out %.h,$^) -o $@ $(LDFLAGS) $(HALIDE_SYSTEM_LIBS) $(USE_EXPORT_DYNAMIC)  -I /home/neo/Halide/src

clean:
	@echo "clean" 
	rm -rf $(BIN)

default: $(BIN)/gemm.generator ;