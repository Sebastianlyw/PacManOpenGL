#pragma once

#include <string>

static const float EPSILON = 0.0001f;
const float PI = 3.14159f;
const float TWO_PI = 2.0f * PI;

bool IsEqual(float x, float y);

bool IsGreaterThanOrEqual(float x, float y);

bool IsLessThanOrEqual(float x, float y);

float MillisecondsToSeconds(uint32_t milliseconds);

uint32_t GetIndex(uint32_t width, uint32_t r, uint32_t c);
//STring == string
bool StringCompare(const std::string& a, const std::string& b);

float Clamp(float val, float min, float max);

struct Size
{
	uint32_t width = 0, height = 0;
};
