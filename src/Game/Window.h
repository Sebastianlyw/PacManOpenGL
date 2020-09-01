#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "../Graphics/Camera.h"
using namespace std;

class Window
{
public:
	Window(int width, int height);

	~Window();
	void Resize();

	void Mainloop();

private:
	int m_width, m_height;
	GLFWwindow* window_ptr;
	double startFrame, lastFrame, delta;
};
