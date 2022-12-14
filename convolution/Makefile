CXX86=x86_64-apple-darwin21-g++-12
CXXISPC=ispc --arch=x86_64 --colored-output
CXX=g++
IMAGE=terra.png
CXXFLAGS=-I/usr/local/Cellar/libpng/1.6.39/include -I/usr/local/Cellar/png++/0.2.10/include -L/usr/local/Cellar/libpng/1.6.39/lib -lpng16
all: convolution

convolution: convolution.cpp
	$(CXX86) $(CXXFLAGS) $< -o $@ -fopenmp

run: convolution ../images/$(IMAGE)
	./convolution ../images/$(IMAGE)

clean:
	rm -rf convolution
	rm -rf ../images/outputConv.png