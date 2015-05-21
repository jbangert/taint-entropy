CXXFLAGS+=-std=c++11 -lcrypto -O0 -ggdb -lprotobuf -lpb2json
BINARIES=average_length print find_blocks lengths
all: $(BINARIES)
%.pb.cc:%.proto
	protoc --cpp_out=. $<
print: print.cpp trace.pb.cc
average_length: average_length.cpp
clean:
	rm $(BINARIES)
