#include "ShaderManager.h"

std::stringstream ReadFromFile(std::string pathToFile)
{
	std::ifstream t(pathToFile);
	std::stringstream buffer;
	buffer << t.rdbuf();
	return buffer;
}


unsigned int CreateShader(std::string pathToFile) {
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	return vertexShader;
}