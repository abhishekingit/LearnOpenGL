#pragma once

#include <glad/glad.h>
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>

class Shader {
public:

	unsigned int ID;

	Shader(const char* vertexShaderPath, const char* fragmentShaderPath);

	void use();

	void setBool(const char* vertexShaderPath, bool value) const;
	void setInt(const char* vertexShaderPath, int value) const;

};
