#include "TestTexture2D.h"

#include"renderer.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test {
	TestTexture2D::TestTexture2D()
        : m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
        m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
        m_translationA(200, 200, 0),m_translationB(400, 200,0)
	{
        float positions[] = {
          50.0f,-50.0f,1.0f,0.0f,//1
          50.0f, 50.0f,1.0f,1.0f,//2
         -50.0f, 50.0f,0.0f,1.0f,//3
         -50.0f,-50.0f,0.0f,0.0f,//0

        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0,
         
        };
    
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        m_VAO = std::make_unique<VertexArray>();
        m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);   
      

        m_VAO->AddBuffer(*m_VertexBuffer,layout);
        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);


        m_shader = std::make_unique<Shader>("res/shaders/Basic.shader");
        m_shader->Bind();
        m_shader->SetUniforme4f("u_Color", 0.2f, 0.3f, 0.4f, 1.0f);
        m_Texture = std::make_unique<Texture>("res/textures/zyfirlogo.png");
        m_shader->SetUniforme1i("u_Texture", 0);
         
               // glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	TestTexture2D::~TestTexture2D()
	{
	}
	void TestTexture2D::OnUpdate(float deltaTime)
	{
	}
	void TestTexture2D::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

        Renderer renderer;

        m_Texture->Bind();



        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_translationA);
            glm::mat4 mvp = m_Proj * m_View * model;
            m_shader->Bind();//bind一次绑定就一直绑定
            m_shader->SetUniformMat4f("u_MVP", mvp);

            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_shader);
        }

        {
            glm::mat4 model = glm::translate(glm:: mat4(1.0f), m_translationB);
            glm::mat4 mvp = m_Proj * m_View * model;
            m_shader->Bind();  
            m_shader->SetUniformMat4f("u_MVP", mvp);

            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_shader);
        }

   
	}
	void TestTexture2D::OnImGuiRender()
	{
        ImGui::SliderFloat3("Translation A", &m_translationA.x, 0.0f, 960.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::SliderFloat3("Translation B", &m_translationB.x, 0.0f, 960.0f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}