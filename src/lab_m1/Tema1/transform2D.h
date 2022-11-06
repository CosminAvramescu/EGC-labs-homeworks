#pragma once

#include "utils/glm_utils.h"

namespace transform2D
{
    int rotation = 0;
    // Translate matrix
    inline glm::mat3 Translate(float translateX, float translateY)
    {
        // TODO(student): Implement the translation matrix
        return glm::transpose(glm::mat3(1, 0, translateX,
            0, 1, translateY,
            0, 0, 1));

    }

    // Scale matrix
    inline glm::mat3 Scale(float scaleX, float scaleY)
    {
        // TODO(student): Implement the scaling matrix
        return glm::transpose(glm::mat3(scaleX, 0, 0,
            0, scaleY, 0,
            0, 0, 1));

    }

    // Rotate matrix
    inline glm::mat3 Rotate(float radians)
    {
        // TODO(student): Implement the rotation matrix
        return glm::transpose(glm::mat3(cos(radians), -sin(radians), 0,
            sin(radians), cos(radians), 0,
            0, 0, 1));

    }

    inline void MoveDuck(const std::string& name, int *direction3, glm::mat3 *modelMatrix,
        float deltaTimeSeconds, float* translateX, float* translateY, int speedX, int speedY)
    {
        switch (*direction3)
        {
        case 1: //dreapta-sus
            *translateX += deltaTimeSeconds * speedX;
            *translateY += deltaTimeSeconds * speedY;
            if (*translateX > 1050) {
                //rotation += 100 * deltaTimeSeconds;
                //(*modelMatrix) *= transform2D::Rotate(rotation);
                *direction3 = 3;
            }
            else if (*translateY > 680) {
                *direction3 = 2;
            }
            break;
        case 2: //dreapta-jos
            *translateX += deltaTimeSeconds * speedX;
            *translateY -= deltaTimeSeconds * speedY;
            if (*translateX > 1050) {
                *direction3 = 4;
            }
            else if (*translateY < 180) {
                *direction3 = 1;
            }
            break;
        case 3: //stanga-sus
            *translateX -= deltaTimeSeconds * speedX;
            *translateY += deltaTimeSeconds * speedY;
            if (*translateX < -10) {
                *direction3 = 1;
            }
            else if (*translateY > 680) {
                *direction3 = 4;
            }
            break;
        case 4: //stanga-jos
            *translateX -= deltaTimeSeconds * speedX;
            *translateY -= deltaTimeSeconds * speedY;
            if (*translateX < -10) {
                *direction3 = 2;
            }
            else if (*translateY < 180) {
                *direction3 = 3;
            }
            break;
        default:
            break;
        }

        (* modelMatrix) *= transform2D::Translate(*translateX, *translateY);
    }

    inline void KillDuck(const std::string& name, glm::mat3* modelMatrix, float deltaTimeSeconds, float* translateX, float* translateY, glm::ivec2 resolution)
    {
        *translateY -= deltaTimeSeconds * (resolution.y / 20);
        (*modelMatrix) *= transform2D::Translate(*translateX, *translateY);
    }

    inline void EscapeDuck(const std::string& name, glm::mat3* modelMatrix, float deltaTimeSeconds, float* translateX, float* translateY, glm::ivec2 resolution)
    {
        *translateY += deltaTimeSeconds * (resolution.y / 20);
        (*modelMatrix) *= transform2D::Translate(*translateX, *translateY);
    }
}   // namespace transform2D
