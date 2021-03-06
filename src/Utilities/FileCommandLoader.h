#pragma once

#include <functional>
#include <vector>
#include <string>
#include <stdint.h>
#include <glm/glm.hpp>

enum class CommandType : unsigned char
{
	COMMAND_ONE_LINE = 0,
	COMMAND_MULTI_LINE
};

struct ParseFuncParams
{
	std::string line;
	size_t dilimitPos;
	uint32_t lineNum;
};

using ParseFunc = std::function<void(const ParseFuncParams& params)>;

struct Command
{
	CommandType commandType = CommandType::COMMAND_ONE_LINE;
	std::string command = "";
	ParseFunc parseFunc = nullptr;
};

class FileCommandLoader
{
public:

	void AddCommand(const Command& command);
	bool LoadFile(const std::string& filePath);

	static glm::vec2 ReadSize(const ParseFuncParams& params);
	static int ReadInt(const ParseFuncParams& params);
	static std::string ReadString(const ParseFuncParams& params);
	static char ReadChar(const ParseFuncParams& params);

private:
	std::vector<Command> mCommands;
};

