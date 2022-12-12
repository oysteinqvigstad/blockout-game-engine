#include "GeometricTools.h"

namespace GeometricTools {
    glm::mat4 calcTransformMatrix(const glm::vec3 scale, const glm::vec3 rotate,
                        const float degrees, glm::vec3 translate) {
        auto matrix = glm::translate(glm::mat4(1.0f), translate);
        matrix = glm::rotate(matrix, glm::radians(degrees), rotate);
        matrix = glm::scale(matrix, scale);
        return matrix;
    }
}