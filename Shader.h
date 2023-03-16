#pragma once
#include<string>
#include<unordered_map>

#include"glm/glm.hpp"

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	mutable std::unordered_map<std::string, int> m_UniformLocationCache;
public:   
	Shader(const std::string& filepath);	
	~Shader();

	void Bind() const;
	void Unbind() const;
  
	//set uniform
	void SetUniforme1i(const std::string& name, int value);
	void SetUniforme1f(const std::string& name, float value);
	void SetUniforme4f(const std::string& name, float v0, float v1, float f2, float f3);
	void SetUniform1iv(const std::string& name, int count, int* value);

	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
private:
	ShaderProgramSource ParsShader(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	
	int GetUniformLocation(const std::string& name) const;
};