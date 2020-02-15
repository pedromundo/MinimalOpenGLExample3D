all:
	g++ --std=c++14 -Ofast *.cpp -lGLU -lm -lglut -lGLEW -lgl -o OGL3D -I/opt/local/include -L/opt/local/lib
