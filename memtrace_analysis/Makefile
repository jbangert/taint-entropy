CXXFLAGS+=-std=c++11 -lcrypto -O0 -ggdb -lprotobuf
BINARIES=print disas
all: $(BINARIES)
%.pb.cc:%.proto
	protoc --cpp_out=. $<
print: CXXFLAGS += -lpb2json
print: print.cpp trace.pb.cc
disas: CXXFLAGS += -ldistorm3
disas: disas.cpp trace.pb.cc

clean:
	rm $(BINARIES)
