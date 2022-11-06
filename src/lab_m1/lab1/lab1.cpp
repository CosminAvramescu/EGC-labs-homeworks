#include "lab_m1/lab1/lab1.h"

#include <vector>
#include <string.h>
#include <iostream>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


float color, x=-1, y=0.5f, z=0, x_2=1, y_2=0.5f, z_2=0;
string mesh="box";

Lab1::Lab1()
{
    // TODO(student): Never forget to initialize class variables!

}


Lab1::~Lab1()
{
}


void Lab1::Init()
{
    // Load a mesh from file into GPU memory. We only need to do it once,
    // no matter how many times we want to draw this mesh.
    {
        Mesh* mesh1 = new Mesh("box");
        mesh1->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");

        Mesh* mesh2 = new Mesh("sphere");
        mesh2->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");

        Mesh* mesh3 = new Mesh("teapot");
        mesh3->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "teapot.obj");

        meshes[mesh1->GetMeshID()] = mesh1;
        meshes[mesh2->GetMeshID()] = mesh2;
        meshes[mesh3->GetMeshID()] = mesh3;
    }

    // TODO(student): Load some more meshes. The value of RESOURCE_PATH::MODELS
    // is actually a path on disk, go there and you will find more meshes.

}


void Lab1::FrameStart()
{
}


void Lab1::Update(float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->props.resolution;

    // Sets the clear color for the color buffer

    // TODO(student): Generalize the arguments of `glClearColor`.
    // You can, for example, declare three variables in the class header,
    // that will store the color components (red, green, blue).
    glClearColor(color, 0, 0, 1);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);

    // Render the object
    RenderMesh(meshes["box"], glm::vec3(2, 0.5f, 1), glm::vec3(0.5f));

    // Render the object again but with different properties
    RenderMesh(meshes[mesh], glm::vec3(x, y, z));

    RenderMesh(meshes["sphere"], glm::vec3(x_2, y_2, z_2));
    // TODO(student): We need to render (a.k.a. draw) the mesh that
    // was previously loaded. We do this using `RenderMesh`. Check the
    // signature of this function to see the meaning of its parameters.
    // You can draw the same mesh any number of times.
}


void Lab1::FrameEnd()
{
    DrawCoordinateSystem();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab1::OnInputUpdate(float deltaTime, int mods)
{
    // Treat continuous update based on input

    // TODO(student): Add some key hold events that will let you move
    // a mesh instance on all three axes. You will also need to
    // generalize the position used by `RenderMesh`.
    if (window->KeyHold(GLFW_KEY_W)) {
        // TODO(student): Change the values of the color components.
        z -= 0.3f * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_A)) {
        // TODO(student): Change the values of the color components.
        x -= 0.3f * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_S)) {
        // TODO(student): Change the values of the color components.
        z += 0.3f * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_D)) {
        // TODO(student): Change the values of the color components.
        x += 0.3f * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_E)) {
        // TODO(student): Change the values of the color components.
        y -= 0.3f * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_Q)) {
        // TODO(student): Change the values of the color components.
        y += 0.3f * deltaTime;
    }
}


void Lab1::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_F) {
        // TODO(student): Change the values of the color components.
        color = 1;
    }

    if (key == GLFW_KEY_T) {
        // TODO(student): Change the values of the color components.
        if (mesh == "box")
            mesh = "teapot";
        else if (mesh == "teapot")
            mesh = "sphere";
        else if (mesh == "sphere")
            mesh = "box";
    }

    // TODO(student): Add a key press event that will let you cycle
    // through at least two meshes, rendered at the same position.
    // You will also need to generalize the mesh name used by `RenderMesh`.

}


void Lab1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
    x_2 += (deltaX)*0.01f;
    y_2 -= (deltaY)*0.01f;
}


void Lab1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
    // Treat mouse scroll event
}


void Lab1::OnWindowResize(int width, int height)
{
    // Treat window resize event
}
