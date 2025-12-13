#pragma once

#ifndef __gl_h_
#include <glad/gl.h>
#endif

#include <string>
#include <vector>
#include <map>

struct GLVar {
	std::string name;
	GLsizei length;
	GLint size;
	GLenum type;

	GLuint index;
	GLint offset;
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
	std::map<std::string, const GLVar *const> active_uniforms;

	std::map<std::string, GLint> uniformLoc;
	std::map<std::string, GLint> attributeLoc;

	Shader(const char* vertexPath, const char* fragmentPath);

	unsigned int get();
	
	void use();
};