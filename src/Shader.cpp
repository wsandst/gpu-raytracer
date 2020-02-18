#include "Shader.h"


Shader::Shader() 
{
}

Shader::Shader(const char * vertexPath, const char * fragmentPath)
{
	std::string vertexCode = FileHandler::loadStrFile(vertexPath);
	std::string fragmentCode = FileHandler::loadStrFile(fragmentPath);

	GLchar const* vertexChars = vertexCode.c_str();
	GLchar const* fragmentChars = fragmentCode.c_str();

	GLint vertexLength = vertexCode.size();
	GLint fragmentLength = fragmentCode.size();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexChars, &vertexLength);
	glCompileShader(vertexShader);
	printIVSuccess(vertexShader, GL_COMPILE_STATUS);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentChars, &fragmentLength);
	glCompileShader(fragmentShader);
	printIVSuccess(fragmentShader, GL_COMPILE_STATUS);

	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);
	printIVSuccess(ID, GL_LINK_STATUS);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

Shader::~Shader()
{
}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::setInt(std::string name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setBool(std::string name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setFloat(std::string name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec2(const std::string &name, const glm::vec2 &value) const
{
	glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setVec2(const std::string &name, float x, float y) const
{
	glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
{
	int id = glGetUniformLocation(ID, name.c_str());
	glUniform3fv(id, 1, &value[0]);
}

void Shader::setVec3(const std::string &name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void Shader::setVec4(const std::string &name, const glm::vec4 &value) const
{
	glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setVec4(const std::string &name, float x, float y, float z, float w) const
{
	glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}

void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const
{
	glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}


void Shader::printIVSuccess(unsigned int shader, GLenum status)
{
	int shaderSuccess;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &shaderSuccess);
	if (!shaderSuccess) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << status << " FAILED:\n" << infoLog << std::endl;
	}
}
