#include "ComputeShader.h"


ComputeShader::ComputeShader()
{
}

ComputeShader::ComputeShader(const char* path)
{
	this->path = path;
	create();
}

ComputeShader::~ComputeShader()
{
}

void ComputeShader::create()
{
	std::string shaderCode = FileHandler::loadStrFile(path);

	GLchar const* shaderChars = shaderCode.c_str();

	GLint shaderLength = shaderCode.size();

	GLuint computeShader = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(computeShader, 1, &shaderChars, &shaderLength);
	glCompileShader(computeShader);
	printIVSuccess(computeShader, GL_COMPILE_STATUS);

	ID = glCreateProgram();
	glAttachShader(ID, computeShader);
	glLinkProgram(ID);
	printIVSuccess(ID, GL_LINK_STATUS);

	glDeleteShader(computeShader);
}

void ComputeShader::reload()
{
	glDeleteProgram(ID);
	create();
}

void ComputeShader::use()
{
	glUseProgram(ID);
}

void ComputeShader::setInt(std::string name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void ComputeShader::setBool(std::string name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void ComputeShader::setFloat(std::string name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void ComputeShader::setVec2(const std::string& name, const glm::vec2& value) const
{
	glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void ComputeShader::setVec2(const std::string& name, float x, float y) const
{
	glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}

void ComputeShader::setVec3(const std::string& name, const glm::vec3& value) const
{
	int location = glGetUniformLocation(ID, name.c_str());
	glUniform3fv(location, 1, &value[0]);
}

void ComputeShader::setVec3(const std::string& name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void ComputeShader::setVec4(const std::string& name, const glm::vec4& value) const
{
	glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void ComputeShader::setVec4(const std::string& name, float x, float y, float z, float w) const
{
	glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}

void ComputeShader::setMat2(const std::string& name, const glm::mat2& mat) const
{
	glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void ComputeShader::setMat3(const std::string& name, const glm::mat3& mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void ComputeShader::setMat4(const std::string& name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}


void ComputeShader::printIVSuccess(unsigned int shader, GLenum status)
{
	int shaderSuccess;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &shaderSuccess);
	if (!shaderSuccess) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << status << " FAILED:\n" << infoLog << std::endl;
	}
}
