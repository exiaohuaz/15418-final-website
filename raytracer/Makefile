CPPFLAGS=-I/usr/local/Cellar/libpng/1.6.38/include -I/usr/local/Cellar/png++/0.2.10/include -L/usr/local/Cellar/libpng/1.6.38/lib -lpng16
IMAGEDIR=../images

all: rays compare run

rays: rays-seq rays-par

rays-seq: raytracer-sequential.cpp
	x86_64-apple-darwin21-g++-12 $(CPPFLAGS) raytracer-sequential.cpp -o raytrace-sequential

rays-par: raytracer-parallel.cpp
	x86_64-apple-darwin21-g++-12 $(CPPFLAGS) raytracer-parallel.cpp -o raytrace-parallel -fopenmp

compare: imageCompare.cpp
	x86_64-apple-darwin21-g++-12 $(CPPFLAGS) imageCompare.cpp -o imageCompare

run: raytrace-sequential raytrace-parallel imageCompare
	./raytrace-sequential $(IMAGEDIR)/bwbw.png $(IMAGEDIR)/bwbwTrace.png lights.txt $(IMAGEDIR)/outputSeq.png
	./raytrace-parallel $(IMAGEDIR)/bwbw.png $(IMAGEDIR)/bwbwTrace.png lights.txt $(IMAGEDIR)/outputPar.png
	./imageCompare $(IMAGEDIR)/outputSeq.png $(IMAGEDIR)/outputPar.png


clean:
	rm -rf raytrace-sequential raytrace-parallel imageCompare $(IMAGEDIR)/outputSeq.png $(IMAGEDIR)/outputPar.png

