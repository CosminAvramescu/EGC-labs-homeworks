#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema2/lab_camera.h"


namespace m1
{
	class Tema2 : public gfxc::SimpleScene
	{
	public:
		vector<glm::vec3> enemiesCenter;
 		vector<int> enemiesIndices, enemiesPosition;
		int index = 0;
		vector<VertexFormat> centerRoadVertices;
		vector<unsigned int> roadIndices;
		vector<VertexFormat> roadVertices;
		vector<VertexFormat> interiorRoadVertices, exteriorRoadVertices;
		float orthoRight = 8.0f, orthoUp = 4.5f, orthoDown = -4.5f, orthoLeft = -8.0f;
		struct ViewportArea
		{
			ViewportArea() : x(0), y(0), width(1), height(1) {}
			ViewportArea(int x, int y, int width, int height)
				: x(x), y(y), width(width), height(height) {}
			int x;
			int y;
			int width;
			int height;
		};

		Tema2();
		~Tema2();

		void Init() override;

	private:
		void CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices);
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		bool Tema2::intersect(glm::vec3 c1, glm::vec3 c2, float R1, float R2);
		bool SameSide(glm::vec3 p1, glm::vec3 p2, glm::vec3 a, glm::vec3 b);
		bool PointInTriangle(glm::vec3 p, glm::vec3 a, glm::vec3 b, glm::vec3 c);
		void RenderMesh(string type, Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, int ortho);
		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;

		void RenderScene(float deltaTimeSeconds, int ortho);
	protected:
		glm::mat4 modelMatrix;
		float angularStepOY;
		GLenum polygonMode;
		ViewportArea miniViewportArea;

		implemented::Camera2* camera, * orthoCamera, * aux;
		glm::mat4 projectionMatrix, p, orthoProjectionMatrix;
		bool ok = true, colision = false;
	};
}   // namespace m1

