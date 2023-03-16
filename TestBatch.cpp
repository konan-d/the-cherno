#include "TestBatch.h"

#include"renderer.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test {
    TestBatch::TestBatch()
        : m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
        m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))
       // m_translationA(200, 200, 0), m_translationB(400, 200, 0)
    {
        float positions[] = {
          200.0f, 100.0f,1.0f,0.0f,0.18f, 0.6f, 0.96f, 1.0f, 0.0f,//1
          200.0f, 300.0f,1.0f,1.0f,0.18f, 0.6f, 0.96f, 1.0f, 0.0f,//2
         -200.0f, 300.0f,0.0f,1.0f,0.18f, 0.6f, 0.96f, 1.0f, 0.0f,//3
         -200.0f, 100.0f,0.0f,0.0f,0.18f, 0.6f, 0.96f, 1.0f, 0.0f, //0

          700.0f, 100.0f,1.0f,0.0f, 1.0f, 0.6f, 0.3f, 1.0f, 1.0f,//1
          700.0f, 300.0f,1.0f,1.0f, 1.0f, 0.6f, 0.3f, 1.0f, 1.0f,//2
          300.0f, 300.0f,0.0f,1.0f, 1.0f, 0.6f, 0.3f, 1.0f, 1.0f,//3
          300.0f, 100.0f,0.0f,0.0f, 1.0f, 0.6f, 0.3f, 1.0f, 1.0f//0
        };

        unsigned int indices[] = {
            0, 1, 2,2, 3, 0,
            4, 5, 6, 6, 7, 4
        };

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        m_VAO = std::make_unique<VertexArray>();
        m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 9 * 8 * sizeof(float));
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        layout.Push<float>(4);
        layout.Push<float>(1);


        m_VAO->AddBuffer(*m_VertexBuffer, layout);
        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 12);


        m_shader = std::make_unique<Shader>("res/shaders/Batch.shader");
        m_shader->Bind();
        m_shader->SetUniforme4f("u_Color", 0.2f, 0.3f, 0.4f, 1.0f);
        m_Texture[0] = std::make_unique<Texture>("res/textures/zyfirlogo.png");
        m_Texture[1] = std::make_unique<Texture>("res/textures/seclogo.png");

        int samplers[2] = { 0,1 };
        m_shader->SetUniform1iv("u_Texture", 2, samplers);

        // glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    TestBatch::~TestBatch()
    {
    }
    void TestBatch::OnUpdate(float deltaTime)
    {
    }
    void TestBatch::OnRender()
    {
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        Renderer renderer;


        for (int i = 0; i < 2; i++)
        {
            m_Texture[i]->Bind(i);
        }
        

        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_translationA);
            glm::mat4 mvp = m_Proj * m_View * model;
            m_shader->Bind();//bind一次绑定就一直绑定
            m_shader->SetUniformMat4f("u_MVP", mvp);

            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_shader);
        }

        { 
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_translationB);
            glm::mat4 mvp = m_Proj * m_View * model;
            m_shader->Bind();
            m_shader->SetUniformMat4f("u_MVP", mvp);

            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_shader);
        }
        
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(100.0f, 100.0f, 0.0f));
            glm::mat4 mvp = m_Proj * m_View * model;
            m_shader->Bind();
            m_shader->SetUniformMat4f("u_MVP", mvp);

            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_shader);
        }
    }
    void TestBatch::OnImGuiRender()
    {
        ImGui::SliderFloat3("Translation A", &m_translationA.x, 0.0f, 960.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::SliderFloat3("Translation B", &m_translationB.x, 0.0f, 960.0f); 
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
}