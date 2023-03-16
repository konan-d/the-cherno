#include "TestDynamicBatch.h"

#include"renderer.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include<array>

uint32_t Vertexcount = 1000;
uint32_t VBcount = 4 * Vertexcount;
uint32_t Indexcount = 6 * Vertexcount;

struct vec2
{
    float x, y;
};

struct vec4
{
    float x, y, z, w;
};


struct Vertex
{
    vec2 position;
    vec2 Texcoords;
    vec4 Color;
    float TexID;
};


static std::array<Vertex, 4> CreateQuad(float x, float y, float texid)
{
    float size = 100.0f;

    Vertex v0;   
    v0.position = { x + size  , y};
    v0.Texcoords = { 1.0f,0.0f};
    v0.Color = { 0.18f, 0.6f, 0.96f, 1.0f };
    v0.TexID = texid;

    Vertex v1;
    v1.position = { x + size , y + size };
    v1.Texcoords = { 1.0f,1.0f};
    v1.Color = { 0.18f, 0.6f, 0.96f, 1.0f };
    v1.TexID = texid;

    Vertex v2;
    v2.position = { x, y + size};
    v2.Texcoords = { 0.0f,1.0f};
    v2.Color = { 0.18f, 0.6f, 0.96f, 1.0f };
    v2.TexID = texid;

    Vertex v3;
    v3.position = { x , y };
    v3.Texcoords = { 0.0f,0.0f};
    v3.Color = { 0.18f, 0.6f, 0.96f, 1.0f };
    v3.TexID = texid;

    return {v0, v1, v2, v3};
}

namespace test {
    TestDynamicBatch::TestDynamicBatch()
        : m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
        m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))
        // m_translationA(200, 200, 0), m_translationB(400, 200, 0)
    {
        unsigned int indices[] = {
       0, 1, 2, 2, 3, 0,
       4, 5, 6, 6, 7, 4
        };
      

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        m_VAO = std::make_unique<VertexArray>();
        m_VAO->Bind();

        GLCall(glCreateBuffers(1, &m_RendererBID));//id值就是前面的1这个数赋予。
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererBID));
        GLCall(glBufferData(GL_ARRAY_BUFFER, VBcount, nullptr, GL_DYNAMIC_DRAW));

        GLCall(glEnableVertexAttribArray(0));
        GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, position)));

        GLCall(glEnableVertexAttribArray(1));
        GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Texcoords)));

        GLCall(glEnableVertexAttribArray(2));
        GLCall(glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Color)));

        GLCall(glEnableVertexAttribArray(3));
        GLCall(glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexID)));


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


    TestDynamicBatch::~TestDynamicBatch()
    {
    }
    void TestDynamicBatch::OnUpdate(float deltaTime)
    {
    }
    void TestDynamicBatch::OnRender()
    {
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        //set dynamic buffer
        /*float positions[] = {
         200.0f, 100.0f,1.0f,0.0f,0.18f, 0.6f, 0.96f, 1.0f, 0.0f,//1
         200.0f, 300.0f,1.0f,1.0f,0.18f, 0.6f, 0.96f, 1.0f, 0.0f,//2
        -200.0f, 300.0f,0.0f,1.0f,0.18f, 0.6f, 0.96f, 1.0f, 0.0f,//3
        -200.0f, 100.0f,0.0f,0.0f,0.18f, 0.6f, 0.96f, 1.0f, 0.0f, //0

         700.0f, 100.0f,1.0f,0.0f, 1.0f, 0.6f, 0.3f, 1.0f, 1.0f,//1
         700.0f, 300.0f,1.0f,1.0f, 1.0f, 0.6f, 0.3f, 1.0f, 1.0f,//2
         300.0f, 300.0f,0.0f,1.0f, 1.0f, 0.6f, 0.3f, 1.0f, 1.0f,//3
         300.0f, 100.0f,0.0f,0.0f, 1.0f, 0.6f, 0.3f, 1.0f, 1.0f//0
        };*/

        auto q0 = CreateQuad(move[0], move[1], 0.0f);
        auto q1 = CreateQuad(400.0f, 100.0f, 1.0f);

        Vertex positions[8];
        memcpy(positions, q0.data(), q0.size() * sizeof(Vertex));
        memcpy(positions + q0.size(), q1.data(), q1.size() * sizeof(Vertex));

          

        glBindBuffer(GL_ARRAY_BUFFER, m_RendererBID);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(positions), positions);

        Renderer renderer;

        for (int i = 0; i < 2; i++)
        {
            m_Texture[i]->Bind(i);
        }
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(100.0f, 100.0f, 0.0f));
            glm::mat4 mvp = m_Proj * m_View * model;
            m_shader->Bind();
            m_shader->SetUniformMat4f("u_MVP", mvp);

            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_shader);
        }
    }
    void TestDynamicBatch::OnImGuiRender()
    {        
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::DragFloat2("control", move, 1.0f);

    }
}