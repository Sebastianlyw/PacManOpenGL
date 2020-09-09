#include "ShaderManager.h"
#include <iostream>

ShaderManager& ShaderManager::Use()
{
	glUseProgram(this->ID);
	return *this;
}

void ShaderManager::Compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource)
{
	uint32_t sVertex, sFragment, gShader;
	// vertex ShaderManager
	sVertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(sVertex, 1, &vertexSource, NULL);
	glCompileShader(sVertex);
	checkCompileErrors(sVertex, "VERTEX");
	// fragment ShaderManager
	sFragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(sFragment, 1, &fragmentSource, NULL);
	glCompileShader(sFragment);
	checkCompileErrors(sFragment, "FRAGMENT");
	// if geometry shader source code is given, also compile geometry shader
	if (geometrySource != nullptr)
	{
		gShader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(gShader, 1, &geometrySource, NULL);
		glCompileShader(gShader);
		checkCompileErrors(gShader, "GEOMETRY");
	}
	// shader program
	this->ID = glCreateProgram();
	glAttachShader(this->ID, sVertex);
	glAttachShader(this->ID, sFragment);
	if (geometrySource != nullptr)
		glAttachShader(this->ID, gShader);
	glLinkProgram(this->ID);
	checkCompileErrors(this->ID, "PROGRAM");
	// delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(sVertex);
	glDeleteShader(sFragment);
	if (geometrySource != nullptr)
		glDeleteShader(gShader);
}

void ShaderManager::SetFloat(const char* name, float value, bool useShader)
{
	if (useShader)
		this->Use();
	glUniform1f(glGetUniformLocation(this->ID, name), value);
}
void ShaderManager::SetInteger(const char* name, int value, bool useShader)
{
	if (useShader)
		this->Use();
	glUniform1i(glGetUniformLocation(this->ID, name), value);
}
void ShaderManager::SetVector2f(const char* name, float x, float y, bool useShader)
{
	if (useShader)
		this->Use();
	glUniform2f(glGetUniformLocation(this->ID, name), x, y);
}
void ShaderManager::SetVector2f(const char* name, const glm::vec2& value, bool useShader)
{
	if (useShader)
		this->Use();
	glUniform2f(glGetUniformLocation(this->ID, name), value.x, value.y);
}
void ShaderManager::SetVector3f(const char* name, float x, float y, float z, bool useShader)
{
	if (useShader)
		this->Use();
	glUniform3f(glGetUniformLocation(this->ID, name), x, y, z);
}
void ShaderManager::SetVector3f(const char* name, const glm::vec3& value, bool useShader)
{
	if (useShader)
		this->Use();
	glUniform3f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z);
}
void ShaderManager::SetVector4f(const char* name, float x, float y, float z, float w, bool useShader)
{
	if (useShader)
		this->Use();
	glUniform4f(glGetUniformLocation(this->ID, name), x, y, z, w);
}
void ShaderManager::SetVector4f(const char* name, const glm::vec4& value, bool useShader)
{
	if (useShader)
		this->Use();
	glUniform4f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z, value.w);
}
void ShaderManager::SetMatrix4(const char* name, const glm::mat4& matrix, bool useShader)
{
	if (useShader)
		this->Use();
	glUniformMatrix4fv(glGetUniformLocation(this->ID, name), 1, false, glm::value_ptr(matrix));
}


void ShaderManager::checkCompileErrors(uint32_t object, std::string type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(object, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(object, 1024, NULL, infoLog);
			std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
				<< infoLog << "\n -- --------------------------------------------------- -- "
				<< std::endl;
		}
	}
	else
	{
		glGetProgramiv(object, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(object, 1024, NULL, infoLog);
			std::cout << "| ERROR::ShaderManager: Link-time error: Type: " << type << "\n"
				<< infoLog << "\n -- --------------------------------------------------- -- "
				<< std::endl;
		}
	}
}