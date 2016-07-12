#include <cstdio>
#include <cstdlib>

#include "..\lib\glew\glew.h"
#include "..\lib\freeglut\freeglut.h"
#include "..\lib\glfw\glfw3.h"
#include "..\lib\glm\glm.hpp"
#include "..\lib\glm\gtc\matrix_transform.hpp"

#include "Shader.hpp"
#include "Camera.hpp"
#include "Cube.hpp"
#include "Cubicle.hpp"
#include "Algorithms.hpp"
#include "texture.hpp"
#include "Common.hpp"

//TODO: this should be done in buildOptions:
//#define RUBIK_DEBUG

GLFWwindow* window;

using namespace glm;

void handleKeyPressed(float dt)
{
	double currentTime = glfwGetTime(); // Read curent time
	static double lastTime = glfwGetTime(); // And compare it with the previous one

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		// Move Camera to the Left
		Camera::moveCamera(Camera::cameraMove::cmLEFT, dt);
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		// Move Camera to the Right
		Camera::moveCamera(Camera::cameraMove::cmRIGHT, dt);
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		// Move Camera Upwards
		Camera::moveCamera(Camera::cameraMove::cmUP, dt);
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		// Move Camera Downwards
		Camera::moveCamera(Camera::cameraMove::cmDOWN, dt);
	}

	if (!RubikAlgorithms::isFrozen()) // We don't want to move anything if the cube is already spinning
	{
		if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_KP_ENTER) == GLFW_PRESS)
		{
			// Solve 1 cubicle
			if (lastTime + timeDelay < currentTime)
			{
				lastTime = currentTime;
				RubikAlgorithms::solveRubik();
			}
		}

		short modifier = 1; // This variable determines which way is a face rotated

		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
			modifier = 2; // Do a double rotation

		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
			modifier = 3; // Do a counter rotation

		if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS)
		{
			// Rotate Left face
			if (lastTime + timeDelay < currentTime)
			{
				lastTime = currentTime;
				RubikAlgorithms::rotateFace(RubikAlgorithms::mLeft, modifier);
			}
		}

		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS)
		{
			// Rotate Right face
			if (lastTime + timeDelay < currentTime)
			{
				lastTime = currentTime;
				RubikAlgorithms::rotateFace(RubikAlgorithms::mRight, modifier);
			}
		}

		if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS)
		{
			//rotate Up face
			if (lastTime + timeDelay < currentTime)
			{
				lastTime = currentTime;
				RubikAlgorithms::rotateFace(RubikAlgorithms::mTop, modifier);
			}
		}

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS)
		{
			// Rotate Down face
			if (lastTime + timeDelay < currentTime)
			{
				lastTime = currentTime;
				RubikAlgorithms::rotateFace(RubikAlgorithms::mBottom, modifier);
			}
		}

		if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_KP_5) == GLFW_PRESS)
		{
			// Rotate Front face
			if (lastTime + timeDelay < currentTime)
			{
				lastTime = currentTime;
				RubikAlgorithms::rotateFace(RubikAlgorithms::mFront, modifier);
			}
		}

		if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_KP_0) == GLFW_PRESS)
		{
			// Rotate Back face
			if (lastTime + timeDelay < currentTime)
			{
				lastTime = currentTime;
				RubikAlgorithms::rotateFace(RubikAlgorithms::mBack, modifier);
			}
		}

	} else { // If Frozen then we just do the next move in the queue
		if (lastTime + timeDelay < currentTime)
		{
			lastTime = currentTime;
			RubikAlgorithms::solveRubik();
		}
	}
}


void runAnimation(float dt)
{
	RubikCube::mainCube.animate(dt);
}

void logicLoop()
{
	static double lastTime = glfwGetTime();
	double currentTime = glfwGetTime();
	float dt = float(currentTime - lastTime);

	handleKeyPressed(dt);
	runAnimation(dt);

	// Update lastTime
	lastTime = currentTime;
}

int main(void)
{

#ifdef RUBIK_DEBUG
	double lastTime = glfwGetTime();
	int nbFrames = 0;
#endif //RUBIK_DEBUG

	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	//window = glfwCreateWindow(1024, 768, "Rubik", NULL, NULL);
	window = glfwCreateWindow(1200, 800, "Rubik", NULL, NULL);
	if (window == NULL){
		fprintf(stderr, "Failed to open GLFW window.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture keys being pressed
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Background Color
	glClearColor(0.4f, 0.4f, 0.4f, 0.0f);

	unsigned int vertexArrayID;
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	unsigned int programID = loadShader("shaders\\ShaderVertex.glsl", "shaders\\ShaderFragment.glsl");
	Common::textShaderID = loadShader("shaders\\TextVertex.glsl", "shaders\\TextFragment.glsl");
	Common::textTexture = loadBitmap("res\\Lucida.bmp");
	Common::textUniformID = glGetUniformLocation(Common::textShaderID, "TextureSampler");
	glGenBuffers(1, &Common::vertexBuffer);
	glGenBuffers(1, &Common::UVBuffer);

	unsigned int ProjectionID = glGetUniformLocation(programID, "Projection");
	unsigned int ViewID = glGetUniformLocation(programID, "View");
	unsigned int ModelID = glGetUniformLocation(programID, "Model");

	Camera::init();
	RubikAlgorithms::freeze(false);
	RubikAlgorithms::toggleHints(true);
	Common::hintText = "Welcome to Rubik!\n";

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	//glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	glm::mat4 Projection = glm::translate(0.4f, 0.0f, 0.0f) * glm::perspective(45.0f, 3.0f / 2.0f, 0.1f, 100.0f);
	// The reason for the translation matrix is so we can add text to the left

	// Cubicle Vertex Buffer
	unsigned int vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(idx_vertex_buffer_data), idx_vertex_buffer_data, GL_STATIC_DRAW);

	// Cubicle Color Buffer
	unsigned int colorBuffer;
	glGenBuffers(1, &colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(idx_color_buffer_data), idx_color_buffer_data, GL_STATIC_DRAW);

	// Cubicle Indexes Buffer to read info from the other buffers
	unsigned int idxBuffer;
	glGenBuffers(1, &idxBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idxBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idx_buffer_data), idx_buffer_data, GL_STATIC_DRAW);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	do{

#ifdef RUBIK_DEBUG
		// Measure speed
		double currentTime = glfwGetTime();
		nbFrames++;
		if (currentTime - lastTime >= 1.0) {
			// printf and reset timer
			printf("%f ms/frame - FPS:%f\n", 1000.0/double(nbFrames), double(nbFrames));
			nbFrames = 0;
			lastTime += 1.0;
		}
#endif //RUBIK_DEBUG

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Set up the shader for drawing the cube
		glUseProgram(programID);

		logicLoop(); // This one does all the user interactions

		// Give the corresponding matrix to the shader
		glUniformMatrix4fv(ProjectionID, 1, GL_FALSE, &Projection[0][0]);
		glUniformMatrix4fv(ViewID, 1, GL_FALSE, Camera::getCamView());

		// We loop for each cubicle, and draw it
		for (short i = 0; i < 27; ++i)
		{
			// Give to the shader the cubicle's position
			glUniformMatrix4fv(ModelID, 1, GL_FALSE, RubikCube::mainCube.getCubicleModel(i));

			// Pass the vertex buffer
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

			// Pass the color buffer
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idxBuffer);

			// Draw the cubicle
			glDrawElements(GL_TRIANGLES, 72, GL_UNSIGNED_SHORT, (void*)0);
			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
		}

		// Now print the text on the screen
		printText2D(Common::hintText.c_str(), -0.95f, 0.9f, 38.0f);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	// We finished the execution, now we delete the buffers
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &Common::vertexBuffer);
	glDeleteBuffers(1, &colorBuffer);
	glDeleteBuffers(1, &Common::UVBuffer);
	glDeleteProgram(programID);
	glDeleteProgram(Common::textShaderID);
	glDeleteTextures(1, &Common::textTexture);
	glDeleteVertexArrays(1, &idxBuffer);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

