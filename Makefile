all:
	g++ src/main.cpp src/edges.cpp src/sharpen.cpp `pkg-config opencv --libs` -std=c++11 -o DumbEdgeFinder