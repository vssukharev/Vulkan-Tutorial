
CFLAGS = -std=c++23 -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread

.PHONY: run clean debug

shaders: 09_shader_base.vert 09_shader_base.frag
	glslc 09_shader_base.vert -o vert.spv
	glslc 09_shader_base.frag -o frag.spv

test: main.cpp shaders
	g++ $(CFLAGS) -o ./test ./main.cpp $(LDFLAGS)

debug: main.cpp shaders
	g++ $(CFLAGS) -g -o ./test ./main.cpp $(LDFLAGS)

run: test
	./test

clean: 
	rm -f ./test

