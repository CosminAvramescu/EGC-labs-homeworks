#pragma once

#include <iostream>
#include "utils/glm_utils.h"
#include "utils/math_utils.h"
using namespace std;

namespace implemented
{
    class Camera2
    {
     public:
        Camera2()
        {
            position    = glm::vec3(0, 2, 5);
            forward     = glm::vec3(0, 0, -1);
            up          = glm::vec3(0, 1, 0);
            right       = glm::vec3(1, 0, 0);
            center = glm::vec3(0, 1, 0);
            distanceToTarget = 2.5;
        }

        Camera2(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up)
        {
            Set(position, center, up);
        }

        ~Camera2()
        { }

        // Update Camera2
        void Set(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up)
        {
            this->position = position;
            this->forward     = glm::normalize(center - position);
            this->right       = glm::cross(forward, up);
            this->up    = glm::cross(right, forward);
            this->center = center;
        }

        void MoveForward(float distance)
        {
            // Translates the Camera2 using the `dir` vector computed from
            // `forward`. Movement will always keep the Camera2 at the same
            // height. For example, if you rotate your head up/down, and then
            // walk forward, then you will still keep the same relative
            // distance (height) to the ground!
            glm::vec3 dir = glm::normalize(glm::vec3(forward.x, 0, forward.z));
            position += dir * distance;
        }

        void TranslateForward(float distance)
        {
            // TODO(student): Translate the Camera2 using the `forward` vector.
            // What's the difference between `TranslateForward()` and
            // `MoveForward()`?
            position += glm::normalize(forward) * distance;
        }

        void TranslateUpward(float distance)
        {
            // TODO(student): Translate the Camera2 using the `up` vector.
            position += distance * glm::normalize(up);
        }

        void TranslateRight(float distance)
        {
            // TODO(student): See instructions below. Read the entire thing!
            // You need to translate the Camera2 using the `right` vector.
            // Usually, however, translation using Camera2's `right` vector
            // is not very useful, because if the Camera2 is rotated around the
            // `forward` vector, then the translation on the `right` direction
            // will have an undesired effect, more precisely, the Camera2 will
            // get closer or farther from the ground. The solution is to
            // actually use the projected `right` vector (projected onto the
            // ground plane), which makes more sense because we will keep the
            // same distance from the ground plane.
            position += distance * glm::normalize(right);
        }

        void RotateFirstPerson_OX(float angle)
        {
            // TODO(student): Compute the new `forward` and `up` vectors.
            // Don't forget to normalize the vectors! Use `glm::rotate()`.
            forward = glm::normalize(glm::rotate(glm::mat4(1.f), angle, right) * glm::vec4(forward, 0));
            up = glm::normalize(glm::cross(right, forward));
        }

        void RotateFirstPerson_OY(float angle)
        {
            // TODO(student): Compute the new `forward`, `up` and `right`
            // vectors. Use `glm::rotate()`. Don't forget to normalize the
            // vectors!
            //std::cout<< forward<<" ";
            this->forward = glm::normalize(glm::rotate(glm::mat4(1.f), angle, glm::vec3(0, 1, 0)) * glm::vec4(forward, 0));
            //std::cout << forward << endl;
            right = glm::normalize(glm::rotate(glm::mat4(1.f), angle, glm::vec3(0, 1, 0)) * glm::vec4(right, 0));
            up = glm::normalize(glm::cross(right, forward));
        }

        void RotateFirstPerson_OZ(float angle)
        {
            // TODO(student): Compute the new `right` and `up`. This time,
            // `forward` stays the same. Use `glm::rotate()`. Don't forget
            // to normalize the vectors!
            right = glm::normalize(glm::rotate(glm::mat4(1.f), angle, forward) * glm::vec4(right, 0));
            up = glm::normalize(glm::rotate(glm::mat4(1.f), angle, forward) * glm::vec4(up, 0));
            forward = glm::cross(up, right);
        }

        void RotateThirdPerson_OX(float angle)
        {
            // TODO(student): Rotate the Camera2 in third-person mode around
            // the OX axis. Use `distanceToTarget` as translation distance.
            TranslateForward(distanceToTarget);
            RotateFirstPerson_OX(angle);
            TranslateForward(-distanceToTarget);
        }

        void RotateThirdPerson_OY(float angle)
        {
            // TODO(student): Rotate the Camera2 in third-person mode around
            // the OY axis.
            TranslateForward(distanceToTarget);
            RotateFirstPerson_OY(angle);
            TranslateForward(-distanceToTarget);
        }

        void RotateThirdPerson_OZ(float angle)
        {
            // TODO(student): Rotate the Camera2 in third-person mode around
            // the OZ axis.
            TranslateForward(distanceToTarget);
            RotateFirstPerson_OZ(angle);
            TranslateForward(-distanceToTarget);
        }

        glm::mat4 GetViewMatrix()
        {
            // Returns the view matrix
            return glm::lookAt(position, position + forward, up);
        }

        glm::vec3 GetTargetPosition()
        {
            return position + forward * distanceToTarget;
        }

     public:
        float distanceToTarget;
        glm::vec3 position;
        glm::vec3 forward;
        glm::vec3 right;
        glm::vec3 up;
        glm::vec3 center;
    };
}   // namespace implemented
