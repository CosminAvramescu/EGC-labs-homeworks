#include "lab_m1/lab5/lab5.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab5::Lab5()
{
}


Lab5::~Lab5()
{
}


void Lab5::Init()
{
    renderCameraTarget = false;

    camera = new implemented::Camera();
    camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("teapot");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "teapot.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // TODO(student): After you implement the changing of the projection
    // parameters, remove hardcodings of these parameters
    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
    //projectionMatrix = glm::ortho(-10.f, 10.0f, -10.0f, 10.0f, 0.0f, 100.0f);
}


void Lab5::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Lab5::Update(float deltaTimeSeconds)
{

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(45.0f), glm::vec3(0, 1, 0));

        // Attention! The RenderMesh function overrides the usual RenderMesh that we used until now
        // It uses the viewMatrix from Laborator::Camera instance and the local projectionMatrix
        RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(2, 0.5f, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(60.0f), glm::vec3(1, 0, 0));
        RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-2, 0.5f, 0));
        RenderMesh(meshes["box"], shaders["Simple"], modelMatrix);
    }

    // Render the camera target. Useful for understanding where is the rotation point in Third-person camera movement
    if (renderCameraTarget)
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
        RenderMesh(meshes["sphere"], shaders["VertexNormal"], modelMatrix);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(7, 0, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(1.5f));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(30.0f), glm::vec3(0, 1, 1));
        RenderMesh(meshes["teapot"], shaders["Simple"], modelMatrix);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(10, 0, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(2.0f));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(30.0f), glm::vec3(0, 1, 1));
        RenderMesh(meshes["teapot"], shaders["VertexNormal"], modelMatrix);
    }
}


void Lab5::FrameEnd()
{
    DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}


void Lab5::RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab5::OnInputUpdate(float deltaTime, int mods)
{
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float cameraSpeed = 2.0f;

        if (window->KeyHold(GLFW_KEY_W)) {
            // TODO : translate the camera forward
            camera->TranslateForward(deltaTime * cameraSpeed);
        }

        if (window->KeyHold(GLFW_KEY_A)) {
            // TODO : translate the camera to the left
            camera->TranslateRight(-deltaTime * cameraSpeed);
        }

        if (window->KeyHold(GLFW_KEY_S)) {
            // TODO : translate the camera backwards
            camera->TranslateForward(-deltaTime * cameraSpeed);
        }

        if (window->KeyHold(GLFW_KEY_D)) {
            // TODO : translate the camera to the right
            camera->TranslateRight(deltaTime * cameraSpeed);
        }

        if (window->KeyHold(GLFW_KEY_Q)) {
            // TODO : translate the camera down
            camera->TranslateUpward(-deltaTime * cameraSpeed);
        }

        if (window->KeyHold(GLFW_KEY_E)) {
            // TODO : translate the camera up
            camera->TranslateUpward(deltaTime * cameraSpeed);
        }

        if (window->KeyHold(GLFW_KEY_O)) {
            orthoLeft = -8.0f;
            orthoRight = 8.0f;
            orthoUp = 4.5f;
            orthoDown = -4.5;
            projectionMatrix = glm::ortho(orthoLeft, orthoRight, orthoDown, orthoUp, 0.0f, 200.0f);
            isOrtho = true;
        }

        // inversare FOV
        if (window->KeyHold(GLFW_KEY_L)) {
            projectionMatrix = glm::perspective(-RADIANS(60), -2.0f, 0.01f, -200.0f);
            isOrtho = false;
        }
        // FOV normal
        if (window->KeyHold(GLFW_KEY_P))
        {
             projectionMatrix = glm::perspective(RADIANS(60), 2.0f, 0.01f, 200.0f);
            isOrtho = false;
        }

        // increase height
        if (window->KeyHold(GLFW_KEY_UP) && isOrtho) {
            orthoUp += deltaTime;
            orthoDown -= deltaTime;
            projectionMatrix = glm::ortho(orthoLeft, orthoRight, orthoDown, orthoUp, 0.0f, 200.0f);
        }

        // increase height
        if (window->KeyHold(GLFW_KEY_DOWN) && isOrtho) {
            orthoUp -= deltaTime;
            orthoDown += deltaTime;
            projectionMatrix = glm::ortho(orthoLeft, orthoRight, orthoDown, orthoUp, 0.0f, 200.0f);
        }

        // increase length
        if (window->KeyHold(GLFW_KEY_RIGHT) && isOrtho) {
            orthoRight += deltaTime;
            orthoLeft -= deltaTime;
            projectionMatrix = glm::ortho(orthoLeft, orthoRight, orthoDown, orthoUp, 0.0f, 200.0f);
        }

        // decrease length
        if (window->KeyHold(GLFW_KEY_LEFT) && isOrtho) {
            orthoRight -= deltaTime;
            orthoLeft += deltaTime;
            projectionMatrix = glm::ortho(orthoLeft, orthoRight, orthoDown, orthoUp, 0.0f, 200.0f);
        }
    }
}


void Lab5::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_T)
    {
        renderCameraTarget = !renderCameraTarget;
    }
    // TODO(student): Switch projections

}


void Lab5::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab5::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event

    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;

        if (window->GetSpecialKeyState() == 0) {
            renderCameraTarget = false;
            // TODO(student): Rotate the camera in first-person mode around
            // OX and OY using `deltaX` and `deltaY`. Use the sensitivity
            // variables for setting up the rotation speed.
            camera->RotateFirstPerson_OX(-2 * sensivityOX * deltaY);
            camera->RotateFirstPerson_OY(-2 * sensivityOY * deltaX);
        }

        if (window->GetSpecialKeyState() & GLFW_MOD_CONTROL) {
            renderCameraTarget = true;
            // TODO(student): Rotate the camera in third-person mode around
            // OX and OY using `deltaX` and `deltaY`. Use the sensitivity
            // variables for setting up the rotation speed.
            camera->RotateThirdPerson_OX(-2 * sensivityOX * deltaY);
            camera->RotateThirdPerson_OY(2 * sensivityOY * deltaX);
        }
    }
}


void Lab5::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab5::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab5::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Lab5::OnWindowResize(int width, int height)
{
}
