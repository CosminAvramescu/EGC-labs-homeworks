#include "lab_m1/lab2/lab2.h"

#include <vector>
#include <iostream>

#include "core/engine.h"
#include "utils/gl_utils.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */
bool cull = false;

Lab2::Lab2()
{
}


Lab2::~Lab2()
{
}


void Lab2::Init()
{
    cullFace = GL_BACK;
    polygonMode = GL_FILL;

    // Load a mesh from file into GPU memory
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Create a mesh box using custom data
    {
        vector<VertexFormat> vertices
        {
            VertexFormat(glm::vec3(-1, -1,  1), glm::vec3(0, 1, 1), glm::vec3(0.2, 0.8, 0.6)),
            // TODO(student): Complete the vertices data for the cube mesh
            VertexFormat(glm::vec3(1, 0, 1), glm::vec3(0, 1, 1), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(0, 1, 1), glm::vec3(0, 1, 1), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(1, 1, 1), glm::vec3(0, 1, 1), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0, 1, 1), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(1, 0, 0), glm::vec3(0, 1, 1), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(0, 1, 0), glm::vec3(0, 1, 1), glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(1, 1, 0), glm::vec3(0, 1, 1), glm::vec3(0.2, 0.8, 0.6))
        };

        vector<unsigned int> indices =
        {
            0, 1, 2,    // indices for first triangle
            1, 3, 2,    // indices for second triangle
            // TODO(student): Complete indices data for the cube mesh
            2, 3, 7,
            2, 7, 6,
            1, 7, 3,
            1, 5, 7,
            6, 7, 4,
            7, 5, 4,
            0, 4, 1,
            1, 4, 5,
            2, 6, 4,
            0, 2, 4
        };

        vector<VertexFormat> vertices2
        {
            VertexFormat(glm::vec3(0, 0,  0), glm::vec3(0, 1, 1)),
            // TODO: Complete the information for the cube
            VertexFormat(glm::vec3(2, 0, 0), glm::vec3(1, 0, 0)),
            VertexFormat(glm::vec3(2, 0, 2), glm::vec3(1, 0, 0)),
            VertexFormat(glm::vec3(0, 0, 2), glm::vec3(0, 1, 1)),
            VertexFormat(glm::vec3(1, 3, 1), glm::vec3(1, 0, 0)),

        };
        vector<unsigned int> indices2 =
        {
            0, 1, 2,
            2, 3, 0,
            0, 1, 4,
            1, 2, 4,
            0, 4, 3,
            2, 3, 4,
        };
        vector<VertexFormat> vertices3
        {
            VertexFormat(glm::vec3(0, 0,  0), glm::vec3(1, 0, 0)),
            VertexFormat(glm::vec3(2, 0, 0), glm::vec3(1, 0, 0)),
            VertexFormat(glm::vec3(2, 0, 2), glm::vec3(1, 0, 0)),
            VertexFormat(glm::vec3(0, 0, 2), glm::vec3(1, 0, 0)),

        };
        vector<unsigned int> indices3 =
        {
            2, 1, 0,
            2, 3, 0,
        };


        // TODO(student): Create a tetrahedron mesh. You can create it with
        // only 4 vertices, or you can choose the harder route and create it
        // with 12 vertices. Think about it, why would you want that, and how
        // would you do it? After all, a tetrahedron has only 4 vertices
        // by definition!

        // TODO(student): Create a square using two triangles with
        // opposing vertex orientations.

        meshes["cube_A"] = new Mesh("generated cube 1");
        meshes["cube_A"]->InitFromData(vertices, indices);
        meshes["tetraedru"] = new Mesh("tetraedru");
        meshes["tetraedru"]->InitFromData(vertices2, indices2);
        meshes["square"] = new Mesh("square triangles");
        meshes["square"]->InitFromData(vertices3, indices3);

        // Actually create the mesh from the data]
        CreateMesh("cube_B", vertices, indices);
    }


}


void Lab2::CreateMesh(const char *name, const std::vector<VertexFormat> &vertices, const std::vector<unsigned int> &indices)
{
    unsigned int VAO = 0;
    // TODO(student): Create the VAO and bind it
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO = 0;
    // TODO(student): Create the VBO and bind it
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // TODO(student): Send vertices data into the VBO buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    unsigned int IBO = 0;
    // TODO(student): Create the IBO and bind it
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    // TODO(student): Send indices data into the IBO buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    // ========================================================================
    // This section demonstrates how the GPU vertex shader program
    // receives data. It will be learned later, when GLSL shaders will be
    // introduced. For the moment, just think that each property value from
    // our vertex format needs to be sent to a certain channel, in order to
    // know how to receive it in the GLSL vertex shader.

    // Set vertex position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

    // Set vertex normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

    // Set texture coordinate attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

    // Set vertex color attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
    // ========================================================================

    // TODO(student): Unbind the VAO
    glBindVertexArray(VAO);

    // Check for OpenGL errors
    if (GetOpenGLError() == GL_INVALID_OPERATION)
    {
        cout << "\t[NOTE] : For students : DON'T PANIC! This error should go away when completing the tasks." << std::endl;
        cout << "\t[NOTE] : For developers : This happens because OpenGL core spec >=3.1 forbids null VAOs." << std::endl;
    }

    // Mesh information is saved into a Mesh object
    meshes[name] = new Mesh(name);
    meshes[name]->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
}


void Lab2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();

    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Lab2::Update(float deltaTimeSeconds)
{
    glLineWidth(3);
    glPointSize(5);
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

    // TODO(student): Enable face culling
    glEnable(GL_CULL_FACE);
    // TODO(student): Set face custom culling. Use the `cullFace` variable.
    if (cull)
        glCullFace(GL_FRONT);
    else
        glCullFace(GL_BACK);
    // Render an object using face normals for color
    RenderMesh(meshes["box"], shaders["VertexNormal"], glm::vec3(0, 0.5f, -1.5f), glm::vec3(0.75f));

    // Render an object using colors from vertex
    RenderMesh(meshes["cube_B"], shaders["VertexColor"], glm::vec3(0.5f, 0.5f, 1.5f), glm::vec3(0.25f));
    RenderMesh(meshes["tetraedru"], shaders["VertexColor"], glm::vec3(0.5f, 0.5f, 0), glm::vec3(0.25f));
    RenderMesh(meshes["cube_C"], shaders["VertexColor"], glm::vec3(1, 1, 1), glm::vec3(0.50f));
    RenderMesh(meshes["square"], shaders["VertexColor"], glm::vec3(3, 0, 2), glm::vec3(0.50f));

    // TODO(student): Draw the mesh that was created with `CreateMesh()`

    // TODO(student): Draw the tetrahedron

    // TODO(student): Draw the square

    // TODO(student): Disable face culling
    float radius = 0.4f;
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(0.2f, 0.7f, 0.1f);
    glVertex2f(2, 2);
    for (int i = 0; i <= 360; i = i + 60) {
        glVertex2f(2 + radius * cos(i * M_PI / 180), 2 + radius * sin(i * M_PI / 180));
    }
    glEnd();

    glDisable(GL_CULL_FACE);

}


void Lab2::FrameEnd()
{
    DrawCoordinateSystem();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab2::OnInputUpdate(float deltaTime, int mods)
{
}


void Lab2::OnKeyPress(int key, int mods)
{
    // TODO(student): Switch between GL_FRONT and GL_BACK culling.
    // Save the state in `cullFace` variable and apply it in the
    // `Update()` method, NOT here!

    if (key == GLFW_KEY_SPACE)
    {
        switch (polygonMode)
        {
        case GL_POINT:
            polygonMode = GL_FILL;
            break;
        case GL_LINE:
            polygonMode = GL_POINT;
            break;
        default:
            polygonMode = GL_LINE;
            break;
        }
    }

    if (key == GLFW_KEY_F2) {
        if (cull)
            cull = false;
        else
            cull = true;
    }
}


void Lab2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Lab2::OnWindowResize(int width, int height)
{
}
