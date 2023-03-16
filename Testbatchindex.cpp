#include "Testbatchindex.h"

#include"renderer.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include<array>

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

static Vertex* CreateQuad(Vertex* buffer, float x, float y, float texid)
{
    float size = 100.0f;

    buffer->position = { x + size  , y };
    buffer->Texcoords = { 1.0f,0.0f };
    buffer->Color = { 0.18f, 0.6f, 0.96f, 1.0f };
    buffer->TexID = texid;
    buffer++;
     
    buffer->position = { x + size , y + size };
    buffer->Texcoords = { 1.0f,1.0f };
    buffer->Color = { 0.18f, 0.6f, 0.96f, 1.0f };
    buffer->TexID = texid;
    buffer++;

    buffer->position = { x, y + size };
    buffer->Texcoords = { 0.0f,1.0f };
    buffer->Color = { 0.18f, 0.6f, 0.96f, 1.0f };
    buffer->TexID = texid;
    buffer++;

    buffer->position = { x , y };
    buffer->Texcoords = { 0.0f,0.0f };
    buffer->Color = { 0.18f, 0.6f, 0.96f, 1.0f };
    buffer->TexID = texid;
    buffer++;

    return buffer ;
}

const size_t MaxQuaqcount = 1000;
const size_t MaxVertecount = 4 * MaxQuaqcount;
const size_t MaxIndexcount = 6 * MaxQuaqcount;
uint32_t indices[MaxIndexcount];

namespace test {
    Testbatchindex::Testbatchindex()
        : m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
        m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))
        // m_translationA(200, 200, 0), m_translationB(400, 200, 0)
    {



        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        m_VAO = std::make_unique<VertexArray>();
        m_VAO->Bind();

        GLCall(glCreateBuffers(1, &m_RendererBID));//id值就是前面的1这个数赋予。
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererBID));
        GLCall(glBufferData(GL_ARRAY_BUFFER, MaxVertecount * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW));

        GLCall(glEnableVertexAttribArray(0));
        GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, position)));

        GLCall(glEnableVertexAttribArray(1));
        GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Texcoords)));

        GLCall(glEnableVertexAttribArray(2));
        GLCall(glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Color)));

        GLCall(glEnableVertexAttribArray(3));
        GLCall(glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexID)));


        uint32_t offset = 0;
       
        for (int i = 0; i < MaxIndexcount; i += 6)
        {
            indices[i + 0] = 0 + offset;
            indices[i + 1] = 1 + offset;
            indices[i + 2] = 2 + offset;

            indices[i + 3] = 2 + offset;
            indices[i + 4] = 3 + offset;
            indices[i + 5] = 0 + offset;
            offset += 4;
        }



        m_shader = std::make_unique<Shader>("res/shaders/Batch.shader");
        m_shader->Bind();
        m_shader->SetUniforme4f("u_Color", 0.2f, 0.3f, 0.4f, 1.0f);
        m_Texture[0] = std::make_unique<Texture>("res/textures/zyfirlogo.png");
        m_Texture[1] = std::make_unique<Texture>("res/textures/seclogo.png");

        int samplers[2] = { 0,1 };
        m_shader->SetUniform1iv("u_Texture", 2, samplers);

        // glBindBuffer(GL_ARRAY_BUFFER, 0);
    }


    Testbatchindex::~Testbatchindex()
    {
    }
    void Testbatchindex::OnUpdate(float deltaTime)
    {
    }
    void Testbatchindex::OnRender()
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

        std::array<Vertex, 1000> positions;
        Vertex* buffer = positions.data();
        for (int y = 0; y < 6; y++)
        {
            for (int x = 0; x < 6; x++)
            {
                buffer = CreateQuad(buffer, x * 100, y * 100, (x + y) % 2);
                indexnum += 6;
            }

        }
        buffer = CreateQuad(buffer, move[0], move[1], 0.0f);
        indexnum += 6;

        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, indexnum);

        glBindBuffer(GL_ARRAY_BUFFER, m_RendererBID);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * positions.size(), positions.data());

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
    void Testbatchindex::OnImGuiRender()
    {
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::DragFloat2("control", move, 1.0f);

    }
}