#ifndef PROG2002_GEOMETRICTOOLS_H
#define PROG2002_GEOMETRICTOOLS_H

#include <array>
#include <list>
#include <stdexcept>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

namespace GeometricTools {
    glm::mat4 calcTransformMatrix(glm::vec3 scale, glm::vec3 rotate,
                                  float degrees, glm::vec3 translate);

    struct chessPiece {
        glm::ivec2 squareNumber;
        glm::vec3 color;
        bool highlighted;
        [[nodiscard]] glm::vec3 getTranslationMatrix() const {
            return {static_cast<float>(squareNumber.x) - 3.5f,
                    0.25f + 0.01f, 3.5f - static_cast<float>(squareNumber.y)};
        }
        [[nodiscard]] glm::vec3 getColor(glm::ivec2 &selectorPos) const {
            if (highlighted)
                return {1.0f, 0.0f, 1.0f};
            else if (squareNumber == selectorPos)
                return {0.0f, 1.0f, 0.0f};
            else
                return color;
        }
    };

    template <int x, int y>
    std::list<chessPiece> createPieces() {
        std::list<chessPiece> list;
        for (int i = 0; i < x; i++) {
            list.push_back({{i, 0}, {1.0f, 0.0f, 0.0f}, false});
            list.push_back({{i, 1}, {1.0f, 0.0f, 0.0f}, false});
            list.push_back({{i, y-1}, {0.0f, 0.0f, 1.0f}, false});
            list.push_back({{i, y-2}, {0.0f, 0.0f, 1.0f}, false});
        }
        return list;
    }

    template <int x, int y>
    constexpr std::array<float, (x+1)*(y+1)*8> UnitGridGeometry3DWTCoords() {
        std::array<float, (x+1)*(y+1)*8> board;
        int index = 0;          // Index in the board
        float xdiv = 1.0f / x;  // 1 unit of X-pos resolution
        float ydiv = 1.0f / y;  // 1 unit of Y-pos resolution
        float norm = -0.5f;     // move {0, 1} to {-0.5, 0.5}
        for (int ypos = 0; ypos <= y; ypos++) {
            for (int xpos = 0; xpos <= x; xpos++) {
                // Position in x and y coordinate
                board[index++] = static_cast<float>(xpos) * xdiv + norm; // X
                board[index++] = static_cast<float>(ypos) * ydiv + norm; // Y
                board[index++] = 0.0f;                                   // Z
                // Alternate color (black/white)
                float color = ((ypos+xpos) % 2) ? 0.0f: 1.0f;
                board[index++] = color;                                  // R
                board[index++] = color;                                  // G
                board[index++] = color;                                  // B
                board[index++] = static_cast<float>(xpos) * xdiv;        // S
                board[index++] = static_cast<float>(ypos) * ydiv;        // T
            }
        }
        return board;
    }





    constexpr std::array<float, 3*(3+3+2)> TextureTriangle {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
             0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
             0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
    };



    constexpr std::array<float, 3*2> UnitTriangle2D = {
            -0.5f, -0.5f, 0.5f, -0.5f, 0.0f, 0.5f
    };

    constexpr std::array<float, 4*5> UnitSquare2D = {
            -0.5f, -0.5f, 0.0f, 0.5f, 0.0f,
             0.5f, -0.5f, 0.0f, 0.5f, 0.0f,
             0.5f,  0.5f, 0.0f, 0.5f, 0.0f,
            -0.5f,  0.5f, 0.0f, 0.5f, 0.0f

    };

    constexpr std::array<GLuint, 6> UnitSquareTopologyTriangles = {
            0, 1, 2, 2, 3, 0
    };

    constexpr std::array<float, 3*24*2> UnitCube3D24WNormals = {

            // front facing vertices
            -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,  // bottom left
             0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,  // bottom right
            -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,  // top left
             0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,  // top right

            // right facing vertices
            0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,  // bottom left
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,  // bottom right
            0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,  // top left
            0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f,  // top right

            // back facing vertices
            0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,  // bottom left
           -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,  // bottom right
            0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,  // top left
           -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,  // top right

            // left facing vertices
           -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,  // bottom left
           -0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f,  // bottom right
           -0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f,  // top left
           -0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f,  // top right

             // top facing vertices
            -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f,  // bottom left
             0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f,  // bottom right
            -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  // top left
             0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  // top right

             // bottom facing vertices
            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,  // bottom left
             0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,  // bottom right
            -0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f,  // top left
             0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f,  // top right
    };

    constexpr std::array<GLuint, 6*3*2> UnitCube3DTopologyTriangles24 = {
             0,  1,  3,  0,  2,  3,
             4,  5,  7,  4,  6,  7,
             8,  9, 11,  8, 10, 11,
            12, 13, 15, 12, 14, 15,
            16, 17, 19, 16, 18, 19,
            20, 21, 23, 20, 22, 23
    };




    constexpr std::array<float, 8*6> UnitCube3D = {
            -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,
             0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f,
             0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,
             0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f,
             0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f
    };

    constexpr std::array<GLuint, 6*6> UnitCubeTopology {
            1, 0, 4, 5, 1, 4,
            1, 2, 5, 2, 6, 5,
            2, 3, 6, 7, 3, 6,
            3, 0, 7, 4, 0, 7,
            0, 1, 3, 1, 2, 3,
            4, 5, 7, 5, 6, 7

    };



    template <size_t size>
    constexpr void scale2D(std::array<float, size> & figure, float x, float y) {
        for (int i = 0; i < size; i += 5) {
            figure[i] *= x;
            figure[i + 1] *= y;
        }
    }

    template <size_t size>
    constexpr void translate2D(std::array<float, size> & figure, float x, float y) {
        for (int i = 0; i < size; i += 5) {
            figure[i] += x;
            figure[i + 1] += y;
        }
    }

    template <int x, int y>
    std::array<float, (x+1)*(y+1)*5> UnitGridGeometry2D() {
        std::array<float, (x+1)*(y+1)*5> board;
        int index = 0;          // Index in the board
        float xdiv = 1.0f / x;  // 1 unit of X-pos resolution
        float ydiv = 1.0f / y;  // 1 unit of Y-pos resolution
        float norm = -0.5f;     // move {0, 1} to {-0.5, 0.5}
        for (int ypos = 0; ypos <= y; ypos++) {
            for (int xpos = 0; xpos <= x; xpos++) {
                // Position in x and y coordinate
                board[index++] = static_cast<float>(xpos) * xdiv + norm;
                board[index++] = static_cast<float>(ypos) * ydiv + norm;
                // Alternate color (black/white)
                float color = ((ypos+xpos) % 2) ? 0.0f: 1.0f;
                board[index++] = color;
                board[index++] = color;
                board[index++] = color;
            }
        }
        return board;
    }

    template <int x, int y>
    std::array<float, (x+1)*(y+1)*6> UnitGridGeometry3D() {
        std::array<float, (x+1)*(y+1)*6> board;
        int index = 0;          // Index in the board
        float xdiv = 1.0f / x;  // 1 unit of X-pos resolution
        float ydiv = 1.0f / y;  // 1 unit of Y-pos resolution
        float norm = -0.5f;     // move {0, 1} to {-0.5, 0.5}
        for (int ypos = 0; ypos <= y; ypos++) {
            for (int xpos = 0; xpos <= x; xpos++) {
                // Position in x and y coordinate
                board[index++] = static_cast<float>(xpos) * xdiv + norm;
                board[index++] = static_cast<float>(ypos) * ydiv + norm;
                board[index++] = 0.0f;
                // Alternate color (black/white)
                float color = ((ypos+xpos) % 2) ? 0.0f: 1.0f;
                board[index++] = color;
                board[index++] = color;
                board[index++] = color;
            }
        }
        return board;
    }

    template <int x, int y>
    constexpr std::array<int, x*y*6> UnitGridTopologyTriangles() {
        std::array<int, x*y*6> board;
        int index = 0;
        for (int ypos = 0; ypos < y; ypos++) {
            for (int xpos = 0; xpos < x; xpos++) {

                board[index++] = (ypos+1)*(x+1) + (xpos+0); // 2
                board[index++] = (ypos+0)*(x+1) + (xpos+0); // 0
                board[index++] = (ypos+0)*(x+1) + (xpos+1); // 1
                board[index++] = (ypos+1)*(x+1) + (xpos+0); // 3
                board[index++] = (ypos+1)*(x+1) + (xpos+1); // 4
                board[index++] = (ypos+0)*(x+1) + (xpos+1); // 5
            }
        }
        return board;
    }



}


#endif //PROG2002_GEOMETRICTOOLS_H
