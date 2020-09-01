#include "Window.h"
#include "PacmanGame.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
PacmanGame Game;
Window::Window(int width, int height)
	:m_width(width),
	m_height(height)
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, false);

	window_ptr = glfwCreateWindow(width, height, "Pacman", NULL, NULL);
	if (window_ptr == nullptr)
	{
		cout << "erorr initilize glfw" << endl;
		return;
	}
	glfwSetKeyCallback(window_ptr, key_callback);
	glfwMakeContextCurrent(window_ptr);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}

	glViewport(0, 0, width, height);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// initialize game
	// ---------------
	Game.Init();

	startFrame = 0;
	lastFrame = 0;
	delta = 0;
}

Window::~Window()
{
	glfwTerminate();
}

void Window::Mainloop()
{
	while (!glfwWindowShouldClose(window_ptr))
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		float currentFrame = glfwGetTime();
		delta = currentFrame - lastFrame;
		lastFrame = currentFrame;

		Game.Update(delta);
		Game.InputUpdate(delta);
		Game.Render(delta);

		glfwSwapBuffers(window_ptr);
		glfwPollEvents();
	}
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// when a user presses the escape key, we set the WindowShouldClose property to true, closing the application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			Game.Keys[key] = true;
		else if (action == GLFW_RELEASE)
			Game.Keys[key] = false;
	}
}


