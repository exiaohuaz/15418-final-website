CPPFLAGS=-I/usr/local/Cellar/libpng/1.6.39/include -I/usr/local/Cellar/png++/0.2.10/include -L/usr/local/Cellar/libpng/1.6.39/lib -lpng16
IMAGEDIR=../images
IMAGE=terra.png

all: rays compare

rays: rays-seq rays-par

rays-seq: raytracer-sequential.cpp
	x86_64-apple-darwin21-g++-12 $(CPPFLAGS) raytracer-sequential.cpp -o raytrace-sequential

rays-par: raytracer-parallel.cpp
	x86_64-apple-darwin21-g++-12 $(CPPFLAGS) raytracer-parallel.cpp -o raytrace-parallel -fopenmp

# This will only work on machine with Nvida GPU
rays-cuda: raytracer.cu
	nvcc raytracer.cu `libpng-config --ldflags` -o raytracer-cuda

compare: imageCompare.cpp
	x86_64-apple-darwin21-g++-12 $(CPPFLAGS) imageCompare.cpp -o imageCompare

run: raytrace-sequential raytrace-parallel imageCompare
	./raytrace-sequential $(IMAGEDIR)/$(IMAGE) $(IMAGEDIR)/outputConv.png lights.txt $(IMAGEDIR)/outputSeq.png
	./raytrace-parallel $(IMAGEDIR)/$(IMAGE) $(IMAGEDIR)/outputConv.png lights.txt $(IMAGEDIR)/outputPar.png
	./imageCompare $(IMAGEDIR)/outputSeq.png $(IMAGEDIR)/outputPar.png


clean:
	rm -rf raytrace-sequential raytrace-parallel raytracer-cuda imageCompare 
	rm -rf $(IMAGEDIR)/outputCuda.png $(IMAGEDIR)/outputSeq.png $(IMAGEDIR)/outputPar.png $(IMAGEDIR)/outputConv.png 
	rm -rf lights.txt

