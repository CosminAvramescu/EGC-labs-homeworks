#pragma once

#include "components/simple_scene.h"


namespace m1
{
	class Tema1 : public gfxc::SimpleScene
	{
	public:
		Tema1();
		~Tema1();

		void Init() override;

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;

	protected:
		float cx, cy;
		glm::mat3 modelMatrix;
		float translateX, translateY;
		float scaleX, scaleY;
		float angularStep1, angularStep2;
		bool direction1 = false, direction2 = true;
		int direction3 = 3;
		int bulletNo = 3, lifeNo = 3;
		float shapeSide = 100;
		glm::ivec2 resolution;
		bool kill = false;
		bool escape = false;
		float duckTime = 0;
		int ducksNo = 0;
		bool incDuck = true;
		float speedX = 60, speedY = 40;
		int randomPositionX = 0, randomPositionY = 0;
		glm::mat3 lastScore;
		float initialScore = 10;
	};
}   // namespace m1
