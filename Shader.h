#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
	GLuint program;

	Shader(const char* vertexPath, const char* fragmentPath);
	void use();
	void deleteShader();
};

#endif