#include "headers.h"
#include "camera/camera.h"
#include "shaders/shader.h"
#include "renderer/entityRenderer.h"
#include "game_object/plane.h"

// --------------------------------
// camera
Camera camera(glm::vec3(0.0f, 0.0f, 7.0f));
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
glm::vec3 lightPos = glm::vec3(-5, 3, -4);

GLFWwindow* openGLallInit();
void mouseCallback(GLFWwindow*, double xpos, double ypos);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void move(GLfloat dtime);


int main() {
  GLFWwindow* window = openGLallInit();
  if (window == NULL) {
    return -1;
  }
  // --------------------------------
  // create shaders
  Shader planeShader("src/shaders/glsl/shader.vs", "src/shaders/glsl/shader.fs");
  // --------------------------------
  // create game object
  Plane plane;
  // --------------------------------
  // create projection
  glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
  while (!glfwWindowShouldClose(window)) {
    // update time and background
    curFrame = glfwGetTime();
    glClearColor(1, 1, 1, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // render plane
    EntityRenderer::renderEntity(
      &planeShader,
      &plane,
      true,
      projection,
      camera.getViewMat(),
      glm::mat4(1.0f),
      lightPos,
      camera.position,
      glm::vec3(1, 1, 1)
    );

    // --------------------------------
    // move camera and swap buffer
    move(curFrame - lastFrame);
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

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "firWindow", NULL, NULL);
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
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetKeyCallback(window, keyCallback);
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
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	SCR_WIDTH = width;
	SCR_HEIGHT = height;
	glViewport(0, 0, width, height);
}

void move(GLfloat dtime) {
	if (keys[GLFW_KEY_W]) {
		camera.keyboardHandler(FORWARD, dtime);
	}
	if (keys[GLFW_KEY_S]) {
		camera.keyboardHandler(BACKWARD, dtime);
	}
	if (keys[GLFW_KEY_A]) {
		camera.keyboardHandler(LEFT, dtime);
	}
	if (keys[GLFW_KEY_D]) {
		camera.keyboardHandler(RIGHT, dtime);
	}
}