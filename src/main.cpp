#include "helper.h"

using namespace std;
class Canvas {
	Position p[3];
	unsigned int va, vb;
public:
	Canvas() {
		p[0] = { -1, -1, 0, 1 };
		p[1] = { 10,  -1, 0, 1 };
		p[2] = { -1,  10, 0, 1 };
		
		glGenVertexArrays(1, &va);
		glGenBuffers(1, &vb);

		glBindVertexArray(va);
		glBindBuffer(GL_ARRAY_BUFFER, vb);
		glBufferData(GL_ARRAY_BUFFER, sizeof(p), &p, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Position), 0);
		glEnableVertexAttribArray(0);
	
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void draw() {
		glBindVertexArray(va);
		glBindBuffer(GL_ARRAY_BUFFER, vb);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
};

pair<float, float> pos{ -0.5, 0 };
float scale = 3;

void process_input(GLFWwindow* win, unsigned int posUnif, unsigned int scaleUnif, unsigned int iterationUnif) {
	
	
	if (glfwGetKey(win, GLFW_KEY_Q) && scale < 3) {
		scale *= 1.01;

	}
	if (glfwGetKey(win, GLFW_KEY_E)) {
		scale *= 0.99;
	}
	if (glfwGetKey(win, GLFW_KEY_W) && pos.second < 1) {
		pos.second += 0.001 * scale;
	}
	if (glfwGetKey(win, GLFW_KEY_S) && pos.second > -1) {
		pos.second -= 0.001 * scale;
	}
	if (glfwGetKey(win, GLFW_KEY_D) && pos. first < 1) {
		pos.first += 0.001 * scale;
	}
	if (glfwGetKey(win, GLFW_KEY_A) && pos.first > -2) {
		pos.first -= 0.001 * scale;
	}

	if (glfwGetKey(win, GLFW_KEY_UP)) {
		ITER += 1;
	}
	if (glfwGetKey(win, GLFW_KEY_DOWN) && ITER > 25) {
		ITER -= 1;
	}


	glUniform2f(posUnif, pos.first, pos.second);
	glUniform2f(scaleUnif, scale * aspect, scale);
	glUniform1i(iterationUnif, ITER);
}

int main() {
	GLFWwindow* window = window_init();
	load_OpenGL();
	unsigned int program = create_and_use_shaders(vs, fs);

	unsigned int resolutionUnif = glGetUniformLocation(program, "iResolution");
	unsigned int iterUnif = glGetUniformLocation(program, "iterations");
	unsigned int posUnif = glGetUniformLocation(program, "pos");
	unsigned int scaleUnif = glGetUniformLocation(program, "scale");

	glUniform2f(resolutionUnif, WIDTH, HEIGHT);
	glUniform1i(iterUnif, ITER);

	double t1 = 0;
	double t2 = glfwGetTime();
	glClearColor(0, 0, 0, 1);

	Canvas c;

	while (!glfwWindowShouldClose(window))
	{
		if (t2 - t1 >= frame_duration) {
			glClear(GL_COLOR_BUFFER_BIT);
			process_input(window, posUnif, scaleUnif, iterUnif);
			c.draw();
			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		t2 = glfwGetTime();
	}
	// -------------------------------------------------------

	terminate(window);
}