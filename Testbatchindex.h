#pragma once

#include "Test.h"


#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

#include<memory>

namespace test {

	class Testbatchindex : public Test
	{
	public:
		Testbatchindex();
		~Testbatchindex();

		void OnUpdate(float deltaTime) override;
		void OnRender()  override;
		void OnImGuiRender()  override;
	private:
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr <IndexBuffer> m_IndexBuffer;
		std::unique_ptr <Shader> m_shader;
		std::unique_ptr <Texture> m_Texture[2];

		glm::mat4 m_Proj, m_View;
		glm::vec3 m_translationA, m_translationB;
		float move[2] = { 100.0f, 200.0f };

		unsigned int m_RendererBID;
		uint32_t indexnum = 0;

	};
}