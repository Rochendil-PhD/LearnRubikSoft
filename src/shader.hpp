#ifndef _SHADER_HPP_
#define _SHADER_HPP_

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "..\lib\glew\glew.h"

unsigned int loadShader(const char* VertexShaderPath, const char* FragmentShaderPath) {

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	std::fstream vertexFile(VertexShaderPath, std::ios_base::in);

	std::string vertexData;

	vertexFile.seekg(0, std::ios::end);
	vertexData.resize((unsigned int)vertexFile.tellg());
	vertexFile.seekg(0, std::ios::beg);
	vertexFile.read(&vertexData[0], vertexData.size());
	vertexFile.close();

	std::fstream fragmentFile(FragmentShaderPath, std::ios_base::in);
	std::string fragmentData;

	fragmentFile.seekg(0, std::ios::end);
	fragmentData.resize((unsigned int)fragmentFile.tellg());
	fragmentFile.seekg(0, std::ios::beg);
	fragmentFile.read(&fragmentData[0], fragmentData.size());
	fragmentFile.close();

	int Result = GL_FALSE;


	printf("Compiling shader : %s\n", VertexShaderPath);
	const char * vertexC_str = vertexData.c_str();
	const int vertexSize = vertexData.size();
	glShaderSource(vertexShader, 1, &vertexC_str, &vertexSize);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &Result);

	if (Result == GL_FALSE) {
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &Result);
		std::vector<char> error_log(Result);
		glGetShaderInfoLog(vertexShader, Result, NULL, &error_log[0]);
		fprintf(stderr, "Vertex Shader ERROR:\n%s\n", &error_log[0]);
		return 0;
	}

	printf("Compiling shader : %s\n", FragmentShaderPath);
	const char * fragmentC_str = fragmentData.c_str();
	const int fragmentSize = fragmentData.size();
	glShaderSource(fragmentShader, 1, &fragmentC_str, &fragmentSize);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &Result);

	if (Result == GL_FALSE) {
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &Result);
		std::vector<char> error_log(Result);
		glGetShaderInfoLog(fragmentShader, Result, NULL, &error_log[0]);
		fprintf(stderr, "Fragment Shader ERROR:\n%s\n", &error_log[0]);
		return 0;
	}

	unsigned int program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	glGetProgramiv(program, GL_COMPILE_STATUS, &Result);

	if (Result == GL_FALSE) {
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &Result);
		std::vector<char> error_log(Result);
		glGetProgramInfoLog(program, Result, NULL, &error_log[0]);
		fprintf(stderr, "Fragment Shader ERROR:\n%s\n", &error_log[0]);
		return 0;
	}

	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return program;
}

#endif // _SHADER_HPP_