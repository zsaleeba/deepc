CXXFLAGS = -g -Wall -D__STDC_LIMIT_MACROS -D__STDC_CONSTANT_MACROS

SRCS=deepc.cpp
OBJS=$(SRCS:%.cpp=%.o)
LIBS=-L/usr/lib/llvm-2.8/lib -lLLVMX86CodeGen -lLLVMSelectionDAG -lLLVMCodeGen -lLLVMJIT -lLLVMTransformUtils -lLLVMInstCombine -lLLVMScalarOpts -lLLVMAnalysis -lLLVMX86Info -lLLVMExecutionEngine -lLLVMTarget -lLLVMSystem -lLLVMCodeGen -lLLVMJIT -lLLVMMC -lLLVMTransformUtils -lLLVMTarget -lLLVMCore -lLLVMSupport -lLLVMSystem -ldl -lpthread

all: minideepc

minideepc: $(OBJS)
	$(CXX) $(CXXFLAGS) -o minideepc $(OBJS) $(LIBS)
