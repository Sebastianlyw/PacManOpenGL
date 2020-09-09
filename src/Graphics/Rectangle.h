#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Rectangle
{
public:
	Rectangle();

	~Rectangle();

	void Draw();

	void Draw_Wireframe();

	glm::mat4 GetTransformationMatrx();

	void setposition(glm::vec2 pos);

	void setscale(glm::vec2 scal);

	void setrotation(float rotation);


	glm::vec2 getposition();

	glm::vec2 getscale();

	float getrotation();

	//vertex array object
	uint32_t VAO;
	uint32_t VBO; // vertex buffer object
	uint32_t CBO; // color buffer object
	uint32_t UVBO; // uv buffer object
private:

	glm::vec2 position;
	glm::vec2 scale;
	float rotaion_angle;


};