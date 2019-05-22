bin = bin
build = build
src = src

CFLAGS = -std=c++11 -g -w
target := $(bin)/main

objects := $(build)/camera.o $(build)/plane.o $(build)/entity.o $(build)/entityRenderer.o \
$(build)/shader.o $(build)/main.o $(build)/glad.o

$(target) : $(objects)
	g++ $(objects) -lglfw $(CFLAGS) -o $@

# glad
$(build)/glad.o : $(src)/glad.c
	g++ $< -o $@ $(CFLAGS) -c

# main
$(build)/main.o : $(src)/main.cpp
	g++ $< -o $@ $(CFLAGS) -c

# camera
$(build)/camera.o : $(src)/camera/camera.cpp $(src)/camera/camera.h
	g++ $< -o $@ $(CFLAGS) -c

# ---------------------------
# game_object               |
# ---------------------------
# entity
$(build)/entity.o : $(src)/game_object/entity.cpp $(src)/game_object/entity.h
	g++ $< -o $@ $(CFLAGS) -c
# plane
$(build)/plane.o : $(src)/game_object/plane.cpp $(src)/game_object/plane.h
	g++ $< -o $@ $(CFLAGS) -c

# ---------------------------
# renderer                  |
# ---------------------------
# entity renderer
$(build)/entityRenderer.o : $(src)/renderer/entityRenderer.cpp $(src)/renderer/entityRenderer.h
	g++ $< -o $@ $(CFLAGS) -c

# ---------------------------
# shaders                   |
# ---------------------------
# shader
$(build)/shader.o : $(src)/shaders/shader.cpp $(src)/shaders/shader.h
	g++ $< -o $@ $(CFLAGS) -c

run:
	./bin/main

clean:
	rm -rf bin/
	rm -rf build/
	mkdir bin/
	mkdir build/