#include "headers.h"
#include "camera/camera.h"
#include "shaders/shader.h"
#include "renderer/entityRenderer.h"
#include "game_object/plane.h"
#include "game_object/fence.h"
#include "game_object/snake.h"

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
glm::vec3 lightPos = glm::vec3(-5, 3, -4); // light
int GROUND_WIDTH = 32 * 2, GROUND_HEIGHT = 16 * 2; // ground size
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
bool collisionDetect(vector<Fence>&, Snake& snake);

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
	// --------------------------------
  // create game object
	// 1. plane
  Plane plane;
	// 2. fence
	vector<Fence> fences;
	// set fence's position
  for (int i = 0; i < GROUND_WIDTH / 2; ++i) {
		Fence temp;
		if (i < GROUND_WIDTH / 4) {
			temp.position = glm::vec3(i * 2, 0, 0);
		} else if (i < GROUND_WIDTH / 2) {
			temp.position = glm::vec3((i - GROUND_WIDTH / 4) * 2, 0, GROUND_HEIGHT - 2 * 2);
		}
		fences.push_back(temp);
	}
	for (int i = 0; i < GROUND_HEIGHT / 2 - 2; ++i) {
		Fence temp;
		temp.position = glm::vec3(0, 0, i * 2 + 2);
		fences.push_back(temp);
		Fence temp_r;
		temp_r.position = glm::vec3(GROUND_WIDTH / 2 - 2, 0, i * 2 + 2);
		fences.push_back(temp_r);
	}
	// snake
	Snake snake;
  // --------------------------------
  // create projection
  glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
  while (!glfwWindowShouldClose(window)) {
    // update time and background
    curFrame = glfwGetTime();
    glClearColor(0.3, 0.4, 0.5, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // render plane
		glm::mat4 planeModel = glm::mat4(1.0f);
		planeModel = glm::translate(planeModel, glm::vec3(plx, ply, plz));
    EntityRenderer::renderEntity(
      &planeShader,
      &plane,
      true,
      projection,
      camera.getViewMat(),
      planeModel,
      lightPos,
      camera.position,
      glm::vec3(1, 1, 1)
    );
    // render fence
		for (int i = 0; i < fences.size(); ++i) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, fences[i].position);
			fenceShader.setInt("texture_diffuse_0", 0);
			EntityRenderer::renderEntity(
				&fenceShader,
				&fences[i],
				true,
				projection,
				camera.getViewMat(),
				model,
				lightPos,
				camera.position,
				glm::vec3(0.5, 0.3, 0)
			);
		}
		// render snake
		for (int i = 0; i < snake.getLength(); ++i) {
			glm::vec3 color = glm::vec3(0, 1, 0);
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, snake.bodies[i].position);
			model = glm::rotate(model, glm::radians(snake.bodies[i].bodyDir), glm::vec3(0, 1, 0));
			if (i == 0) {
				color = glm::vec3(1, 0, 1);
				model = glm::scale(model, glm::vec3(1.05, 1.05, 1.05));
			}
			EntityRenderer::renderEntity(
				&snakeShader,
				&snake.bodies[i],
				true,
				projection,
				camera.getViewMat(),
				model, 
				lightPos,
				camera.position,
				color
			);
		}
		// render imgui
		renderImgui(menu);
    // --------------------------------
    // move camera and swap buffer
		bool isCollide = collisionDetect(fences, snake);
		move(curFrame - lastFrame, snake);
		lastFrame = curFrame;
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
	ImGui::SliderFloat("p x", &plx, -100, 100);
	ImGui::SliderFloat("p y", &ply, -100, 100);
	ImGui::SliderFloat("p z", &plz, -100, 100);
	ImGui::SliderFloat("c x", &camera.position.x, -10, 100);
	ImGui::SliderFloat("c y", &camera.position.y, -10, 100);
	ImGui::SliderFloat("c z", &camera.position.z, -10, 100);
	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool collisionDetect(vector<Fence>& fences, Snake& snake) {
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
				newPos.x = GROUND_WIDTH / 2 - 2 - 1.7;
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