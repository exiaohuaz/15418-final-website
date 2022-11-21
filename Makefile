all: testRay.cpp
	g++ -lpng16 testRay.cpp -o raytrace

clean:
	rm -rf ./raytrace