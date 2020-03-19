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

void ComputeShader::setObject(std::string& name, const shaderObject object)
{
	glUniform1i(glGetUniformLocation(ID, (name + ".type").c_str()), object.type);
	glUniform3fv(glGetUniformLocation(ID, (name + ".pos").c_str()), 1, &object.pos[0]);
	glUniform4fv(glGetUniformLocation(ID, (name + ".mat.color").c_str()), 1, &object.mat.color[0]);
	glUniform1f(glGetUniformLocation(ID, (name + ".mat.reflectivity").c_str()), object.mat.reflectivity);
	glUniform1f(glGetUniformLocation(ID, (name + ".mat.refractivity").c_str()), object.mat.refractivity);
	glUniform1f(glGetUniformLocation(ID, (name + ".mat.transparency").c_str()), object.mat.transparency);

	glUniform1i(glGetUniformLocation(ID, (name + ".vStart").c_str()), object.vStart);
	glUniform1i(glGetUniformLocation(ID,(name + ".vEnd").c_str()), object.vEnd);

	glUniform1f(glGetUniformLocation(ID, (name + ".scale").c_str()), object.scale);

	glUniform3fv(glGetUniformLocation(ID, (name + ".bMax").c_str()), 1, &object.bMax[0]);
	glUniform3fv(glGetUniformLocation(ID, (name + ".bMin").c_str()), 1, &object.bMin[0]);
}

void ComputeShader::setObjects(const std::string &name, std::vector<shaderObject>& objects)
{
	for (size_t i = 0; i < objects.size(); i++)
	{
		std::ostringstream os;
        os << name << "[" << i << "]";
		std::string newName = os.str();
		setObject(newName, objects[i]);
	}
}

void ComputeShader::setLight(std::string &name, const shaderLight light) 
{
	glUniform1i(glGetUniformLocation(ID, (name + ".type").c_str()), light.type);
	glUniform3fv(glGetUniformLocation(ID, (name + ".pos").c_str()), 1, &light.pos[0]);
	glUniform3fv(glGetUniformLocation(ID, (name + ".color").c_str()), 1, &light.color[0]);
	glUniform1f(glGetUniformLocation(ID, (name + ".intensity").c_str()), light.intensity);
}

void ComputeShader::setLights(const std::string &name, const std::vector<shaderLight>& lights)
{
	for (size_t i = 0; i < lights.size(); i++)
	{
		std::ostringstream os;
        os << name << "[" << i << "]";
		std::string newName = os.str();
		setLight(newName, lights[i]);
	}
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
