CPPFLAGS=-I/usr/local/Cellar/libpng/1.6.38/include -I/usr/local/Cellar/png++/0.2.10/include -L/usr/local/Cellar/libpng/1.6.38/lib -lpng16

all: rays run

rays: testRay.cpp
	x86_64-apple-darwin21-g++-12 $(CPPFLAGS) testRay.cpp -o raytrace

run: raytrace
	./raytrace bwbw.png bwbwTrace.png  

clean:
	rm -rf ./raytrace

