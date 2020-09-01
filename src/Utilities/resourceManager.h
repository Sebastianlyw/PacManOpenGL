#pragma once

#include <glad/glad.h>
#include <map>
#include <string>
#include "../Graphics/ShaderManager.h"

class ResourceManager
{
public:
	static std::map<std::string, ShaderManager> Shaders;
	static ShaderManager&  LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name);
	static ShaderManager&  GetShader(std::string name);
	
	static void      Clear();
private:
	ResourceManager() { }
	static ShaderManager loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile = nullptr);
	
};
