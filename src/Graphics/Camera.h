#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <memory>
using namespace std;

class Camera
{
public:
	Camera(int window_width, int window_height);
	~Camera();

	glm::mat4 Get_Projection();

	void UpdateViewport(int new_width, int new_height);

private:
	glm::mat4 projection;
};

//static unique_ptr<Camera> MainCamera;