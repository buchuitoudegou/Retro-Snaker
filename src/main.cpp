#include "headers.h"
#include "camera/camera.h"
#include "shaders/shader.h"
#include "renderer/entityRenderer.h"
#include "game_object/plane.h"
#include "game_object/fence.h"
#include "game_object/snake.h"
#include "game_object/apple.h"
#include "game_object/obstacle.h"

#include <vector>

using namespace std;

// --------------------------------
// camera
Camera camera(glm::vec3(14.162f, 65.120f, 3.870f));
float xPos = 0.0f;
float yPos = 0.0f;
bool firstMouse = true;
bool autoRotate = false;
bool autoScale = false;
bool autoTranslate = false;
bool keys[1024];
// --------------------------------
// global params
float SCR_WIDTH = 800, SCR_HEIGHT = 800; // window size
GLfloat curFrame = 0.0f, lastFrame = 0.0f; // time
glm::vec3 lightPos = glm::vec3(16, 3, 16); // light
int GROUND_WIDTH = 16 * 2, GROUND_HEIGHT = 16 * 2; // ground size
bool gameover = false; // is dead
int score = 0;
// --------------------------------
// plane position
float plx = 14.451, ply = -0.5, plz = 16.763;

GLFWwindow* openGLallInit();
void mouseCallback(GLFWwindow*, double xpos, double ypos);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void move(GLfloat dtime, Snake&);
void initImgui(GLFWwindow*);
void renderImgui(bool);
bool crossOverDetect(vector<Fence>&, Snake& snake);
bool foodEatenDetect(Apple&, Snake&);
bool obsCollisionDetect(vector<Obstacle>&, Snake& snake);

int main() {
	camera.front = glm::vec3(0.000, -0.973, 0.223);
  GLFWwindow* window = openGLallInit();
	glEnable(GL_DEPTH_TEST);
  if (window == NULL) {
    return -1;
  }
	// --------------------------------
	bool menu = true;
	initImgui(window);
  // --------------------------------
  // create shaders
  Shader planeShader("src/shaders/glsl/shader.vs", "src/shaders/glsl/shader.fs");
  Shader fenceShader("src/shaders/glsl/shader.vs", "src/shaders/glsl/shader.fs");
  Shader snakeShader("src/shaders/glsl/shader.vs", "src/shaders/glsl/shader.fs");
	Shader appleShader("src/shaders/glsl/shader.vs", "src/shaders/glsl/shader.fs");
	Shader obstacleShader("src/shaders/glsl/shader.vs", "src/shaders/glsl/shader.fs");
	// --------------------------------
  // create game object
	// 1. plane
  Plane plane;
	// 2. fence
	vector<Fence> fences;
	// set fence's position
  for (int i = 0; i < GROUND_WIDTH / 2; ++i) {
		Fence temp1, temp2;
		temp1.position = glm::vec3(i * 2, 0, 0);
		temp2.position = glm::vec3(i * 2, 0, GROUND_HEIGHT - 2 * 2);
		fences.push_back(temp1);
		fences.push_back(temp2);
	}
	for (int i = 0; i < GROUND_HEIGHT / 2 - 2; ++i) {
		Fence temp;
		temp.position = glm::vec3(0, 0, i * 2 + 2);
		fences.push_back(temp);
		Fence temp_r;
		temp_r.position = glm::vec3(GROUND_WIDTH - 2, 0, i * 2 + 2);
		fences.push_back(temp_r);
	}
	// 3. snake
	Snake snake;
	// 4. apple
	Apple apple;
	apple.randPosition(GROUND_WIDTH, GROUND_HEIGHT);
	// 5. obstacle
	vector<Obstacle> obstacles;
	for (int i = 0; i < 3; ++i) {
		Obstacle tmp;
		obstacles.push_back(tmp);
	}
	obstacles[0].position = glm::vec3(10, 0, 16);
	obstacles[1].position = glm::vec3(20, 0, 8);
	obstacles[2].position = glm::vec3(16, 0, 18);
  // --------------------------------
  // create projection
  glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
  while (!glfwWindowShouldClose(window)) {
    // update time and background
    curFrame = glfwGetTime();
    glClearColor(0.3, 0.4, 0.5, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// ------------------------------
		// render object
    // 1. render plane
		planeShader.setInt("texture_diffuse_0", 0);
		glm::mat4 planeModel = glm::mat4(1.0f);
		planeModel = glm::translate(planeModel, glm::vec3(plx, ply, plz));
    EntityRenderer::renderEntity(
      &planeShader,
      &plane,
      false,
      projection,
      camera.getViewMat(),
      planeModel,
      lightPos,
      camera.position,
      glm::vec3(1, 1, 1)
    );
    // 2. render fence
		for (int i = 0; i < fences.size(); ++i) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, fences[i].position);
			fenceShader.setInt("texture_diffuse_0", 0);
			EntityRenderer::renderEntity(
				&fenceShader,
				&fences[i],
				false,
				projection,
				camera.getViewMat(),
				model,
				lightPos,
				camera.position,
				glm::vec3(0.5, 0.3, 0)
			);
		}
		// 3. render apple
		glm::mat4 appleModel = glm::mat4(1.0f);
		appleModel = glm::translate(appleModel, apple.position);
		appleModel = glm::scale(appleModel, glm::vec3(0.015, 0.015, 0.015));
		EntityRenderer::renderEntity(
				&appleShader,
				&apple,
				false,
				projection,
				camera.getViewMat(),
				appleModel, 
				lightPos,
				camera.position,
				glm::vec3(0, 0, 0)
		);
		// 4. render snake
		for (int i = 0; i < snake.getLength(); ++i) {
			bool useVertColor = false;
			glm::vec3 color = glm::vec3(0, 1, 0);
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, snake.bodies[i].position);
			model = glm::rotate(model, glm::radians(snake.bodies[i].bodyDir), glm::vec3(0, 1, 0));
			if (i == 0) {
				useVertColor = true;
				color = glm::vec3(143.0 / 256.0, 188.0 / 256.0, 143.0 / 256.0);
				model = glm::scale(model, glm::vec3(1.05, 1.05, 1.05));
				if (gameover) {
					color = glm::vec3(1.0f, 0, 0);
				}
			}
			EntityRenderer::renderEntity(
				&snakeShader,
				&snake.bodies[i],
				useVertColor,
				projection,
				camera.getViewMat(),
				model, 
				lightPos,
				camera.position,
				color
			);
		}
		// 5. render obstacle
		for (int i = 0; i < obstacles.size(); ++i) {
			glm::mat4 obsModel = glm::mat4(1.0f);
			obsModel = glm::translate(obsModel, obstacles[i].position);
			EntityRenderer::renderEntity(
				&obstacleShader,
				&obstacles[i],
				false,
				projection,
				camera.getViewMat(),
				obsModel,
				lightPos,
				camera.position,
				glm::vec3(0, 0, 0)
			);
		}
		// 6. render imgui
		renderImgui(menu);
		// --------------------------------
		// collide with obstacle
		bool isDead = obsCollisionDetect(obstacles, snake);	
		if (isDead) {
			gameover = true;
		}
		// --------------------------------
		// food eaten
		bool isEaten = foodEatenDetect(apple, snake);
		if (isEaten) {
			score ++;
			snake.lengthen();
		}
		// --------------------------------
		// cross over
		bool isCross = crossOverDetect(fences, snake);
    // --------------------------------
    // move camera and swap buffer
		if (!gameover) {
			move(curFrame - lastFrame, snake);
			lastFrame = curFrame;
		}
		glfwMakeContextCurrent(window);
		glfwSwapBuffers(window);
		glfwPollEvents();
  }
  return 0;
}

GLFWwindow* openGLallInit() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Retro Snake", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return NULL;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return NULL;
	}

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	// glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetKeyCallback(window, keyCallback);
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	return window;
}
void mouseCallback(GLFWwindow*, double xpos, double ypos) {
	if (firstMouse) {
		xPos = xpos;
		yPos = ypos;
		firstMouse = false;
	}
	float xoffset = xpos - xPos;
	float yoffset = yPos - ypos;
	camera.mouseMoveHandler(-xoffset, -yoffset);
	xPos = xpos;
	yPos = ypos;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_J && action == GLFW_PRESS) {
		autoRotate = !autoRotate;
	}
	if (key == GLFW_KEY_K && action == GLFW_PRESS) {
		autoTranslate = !autoTranslate;
	}
	if (key == GLFW_KEY_L && action == GLFW_PRESS) {
		autoScale = !autoScale;
	}
	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS) {
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	SCR_WIDTH = width;
	SCR_HEIGHT = height;
	glViewport(0, 0, width, height);
}

void move(GLfloat dtime, Snake& snake) {
	// snake.move(dtime);
	if (keys[GLFW_KEY_W]) {
		// camera.keyboardHandler(FORWARD, dtime);
		snake.move(dtime);
	}
	if (keys[GLFW_KEY_S]) {
		// camera.keyboardHandler(BACKWARD, dtime);
	}
	if (keys[GLFW_KEY_A]) {
		// camera.keyboardHandler(LEFT, dtime);
		snake.turn(Snake::turnAngle);
	}
	if (keys[GLFW_KEY_D]) {
		// camera.keyboardHandler(RIGHT, dtime);
		snake.turn(-Snake::turnAngle);
	}
}

void initImgui(GLFWwindow* window) {
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	ImGui::StyleColorsClassic();
}

void renderImgui(bool menu) {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("Menu", &menu, ImGuiWindowFlags_MenuBar);
	// ImGui::SliderFloat("p x", &plx, -100, 100);
	// ImGui::SliderFloat("p y", &ply, -100, 100);
	// ImGui::SliderFloat("p z", &plz, -100, 100);
	// ImGui::SliderFloat("c x", &camera.position.x, -10, 100);
	// ImGui::SliderFloat("c y", &camera.position.y, -10, 100);
	// ImGui::SliderFloat("c z", &camera.position.z, -10, 100);
	if (!gameover) {
		ImGui::Value("Your Score", score);
		if (score > 6) {
			ImGui::Text("Your snake has got the longest body!");
		}
	} else {
		ImGui::Text("You lose!");
	}
	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool crossOverDetect(vector<Fence>& fences, Snake& snake) {
	for (int i = 0; i < fences.size(); ++i) {
		Fence curFence = fences[i];
		float dist = glm::distance2(snake.bodies[0].position, curFence.position);
		if (dist < 2) {
			glm::vec3 newPos = curFence.position;
			if (newPos.z == 0) {
				// bottom boundary
				newPos.z = GROUND_HEIGHT - 2 * 2 - 1.7;
			} else if (newPos.z == GROUND_HEIGHT - 2 * 2) {
				// top boundary
				newPos.z = 0 + 1.7;
			} else if (newPos.x == 0) {
				// left boundary
				newPos.x = GROUND_WIDTH - 2 - 1.7;
			} else {
				// right boundary
				newPos.x = 0 + 1.7;
			}
			snake.moveTo(newPos);
			return true;
		}
	}
	return false;
}

bool foodEatenDetect(Apple& apple, Snake& snake) {
	if (glm::distance2(apple.position, snake.bodies[0].position) < 2) {
		apple.randPosition(GROUND_WIDTH, GROUND_HEIGHT);
		while (glm::distance2(apple.position, glm::vec3(10, 0, 16)) < 2 ||
			glm::distance2(apple.position, glm::vec3(20, 0, 8)) < 2 ||
			glm::distance2(apple.position, glm::vec3(16, 0, 18)) < 2) {
			apple.randPosition(GROUND_WIDTH, GROUND_HEIGHT);				
		}
		return true;
	}
	return false;
}

bool obsCollisionDetect(vector<Obstacle>& obstacles, Snake& snake) {
	for (int i = 0; i < obstacles.size(); ++i) {
		if (glm::distance2(obstacles[i].position, snake.bodies[0].position) < 2) {
			return true;
		}
	}
	return false;
}