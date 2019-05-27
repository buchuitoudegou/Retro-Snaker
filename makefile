bin = bin
build = build
src = src

CFLAGS = -std=c++11 -g -w
target := $(bin)/main

objects := $(build)/camera.o $(build)/plane.o $(build)/entity.o $(build)/entityRenderer.o \
$(build)/shader.o $(build)/main.o $(build)/glad.o $(build)/fence.o \
$(build)/imgui.o $(build)/imgui_draw.o $(build)/imgui_impl_glfw.o \
$(build)/imgui_impl_opengl3.o $(build)/imgui_widgets.o \
$(build)/snakeBody.o $(build)/snake.o $(build)/stb_image.o\
$(build)/mesh.o $(build)/loader.o $(build)/apple.o\
$(build)/obstacle.o

$(target) : $(objects)
	g++ $(objects) -lglfw -lassimp $(CFLAGS) -o $@

# stb image
$(build)/stb_image.o : $(src)/stb_image/stb_image.cpp
	g++ $< -o $@ $(CFLAGS) -c

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
# fence
$(build)/fence.o : $(src)/game_object/fence.cpp $(src)/game_object/fence.h
	g++ $< -o $@ $(CFLAGS) -c
# snake body
$(build)/snakeBody.o : $(src)/game_object/snakeBody.cpp $(src)/game_object/snakeBody.h
	g++ $< -o $@ $(CFLAGS) -c
# snake
$(build)/snake.o : $(src)/game_object/snake.cpp $(src)/game_object/snake.h
	g++ $< -o $@ $(CFLAGS) -c
# apple
$(build)/apple.o : $(src)/game_object/apple.cpp $(src)/game_object/apple.h
	g++ $< -o $@ $(CFLAGS) -c
# obstacle
$(build)/obstacle.o : $(src)/game_object/obstacle.cpp $(src)/game_object/obstacle.h
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

# ---------------------------
# model loader              |
# ---------------------------
# mesh
$(build)/mesh.o : $(src)/mesh/mesh.cpp $(src)/mesh/mesh.h
	g++ $< -o $@ $(CFLAGS) -c

# model loader
$(build)/loader.o : $(src)/model_loader/loader.cpp $(src)/model_loader/loader.h
	g++ $< -o $@ $(CFLAGS) -c

# build imgui
$(build)/%.o : $(src)/imgui/%.cpp
	g++ -c -o $@ $< $(CFLAGS)

run:
	./bin/main

clean:
	rm -rf bin/
	rm -rf build/
	mkdir bin/
	mkdir build/