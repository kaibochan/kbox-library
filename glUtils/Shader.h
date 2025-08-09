#pragma once

#ifndef __gl_h_
#include <glad/glad.h>
#endif

#include <string>
#include <vector>
#include <map>

struct GLVar {
	std::string name;
	GLsizei length;
	GLint size;
	GLenum type;

	GLint location;
};

class Shader {
private:
	unsigned int program;

	std::vector<GLVar> attributes;
	std::vector<GLVar> uniforms;

	static std::string readShaderFile(const char* filePath);
	static unsigned int compileShader(const char* filePath, GLenum shaderType);

	void inspectAttributes();
	void inspectUniforms();

public:
	std::map<std::string, GLint> uniformLoc;
	std::map<std::string, GLint> attributeLoc;

	Shader(const char* vertexPath, const char* fragmentPath);

	unsigned int get();
	
	void use();
};