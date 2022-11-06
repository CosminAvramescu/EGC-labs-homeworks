#include "lab_m1/Tema1/Tema1.h"

#include <vector>
#include <iostream>

#include "lab_m1/Tema1/transform2D.h"
#include "lab_m1/Tema1/object2D.h"

using namespace std;
using namespace m1;

Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
	resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	glm::vec3 corner = glm::vec3(0, 0, 0);

	// TODO(student): Compute coordinates of a square's center, and store
	// then in the `cx` and `cy` class variables (see the header). Use
	// `corner` and `squareSide`. These two class variables will be used
	// in the `Update()` function. Think about it, why do you need them?
	cx = corner.x + squareSide / 2;
	cy = corner.y + squareSide / 2;

	// Initialize tx and ty (the translation steps)
	translateX = 0;
	translateY = 0;

	// Initialize sx and sy (the scale factors)
	scaleX = 1;
	scaleY = 1.5;

	// Initialize angularStep
	angularStep = 13.8;
	angularStep2 = 15.3;

	Mesh* body = object2D::CreateTriangle("body", corner, squareSide, glm::vec3(0.3f, 0.1f, 0.0f));
	AddMeshToList(body);

	Mesh* leftWing = object2D::CreateTriangle("leftWing", corner, squareSide, glm::vec3(0.3f, 0.1f, 0.0f));
	AddMeshToList(leftWing);

	Mesh* head = object2D::CreateCircle("head", corner, squareSide, glm::vec3(0, 0.3f, 0.2f));
	AddMeshToList(head);

	Mesh* rightWing = object2D::CreateTriangle("rightWing", corner, squareSide, glm::vec3(0.3f, 0.1f, 0.0f));
	AddMeshToList(rightWing);

	Mesh* beak = object2D::CreateTriangle("beak", corner, squareSide, glm::vec3(0.7f, 0.7f, 0.2f));
	AddMeshToList(beak);

	Mesh* sky = object2D::CreateSquare("sky", corner, squareSide, glm::vec3(0.3f, 0.5f, 0.7f));
	AddMeshToList(sky);

	Mesh* earth = object2D::CreateSquare("earth", corner, squareSide, glm::vec3(0.3f, 0.5f, 0.2f));
	AddMeshToList(earth);

	Mesh* life = object2D::CreateCircle("life", corner, squareSide, glm::vec3(1, 0.1f, 0.1f));
	AddMeshToList(life);

	Mesh* bullet = object2D::CreateSquare("bullet", corner, squareSide, glm::vec3(0.1f, 0.7f, 0.1f));
	AddMeshToList(bullet);

	Mesh* wireframe = object2D::CreateSquare("wireframe", corner, squareSide, glm::vec3(1, 0.7f, 0.3f), false);
	AddMeshToList(wireframe);

	Mesh* score = object2D::CreateSquare("score", corner, squareSide, glm::vec3(0.4f, 0.3f, 1));
	AddMeshToList(score);
}


void Tema1::FrameStart()
{
	// Clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// Sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}


void Tema1::Update(float deltaTimeSeconds)
{
	if (incDuck) {
		printf("%d\n", ducksNo);
		ducksNo++;
		incDuck = false;
		if (ducksNo % 5 == 0) {
			speedX += (ducksNo / 5) + (speedX / 5);
			speedY += (ducksNo / 5) + (speedY / 5);
		}
	}
	duckTime += deltaTimeSeconds;
	if (duckTime > 5) {
		if (!kill) {
			escape = true;
			lifeNo--;
		}
		duckTime = 0;
	}
	if (bulletNo == 0) {
		if (bulletNo == 0 && !kill) {
			escape = true;
		}
		if (!kill) {
			lifeNo--;
		}
		bulletNo = 3;
	}
	if (kill || escape) {
		duckTime = 0;
	}

	modelMatrix = glm::mat3(1);
	modelMatrix *= transform2D::Translate(0, 0);
	RenderMesh2D(meshes["earth"], shaders["VertexColor"], modelMatrix);

	modelMatrix = glm::mat3(1);
	modelMatrix *= transform2D::Translate(1200, 600);
	RenderMesh2D(meshes["wireframe"], shaders["VertexColor"], modelMatrix);
	modelMatrix = glm::mat3(1);
	modelMatrix *= transform2D::Translate(1200, 600);
	if (kill == true) {
		modelMatrix *= transform2D::Scale(scaleX, scaleY);
	}
	RenderMesh2D(meshes["score"], shaders["VertexColor"], modelMatrix);


	for (int i = 0; i < lifeNo; i++) {
		modelMatrix = glm::mat3(1);
		modelMatrix *= transform2D::Translate(40 + i * 60, 680);
		RenderMesh2D(meshes["life"], shaders["VertexColor"], modelMatrix);
	}


	for (int i = 0; i < bulletNo; i++) {
		modelMatrix = glm::mat3(1);
		modelMatrix *= transform2D::Translate(20 + i * 30, 600);
		RenderMesh2D(meshes["bullet"], shaders["VertexColor"], modelMatrix);
	}

	modelMatrix = glm::mat3(1);
	modelMatrix *= transform2D::Translate(2 * squareSide + randomPositionX, squareSide / 3 + randomPositionY);

	if (kill == true) {
		transform2D::KillDuck("head", &modelMatrix, deltaTimeSeconds, &translateX, &translateY, resolution);
	}
	else if (escape == true) {
		transform2D::EscapeDuck("head", &modelMatrix, deltaTimeSeconds, &translateX, &translateY, resolution);
	}
	else {
		transform2D::MoveDuck("head", &direction3, &modelMatrix, deltaTimeSeconds, &translateX, &translateY, speedX, speedY, randomPositionX, randomPositionY);
	}
	RenderMesh2D(meshes["head"], shaders["VertexColor"], modelMatrix);


	modelMatrix = glm::mat3(1);
	modelMatrix *= transform2D::Translate(squareSide / 2 + randomPositionX, squareSide / 3 + randomPositionY);
	if (kill == true) {
		transform2D::KillDuck("leftWing", &modelMatrix, deltaTimeSeconds, &translateX, &translateY, resolution);
	}
	else if (escape == true) {
		transform2D::EscapeDuck("leftWing", &modelMatrix, deltaTimeSeconds, &translateX, &translateY, resolution);
	}
	else {
		transform2D::MoveDuck("leftWing", &direction3, &modelMatrix, deltaTimeSeconds, &translateX, &translateY, speedX, speedY, randomPositionX, randomPositionY);
	}

	modelMatrix *= transform2D::Translate(squareSide * 0.7, 0);
	if (direction2 == true) {
		angularStep2 -= deltaTimeSeconds;
		modelMatrix *= transform2D::Rotate(1.3 * angularStep2);
		if (angularStep2 < 14.4)
			direction2 = false;
	}
	else if (direction2 == false) {
		angularStep2 += deltaTimeSeconds * 2;
		modelMatrix *= transform2D::Rotate(1.3 * angularStep2);
		if (angularStep2 > 14.7)
			direction2 = true;
	}
	modelMatrix *= transform2D::Translate(-squareSide * 0.7, 0);

	RenderMesh2D(meshes["leftWing"], shaders["VertexColor"], modelMatrix);


	modelMatrix = glm::mat3(1);
	modelMatrix *= transform2D::Translate(squareSide / 2 + randomPositionX, squareSide / 3 + randomPositionY);

	if (kill == true) {
		transform2D::KillDuck("rightWing", &modelMatrix, deltaTimeSeconds, &translateX, &translateY, resolution);
	}
	else if (escape == true) {
		transform2D::EscapeDuck("rightWing", &modelMatrix, deltaTimeSeconds, &translateX, &translateY, resolution);
	}
	else {
		transform2D::MoveDuck("rightWing", &direction3, &modelMatrix, deltaTimeSeconds, &translateX, &translateY, speedX, speedY, randomPositionX, randomPositionY);
	}
	modelMatrix *= transform2D::Translate(squareSide * 0.7, 0);

	if (direction == true) {
		;       angularStep -= deltaTimeSeconds * 2;
		modelMatrix *= transform2D::Rotate(1.3 * angularStep);
		if (angularStep < 14.4)
			direction = false;
	}
	else if (direction == false) {
		angularStep += deltaTimeSeconds;
		modelMatrix *= transform2D::Rotate(1.3 * angularStep);
		if (angularStep > 14.7)
			direction = true;
	}
	modelMatrix *= transform2D::Translate(-squareSide * 0.7, 0);

	RenderMesh2D(meshes["rightWing"], shaders["VertexColor"], modelMatrix);

	modelMatrix = glm::mat3(1);
	modelMatrix *= transform2D::Translate(2 * squareSide + squareSide / 3 + randomPositionX, squareSide / 3.5 + randomPositionY);

	if (kill == true) {
		transform2D::KillDuck("beak", &modelMatrix, deltaTimeSeconds, &translateX, &translateY, resolution);
	}
	else if (escape == true) {
		transform2D::EscapeDuck("beak", &modelMatrix, deltaTimeSeconds, &translateX, &translateY, resolution);
	}
	else {
		transform2D::MoveDuck("beak", &direction3, &modelMatrix, deltaTimeSeconds, &translateX, &translateY, speedX, speedY, randomPositionX, randomPositionY);
	}

	RenderMesh2D(meshes["beak"], shaders["VertexColor"], modelMatrix);

	modelMatrix = glm::mat3(1);
	modelMatrix *= transform2D::Translate(randomPositionX, randomPositionY);

	if (kill == true) {
		transform2D::KillDuck("body", &modelMatrix, deltaTimeSeconds, &translateX, &translateY, resolution);
		if (translateY < 80) {
			translateX = 0;
			translateY = 0;
			kill = false;
			randomPositionX = rand() % 1100 + 100;
			randomPositionY = rand() % 100;
			direction3 = rand() % 2 == 0 ? 1 : 3;
			incDuck = true;
		}
	}
	else if (escape == true) {
		transform2D::EscapeDuck("body", &modelMatrix, deltaTimeSeconds, &translateX, &translateY, resolution);
		if (translateY > 800) {
			translateX = 0;
			translateY = 0;
			escape = false;
			randomPositionX = rand() % 1100 + 100;
			randomPositionY = rand() % 200;
			direction3 = rand() % 2 == 0 ? 1 : 3;
			incDuck = true;
		}
	}
	else {
		transform2D::MoveDuck("body", &direction3, &modelMatrix, deltaTimeSeconds, &translateX, &translateY, speedX, speedY, randomPositionX, randomPositionY);
	}

	RenderMesh2D(meshes["body"], shaders["VertexColor"], modelMatrix);

	modelMatrix = glm::mat3(1);
	modelMatrix *= transform2D::Translate(0, 200);
	RenderMesh2D(meshes["sky"], shaders["VertexColor"], modelMatrix);
}


void Tema1::FrameEnd()
{
}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
}


void Tema1::OnKeyPress(int key, int mods)
{

}


void Tema1::OnKeyRelease(int key, int mods)
{

}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{

}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	bulletNo--;
	if (mouseX <= translateX + randomPositionX + ((2 * 100) + 100 / 2) && mouseX >= translateX + randomPositionX) {
		if (window->GetResolution().y - mouseY <= (100 / 2) + translateY + randomPositionY && window->GetResolution().y - mouseY >= translateY + randomPositionY) {
			if (!escape) {
				kill = true;
			}
		}
	}
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{

}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}
