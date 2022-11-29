CXX86=x86_64-apple-darwin21-g++-12
CXXISPC=ispc --arch=x86_64 --colored-output
CXX=g++
CXXFLAGS=-I/usr/local/Cellar/libpng/1.6.38/include -I/usr/local/Cellar/png++/0.2.10/include 
all: convolution

convolution: convolution.cpp
	$(CXXISPC) --ignore-preprocessor-errors $< $(CXXFLAGS) -o $@
	chmod 777 $@

clean:
	rm -rf convolution