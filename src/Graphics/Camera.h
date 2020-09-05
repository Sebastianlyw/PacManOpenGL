#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;
using namespace std;

class Camera
{
	public:
		Camera(const Camera&) = delete;
		Camera& operator=(const Camera&) = delete;
		Camera(Camera&&) = delete;
		Camera& operator=(Camera&&) = delete;
		static auto& instance() {
			static Camera camera;
			return camera;
		}

		mat4 GetPerspectiveProjection();
		mat4 GetOrthoPrjection();
		mat4 GetViewMatrix();
	
	private:
		Camera();
		mat4 projection;
		mat4 view;
		float fov;
		vec3 cameraPos;
		vec3 cameraTarget;
		vec3 cameraUp;


	
};

