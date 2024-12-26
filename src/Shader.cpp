#include "Shader.h"
#include <glad/glad.h>
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>

Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath) {
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vertexShaderFile;
	std::ifstream fragmentShaderFile;

	vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		vertexShaderFile.open(vertexShaderPath);
		fragmentShaderFile.open(fragmentShaderPath);

		std::stringstream vertexShaderStream, fragmentShaderStream;

		vertexShaderStream << vertexShaderFile.rdbuf();
		fragmentShaderStream << fragmentShaderFile.rdbuf();

		vertexShaderFile.close();
		fragmentShaderFile.close();

		vertexCode = vertexShaderStream.str();
		fragmentCode = fragmentShaderStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
	}

	const char* vshaderCode = vertexCode.c_str();
	const char* fshaderCode = fragmentCode.c_str();

	unsigned int vertexId, fragmentId;
	int success;
	char infoLog[512];

	vertexId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexId, 1, &vshaderCode, NULL);
	glCompileShader(vertexId);

	glGetShaderiv(vertexId, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexId, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << std::endl;
	}

	fragmentId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentId, 1, &fshaderCode, NULL);
	glCompileShader(fragmentId);

	glGetShaderiv(fragmentId, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentId, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << std::endl;
	}
	
	ID = glCreateProgram();
	glAttachShader(ID, vertexId);
	glAttachShader(ID, fragmentId);

	glLinkProgram(ID);
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;

	}

	glDeleteShader(vertexId);
	glDeleteShader(fragmentId);


};

void Shader::use() {
	glUseProgram(ID);
}

void Shader::setBool(const char* vertexShaderPath, bool value) const {
	glUniform1i(glGetUniformLocation(ID, vertexShaderPath), (int)value);
}