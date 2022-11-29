CPPFLAGS=-I/usr/local/Cellar/libpng/1.6.38/include -I/usr/local/Cellar/png++/0.2.10/include -L/usr/local/Cellar/libpng/1.6.38/lib -lpng16

all: rays run

rays: raytracer-sequential.cpp
	x86_64-apple-darwin21-g++-12 $(CPPFLAGS) raytracer-sequential.cpp -o raytrace -fopenmp

compare: imageCompare.cpp
	x86_64-apple-darwin21-g++-12 $(CPPFLAGS) imageCompare.cpp -o imageCompare

convolution: convolution.cpp
	x86_64-apple-darwin21-g++-12 $(CPPFLAGS) convolution.cpp -o convolution 

run: raytrace
	./raytrace bwbw.png bwbwTrace.png lights.txt


clean:
	rm -rf ./raytrace
	rm -rf ./imageCompare
	rm -rf ./convolution

