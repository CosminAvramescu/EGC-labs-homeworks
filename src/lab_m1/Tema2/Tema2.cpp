#include "lab_m1/Tema2/Tema2.h"

#include <vector>
#include <string>
#include <iostream>

#include "lab_m1/Tema2/transform3D.h"

using namespace std;
using namespace m1;


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}


void Tema2::Init()
{
	camera = new implemented::Camera2();
	camera->Set(glm::vec3(28, 1, 0), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
	camera->RotateThirdPerson_OY(RADIANS(-90));

	orthoCamera = new implemented::Camera2();
	orthoCamera->Set(glm::vec3(0, 5, 2.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

	polygonMode = GL_FILL;

	angularStepOY = 0;

	Mesh* mesh = new Mesh("plane");
	mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
	meshes[mesh->GetMeshID()] = mesh;

	vector<VertexFormat> crv
	{
		VertexFormat(glm::vec3(24.93, 0,  1.42), glm::vec3(0, 0, 0)),
		VertexFormat(glm::vec3(22.18, 0, 6.15), glm::vec3(0, 0, 0)),
		VertexFormat(glm::vec3(16.62, 0, 9.5), glm::vec3(0, 0, 0)),
		VertexFormat(glm::vec3(10.06, 0, 12.47), glm::vec3(0, 0, 0)),
		VertexFormat(glm::vec3(3.28, 0, 10.57), glm::vec3(0, 0, 0)),
		VertexFormat(glm::vec3(-2.82, 0, 5.08), glm::vec3(0, 0, 0)),
		VertexFormat(glm::vec3(-8.01, 0, 7.6), glm::vec3(0, 0, 0)),
		VertexFormat(glm::vec3(-14.03, 0, 9.2), glm::vec3(0, 0, 0)),
		VertexFormat(glm::vec3(-19.29, 0, 5.46), glm::vec3(0, 0, 0)),
		VertexFormat(glm::vec3(-20.21, 0, -1.63), glm::vec3(0, 0, 0)),
		VertexFormat(glm::vec3(-16.93, 0, -5.9), glm::vec3(0, 0, 0)),
		VertexFormat(glm::vec3(-12.35, 0, -8.42), glm::vec3(0, 0, 0)),
		VertexFormat(glm::vec3(-5, 0, -10), glm::vec3(0, 0, 0)),
		VertexFormat(glm::vec3(2.21, 0, -7.12), glm::vec3(0, 0, 0)),
		VertexFormat(glm::vec3(7.93, 0, -2.85), glm::vec3(0, 0, 0)),
		VertexFormat(glm::vec3(12.81, 0, -5.29), glm::vec3(0, 0, 0)),
		VertexFormat(glm::vec3(18.45, 0, -7.2), glm::vec3(0, 0, 0)),
		VertexFormat(glm::vec3(23.02, 0, -4.98), glm::vec3(0, 0, 0))
	};

	int noOfPoints = 100;
	for (int j = 0; j < crv.size(); j++) {
		glm::vec3 P1 = crv[j].position;
		glm::vec3 P2;
		if (j + 1 == crv.size())
			P2 = crv[0].position;
		else
			P2 = crv[j + 1].position;
		glm::vec3 D = (P2 - P1);
		for (int k = 1; k <= noOfPoints; k++) {
			centerRoadVertices.push_back(P1 + (float)k * D / (float)noOfPoints);
		}
	}

	for (int i = 0; i < centerRoadVertices.size(); i++) {
		glm::vec3 P1 = centerRoadVertices[i].position;
		glm::vec3 P2;
		if (i + 1 == centerRoadVertices.size())
			P2 = centerRoadVertices[0].position;
		else
			P2 = centerRoadVertices[i + 1].position;
		glm::vec3 D = (P2 - P1);
		glm::vec3 P = glm::cross(D, glm::vec3(0, 1, 0));
		interiorRoadVertices.push_back(VertexFormat(P1 - 30.0f * P, glm::vec3(0, 0, 0)));
		exteriorRoadVertices.push_back(VertexFormat(P1 + 0.001f * P, glm::vec3(0, 0, 0)));
	}

	for (int i = 0; i < exteriorRoadVertices.size(); i++) {
		roadVertices.push_back(exteriorRoadVertices[i]);
	}
	for (int i = 0; i < interiorRoadVertices.size(); i++) {
		roadVertices.push_back(interiorRoadVertices[i]);
	}

	int a = 0, b = 1, c = 1800;
	for (int i = 0; i < 1800; i++) {
		roadIndices.push_back(a);
		roadIndices.push_back(b);
		roadIndices.push_back(c);
		a = b;
		b = c;
		if (i == 1799) {
			c = 1800;
		}
		else
			c = c + 1;
		roadIndices.push_back(a);
		roadIndices.push_back(b);
		roadIndices.push_back(c);
		if (i == 1798) {
			a = b;
			b = 0;
			c = 3599;
		}
		else
			b = a + 1;
	}
	CreateMesh("road", roadVertices, roadIndices);

	vector<VertexFormat> carVertices
	{
		VertexFormat(glm::vec3(0, 0,  0.25), glm::vec3(0.2f, 0.2f, 1)),
		VertexFormat(glm::vec3(0.25, 0, 0.25), glm::vec3(0.2f, 0.2f, 1)),
		VertexFormat(glm::vec3(0, 0.25, 0.25), glm::vec3(0.2f, 0.2f, 1)),
		VertexFormat(glm::vec3(0.25, 0.25, 0.25), glm::vec3(0.2f, 0.2f, 1)),
		VertexFormat(glm::vec3(0, 0, -0.25),glm::vec3(0.2f, 0.2f, 1)),
		VertexFormat(glm::vec3(0.25, 0, -0.25), glm::vec3(0.2f, 0.2f, 1)),
		VertexFormat(glm::vec3(0, 0.25, -0.25), glm::vec3(0.2f, 0.2f, 1)),
		VertexFormat(glm::vec3(0.25, 0.25, -0.25), glm::vec3(0.2f, 0.2f, 1))
	};

	vector<VertexFormat> enemiesCarVertices
	{
		VertexFormat(glm::vec3(0, 0,  0.25), glm::vec3(1, 0.2f, 0.2f)),
		VertexFormat(glm::vec3(0.25, 0, 0.25), glm::vec3(1, 0.2f, 0.2f)),
		VertexFormat(glm::vec3(0, 0.25, 0.25), glm::vec3(1, 0.2f, 0.2f)),
		VertexFormat(glm::vec3(0.25, 0.25, 0.25), glm::vec3(1, 0.2f, 0.2f)),
		VertexFormat(glm::vec3(0, 0, -0.25),glm::vec3(1, 0.2f, 0.2f)),
		VertexFormat(glm::vec3(0.25, 0, -0.25), glm::vec3(1, 0.2f, 0.2f)),
		VertexFormat(glm::vec3(0, 0.25, -0.25), glm::vec3(1, 0.2f, 0.2f)),
		VertexFormat(glm::vec3(0.25, 0.25, -0.25), glm::vec3(1, 0.2f, 0.2f))
	};

	vector<unsigned int> carIndices =
	{
		0, 1, 2,
		1, 3, 2,
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
	CreateMesh("car", carVertices, carIndices); 
	CreateMesh("enemiesCar", enemiesCarVertices, carIndices);

	vector<VertexFormat> treeVertices
	{
		VertexFormat(glm::vec3(0, 0,  0.25), glm::vec3(0.3f, 0.1f, 0.0f)),
		VertexFormat(glm::vec3(0.25, 0, 0.25), glm::vec3(0.3f, 0.1f, 0.0f)),
		VertexFormat(glm::vec3(0, 0.25, 0.25), glm::vec3(0.3f, 0.1f, 0.0f)),
		VertexFormat(glm::vec3(0.25, 0.25, 0.25), glm::vec3(0.3f, 0.1f, 0.0f)),
		VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0.3f, 0.1f, 0.0f)),
		VertexFormat(glm::vec3(0.25, 0, 0), glm::vec3(0.3f, 0.1f, 0.0f)),
		VertexFormat(glm::vec3(0, 0.25, 0), glm::vec3(0.3f, 0.1f, 0.0f)),
		VertexFormat(glm::vec3(0.25, 0.25, 0), glm::vec3(0.3f, 0.1f, 0.0f)),

		VertexFormat(glm::vec3(-0.25, 0.25, -0.25), glm::vec3(0.18f, 0.3f, 0.18f)),
		VertexFormat(glm::vec3(0.5, 0.25, -0.25), glm::vec3(0.18f, 0.3f, 0.18f)),
		VertexFormat(glm::vec3(0.5, 0.25, 0.5), glm::vec3(0.18f, 0.3f, 0.18f)),
		VertexFormat(glm::vec3(-0.25, 0.25, 0.5), glm::vec3(0.18f, 0.3f, 0.18f)),

		VertexFormat(glm::vec3(-0.25, 0.75, -0.25), glm::vec3(0.18f, 0.3f, 0.18f)),
		VertexFormat(glm::vec3(0.5, 0.75, -0.25), glm::vec3(0.18f, 0.3f, 0.18f)),
		VertexFormat(glm::vec3(0.5, 0.75, 0.5), glm::vec3(0.18f, 0.3f, 0.18f)),
		VertexFormat(glm::vec3(-0.25, 0.75, 0.5), glm::vec3(0.18f, 0.3f, 0.18f)),

	};

	vector<unsigned int> treeIndices =
	{
		0, 1, 2,
		1, 3, 2,
		2, 3, 7,
		2, 7, 6,
		1, 7, 3,
		1, 5, 7,
		6, 7, 4,
		7, 5, 4,
		0, 4, 1,
		1, 4, 5,
		2, 6, 4,
		0, 2, 4,

		8, 9, 12,
		9, 12, 13,
		8, 11, 15,
		8, 15, 12,
		11, 10, 14,
		11, 14, 15,
		10, 9, 14,
		9, 14, 13,
		15, 14, 13,
		15, 12, 13,
		11, 10, 9,
		11, 9, 8
	};
	CreateMesh("tree", treeVertices, treeIndices);

	// Sets the resolution of the small viewport
	glm::ivec2 resolution = window->GetResolution();
	miniViewportArea = ViewportArea(resolution.x - 50 - resolution.x / 5.f, 50, resolution.x / 5.f, resolution.y / 5.f);

	projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
	orthoProjectionMatrix = glm::ortho(orthoLeft, orthoRight, orthoDown, orthoUp, 0.0f, 200.0f);

	Shader* shader = new Shader("Tema2Shader");
	shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
	shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
	shader->CreateAndLink();
	shaders[shader->GetName()] = shader;

	for (int i = 0; i < 6; i++) {
		enemiesIndices.push_back(i * 250);
		enemiesPosition.push_back(rand() % 24 + 6);
		enemiesCenter.push_back(glm::vec3(0, 0, 0));
	}
}

void Tema2::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices)
{
	unsigned int VAO = 0;
	// TODO(student): Create the VAO and bind it
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO = 0;
	// TODO(student): Create the VBO and bind it
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// TODO(student): Send carVertices data into the VBO buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	unsigned int IBO = 0;
	// TODO(student): Create the IBO and bind it
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	// TODO(student): Send carIndices data into the IBO buffer
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
	meshes[name]->vertices = vertices;
	meshes[name]->indices = indices;
}

bool Tema2::SameSide(glm::vec3 p1, glm::vec3 p2, glm::vec3  a, glm::vec3  b) {
	glm::vec3 cp1 = glm::cross(b - a, p1 - a);
	glm::vec3 cp2 = glm::cross(b - a, p2 - a);
	if (dot(cp1, cp2) >= 0)
		return true;
	else
		return false;
}

bool Tema2::PointInTriangle(glm::vec3 p, glm::vec3 a, glm::vec3  b, glm::vec3 c) {
	if (SameSide(p, a, b, c) && SameSide(p, b, a, c) && SameSide(p, c, a, b))
		return true;
	else
		return false;
}

void Tema2::FrameStart()
{
	// Clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0.2f, 0.2f, 0.8f, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Sets the screen area where to draw
	glm::ivec2 resolution = window->GetResolution();
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::RenderScene(float deltaTimeSeconds, int ortho) {
	modelMatrix = glm::mat4(1);
	modelMatrix *= transform3D::Translate(0, -0.04f, 0);
	modelMatrix *= transform3D::Scale(2, 0, 2);
	RenderMesh("plane", meshes["plane"], shaders["Tema2Shader"], modelMatrix, ortho);

	modelMatrix = glm::mat4(1);
	modelMatrix *= transform3D::Translate(camera->GetTargetPosition().r, camera->GetTargetPosition().g - 1, camera->GetTargetPosition().b);
	modelMatrix *= transform3D::RotateOY(angularStepOY);
	RenderMesh("car", meshes["car"], shaders["Tema2Shader"], modelMatrix, ortho);


	for (int i = 0; i < 6; i++) {
		glm::vec3 P1 = centerRoadVertices[enemiesIndices[i]].position;
		glm::vec3 P2;
		P2 = centerRoadVertices[enemiesIndices[i] + 1].position;
		glm::vec3 D = (P2 - P1);
		glm::vec3 P = glm::cross(D, glm::vec3(0, 1, 0));

		modelMatrix = glm::mat4(1);
		enemiesCenter[i]=glm::vec3(P1.r - enemiesPosition[i] * P.r, 0, P1.b - enemiesPosition[i] * P.b);
		modelMatrix *= transform3D::Translate(P1.r - enemiesPosition[i] * P.r, 0, P1.b - enemiesPosition[i] * P.b);
		if (D.r > 0 && D.b > 0) {
			modelMatrix *= transform3D::RotateOY(-acos(glm::dot(glm::normalize(D), glm::vec3(0, 0, -1))));
		}
		else if (D.r > 0 && D.b < 0) {
			modelMatrix *= transform3D::RotateOY(acos(glm::dot(glm::normalize(D), glm::vec3(0, 0, 1))));
		}
		else if (D.r < 0 && D.b < 0) {
			modelMatrix *= transform3D::RotateOY(-acos(glm::dot(glm::normalize(D), glm::vec3(0, 0, 1))));
		}
		else if (D.r < 0 && D.b > 0) {
			modelMatrix *= transform3D::RotateOY(acos(glm::dot(glm::normalize(D), glm::vec3(0, 0, -1))));
		}
		RenderMesh("car", meshes["enemiesCar"], shaders["Tema2Shader"], modelMatrix, ortho);
		enemiesIndices[i]++;
		if (enemiesIndices[i] == centerRoadVertices.size() - 1)
			enemiesIndices[i] = 0;
	}

	for (int i = 0; i < interiorRoadVertices.size(); i = i + 50) {
		glm::vec3 P1t = centerRoadVertices[i].position;
		glm::vec3 P2t;
		if (i + 1 == centerRoadVertices.size())
			P2t = centerRoadVertices[0].position;
		else
			P2t = centerRoadVertices[i + 1].position;
		glm::vec3 Dt = (P2t - P1t);
		glm::vec3 Pt = glm::cross(Dt, glm::vec3(0, 1, 0));

		modelMatrix = glm::mat4(1);
		modelMatrix *= transform3D::Translate(P1t.r + 10.0f * Pt.r, -0.04f, P1t.b + 10.0f * Pt.b);
		RenderMesh("tree", meshes["tree"], shaders["Tema2Shader"], modelMatrix, ortho);
	}

	modelMatrix = glm::mat4(1);
	modelMatrix *= transform3D::Translate(0, 0, 0);
	RenderMesh("road", meshes["road"], shaders["Tema2Shader"], modelMatrix, ortho);
}

void Tema2::RenderMesh(string type, Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, int ortho)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// Render an object using the specified shader and the specified position
	glUseProgram(shader->program);
	if (ortho == 0) {
		glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
		glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	}
	else {
		glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(orthoCamera->GetViewMatrix()));
		glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(orthoProjectionMatrix));
	}
	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	int location = glGetUniformLocation(shader->program, "Car");

	glUniform3fv(location, 1, glm::value_ptr(camera->GetTargetPosition()));
	location = glGetUniformLocation(shader->program, "setColor");
	if (type == "plane") {
		glUniform1i(location, 1);
	}
	else {
		glUniform1i(location, 0);
	}
	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->m_VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
	mesh->Render();
}

void Tema2::Update(float deltaTimeSeconds)
{
	glLineWidth(3);
	glPointSize(5);
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
	RenderScene(deltaTimeSeconds, 0);

	glViewport(miniViewportArea.x, miniViewportArea.y, miniViewportArea.width, miniViewportArea.height);
	orthoCamera->position.r = camera->GetTargetPosition().r;
	orthoCamera->position.b = camera->GetTargetPosition().b;
	RenderScene(deltaTimeSeconds, 1);
}

void Tema2::FrameEnd()
{

}

bool Tema2::intersect(glm::vec3 c1, glm::vec3 c2, float R1, float R2) {
	// we are using multiplications because it's faster than calling Math.pow
	float distance = sqrt(
		(c1.r - c2.r) * (c1.r - c2.r) +
		(c1.g - c2.g) * (c1.g - c2.g) +
		(c1.b - c2.b) * (c1.b - c2.b)
	);
	return distance < R1 + R2;
}

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
	float cameraSpeed = 2.0f;
	float sensivityOX = 0.01f;
	float sensivityOY = 0.01f;
	// TODO(student): Add transformation logic
	if (window->KeyHold(GLFW_KEY_W)) {
		glm::vec3 dir = glm::normalize(glm::vec3(camera->forward.r, 0, camera->forward.b));
		for (int i = 0; i < roadIndices.size(); i = i + 3) {
			if (PointInTriangle(camera->GetTargetPosition() + dir * cameraSpeed * deltaTime,
				roadVertices[roadIndices[i]].position,
				roadVertices[roadIndices[i + 1]].position,
				roadVertices[roadIndices[i + 2]].position)) {
				colision = false;
				for (int j = 0; j < 6; j++) {
					if (intersect(glm::vec3(camera->GetTargetPosition().r, 0, camera->GetTargetPosition().b), enemiesCenter[j], 0.25, 0.25)) {
						colision = true;
						break;
					}
				}
				if (!colision) {
					camera->MoveForward(cameraSpeed * deltaTime);
					break;
				}
			}
		}
	}
	if (window->KeyHold(GLFW_KEY_D)) {
		colision = false;
		for (int j = 0; j < 6; j++) {
			if (intersect(glm::vec3(camera->GetTargetPosition().r, 0, camera->GetTargetPosition().b), enemiesCenter[j], 0.25, 0.25)) {
				colision = true;
				break;
			}
		}
		if (!colision) {
			camera->RotateThirdPerson_OY(-deltaTime);
			angularStepOY -= deltaTime;
		}

	}
	if (window->KeyHold(GLFW_KEY_A)) {
		colision = false;
		for (int j = 0; j < 6; j++) {
			if (intersect(glm::vec3(camera->GetTargetPosition().r, 0, camera->GetTargetPosition().b), enemiesCenter[j], 0.25, 0.25)) {
				colision = true;
				break;
			}
		}
		if (!colision) {
			camera->RotateThirdPerson_OY(deltaTime);
			angularStepOY += deltaTime;
		}
	}
	if (window->KeyHold(GLFW_KEY_S)) {
		colision = false;
		for (int j = 0; j < 6; j++) {
			if (intersect(glm::vec3(camera->GetTargetPosition().r, 0, camera->GetTargetPosition().b), enemiesCenter[j], 0.25, 0.25)) {
				colision = true;
				break;
			}
		}
		if (!colision) {
			camera->MoveForward(-cameraSpeed * deltaTime);
		}
	}
}


void Tema2::OnKeyPress(int key, int mods)
{
	// Add key press event
}


void Tema2::OnKeyRelease(int key, int mods)
{
	// Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// Add mouse move event
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// Add mouse button press event
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
