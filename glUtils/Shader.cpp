#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>

std::string Shader::readShaderFile(const char* filePath) {
	std::ifstream fileHandle;
	fileHandle.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	std::string fileText;

	// Read shader file stream into string
	try {
		fileHandle.open(filePath);

		std::stringstream stream;
		stream << fileHandle.rdbuf();
		fileHandle.close();

		fileText = stream.str();
	}
	catch (std::ifstream::failure e) {
		std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	
	return fileText;
}

unsigned int Shader::compileShader(const char* filePath, GLenum shaderType) {
	unsigned shader;
	int compilationStatus;
	char infoLog[512];

	std::string content = Shader::readShaderFile(filePath);
	const char* content_c = content.c_str();

	shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &content_c, NULL);
	glCompileShader(shader);

	// Check shader compilation status
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compilationStatus);
	if (!compilationStatus) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::COMPILATION_FAILED" << std::endl
			<< infoLog << std::endl;
	}

	return shader;
}

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	unsigned int vertex = Shader::compileShader(vertexPath, GL_VERTEX_SHADER);
	unsigned int fragment = Shader::compileShader(fragmentPath, GL_FRAGMENT_SHADER);

	int linkStatus;
	char infoLog[512];

	program = glCreateProgram();
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	glLinkProgram(program);

	// Check program linking status
	glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
	if (!linkStatus) {
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cerr << "ERROR::PROGRAM::LINKING_FAILED" << std::endl
			<< infoLog << std::endl;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);

	inspectAttributes();
	inspectUniforms();
}

void Shader::inspectAttributes() {
	attributes.clear();
	
	glUseProgram(program);

	GLint numAttributes;
	GLsizei nameMaxLength;
	glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &numAttributes);
	glGetProgramiv(program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &nameMaxLength);

	attributes.resize(numAttributes);
	for (GLint i = 0; i < numAttributes; i++) {
		GLchar* name = new GLchar[nameMaxLength];
		glGetActiveAttrib(program, i, nameMaxLength,
			&attributes[i].length,
			&attributes[i].size,
			&attributes[i].type,
			name
		);
		attributes[i].name = name;
		attributes[i].location = glGetAttribLocation(program, name);
		delete[] name;

		attributeLoc.insert({ attributes[i].name, attributes[i].location });
	}
}

void Shader::inspectUniforms() {
	uniforms.clear();

	glUseProgram(program);

	GLint numUniforms;
	GLsizei nameMaxLength;
	glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &numUniforms);
	glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &nameMaxLength);

	uniforms.resize(numUniforms);
	for (GLint i = 0; i < numUniforms; i++) {
		GLchar* name = new GLchar[nameMaxLength];
		glGetActiveUniform(program, i, nameMaxLength,
			&uniforms[i].length,
			&uniforms[i].size,
			&uniforms[i].type,
			name
		);
		uniforms[i].name = name;
		uniforms[i].location = glGetUniformLocation(program, name);
		delete[] name;

		uniformLoc.insert({ uniforms[i].name, uniforms[i].location });
	}
}

unsigned int Shader::get() {
	return program;
}

void Shader::use() {
	glUseProgram(program);
}