#include "object2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"

double radius = 0.2;
double ori_x = 0.0;                         // the origin or center of circle
double ori_y = 0.0;

Mesh* object2D::CreateSquare(
	const std::string& name,
	glm::vec3 leftBottomCorner,
	float length,
	glm::vec3 color,
	bool fill)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
	};
	if (name == "sky") {
		vertices.push_back(VertexFormat(corner, color));
		vertices.push_back(VertexFormat(corner + glm::vec3(1920, 0, 0), color));
		vertices.push_back(VertexFormat(corner + glm::vec3(1920, 1080, 0), color));
		vertices.push_back(VertexFormat(corner + glm::vec3(0, 1080, 0), color));
	}
	if (name == "earth") {
		vertices.push_back(VertexFormat(corner, color));
		vertices.push_back(VertexFormat(corner + glm::vec3(1920, 0, 0), color));
		vertices.push_back(VertexFormat(corner + glm::vec3(1920, 200, 0), color));
		vertices.push_back(VertexFormat(corner + glm::vec3(0, 200, 0), color));
	}
	if (name == "bullet") {
		vertices.push_back(VertexFormat(corner, color));
		vertices.push_back(VertexFormat(corner + glm::vec3(15, 0, 0), color));
		vertices.push_back(VertexFormat(corner + glm::vec3(15, 30, 0), color));
		vertices.push_back(VertexFormat(corner + glm::vec3(0, 30, 0), color));
	}
	if (name == "wireframe") {
		vertices.push_back(VertexFormat(corner, color));
		vertices.push_back(VertexFormat(corner + glm::vec3(50, 0, 0), color));
		vertices.push_back(VertexFormat(corner + glm::vec3(50, 100, 0), color));
		vertices.push_back(VertexFormat(corner + glm::vec3(0, 100, 0), color));
	}
	if (name == "score") {
		vertices.push_back(VertexFormat(corner, color));
		vertices.push_back(VertexFormat(corner + glm::vec3(50, 0, 0), color));
		vertices.push_back(VertexFormat(corner + glm::vec3(50, 30, 0), color));
		vertices.push_back(VertexFormat(corner + glm::vec3(0, 30, 0), color));
	}

	Mesh* square = new Mesh(name);
	std::vector<unsigned int> indices = { 0, 1, 2, 3 };

	if (!fill) {
		square->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		// Draw 2 triangles. Add the remaining 2 indices
		indices.push_back(0);
		indices.push_back(2);
	}

	square->InitFromData(vertices, indices);
	return square;
}

Mesh* object2D::CreateTriangle(
	const std::string& name,
	glm::vec3 leftBottomCorner,
	float length,
	glm::vec3 color,
	bool fill)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{

	};
	if (name == "body") {
		vertices.push_back(VertexFormat(corner, color));
		vertices.push_back(VertexFormat(corner + glm::vec3(0, length/1.5, 0), color));
		vertices.push_back(VertexFormat(corner + glm::vec3(2 * length, length / 3, 0), color));
	}
	else if (name == "rightWing") {
		vertices.push_back(VertexFormat(corner + glm::vec3(length, 0, 0), color));
		vertices.push_back(VertexFormat(corner, color));
		vertices.push_back(VertexFormat(corner + glm::vec3(length / 4, -0.8 * length, 0), color));
	}
	else if (name == "leftWing") {
		vertices.push_back(VertexFormat(corner + glm::vec3(length, 0, 0), color));
		vertices.push_back(VertexFormat(corner, color));
		vertices.push_back(VertexFormat(corner + glm::vec3(length / 4, 0.8 * length, 0), color));
	}
	else if (name == "beak") {
		vertices.push_back(VertexFormat(corner + glm::vec3(0, length / 8, 0), color));
		vertices.push_back(VertexFormat(corner, color));
		vertices.push_back(VertexFormat(corner + glm::vec3(length / 2.5, length / 16, 0), color));
	}

	Mesh* triangle = new Mesh(name);
	std::vector<unsigned int> indices = { 0, 1, 2 };

	if (!fill) {
		triangle->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		// Draw 2 triangles. Add the remaining 2 indices
		//indices.push_back(0);
		//indices.push_back(2);
	}

	triangle->InitFromData(vertices, indices);
	return triangle;
}

Mesh* object2D::CreateCircle(
	const std::string& name,
	glm::vec3 leftBottomCorner,
	float length,
	glm::vec3 color,
	bool fill)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{

	};
	vertices.push_back(VertexFormat(corner + glm::vec3(0, 0, 0), color));

	if (name == "head") {
		for (int i = 0; i < 360; i++) {
			vertices.push_back(VertexFormat(corner + glm::vec3(length / 2.5 * sin(i), length / 2.5 * cos(i), 0), color));
		}
	}
	if (name == "life") {
		for (int i = 0; i < 360; i++) {
			vertices.push_back(VertexFormat(corner + glm::vec3(length / 4 * sin(i), length / 4 * cos(i), 0), color));
		}
	}


	Mesh* circle = new Mesh(name);
	std::vector<unsigned int> indices = { };

	int last_i = 1;
	for (int i = 0; i < 360; i++) {
		indices.push_back(0);
		indices.push_back(last_i);
		indices.push_back(last_i + 1);
		last_i = i + 2;
	}
	indices.push_back(0);
	indices.push_back(360);
	indices.push_back(1);

	if (!fill) {
		circle->SetDrawMode(GL_LINE_LOOP);
	}
	else {

	}

	circle->InitFromData(vertices, indices);
	return circle;
}



