#pragma once
#include<stdint.h>
class Texture
{
public:
	Texture(const char* filepath);

	~Texture();

	void Bind();

	int Getwidth();

	int Getheight();

	unsigned char* Getdata();

private:
	int width, height, numcomponents;
	unsigned char* data;
	uint32_t textureID;
};