//
// Created by oyqv on 12.12.22.
//

#include "ActiveBlock.h"
#include <GeometricTools.h>
#include <random>
#include <RenderCommands.h>

void ActiveBlock::generate() {
    posx = 1; posy = 1; posz = 8;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                tiles[i][j][k] = false;
            }
        }
    }
    tiles[0][0][1] = true;
    tiles[0][1][0] = true;
    tiles[0][1][1] = true;
    tiles[1][1][0] = true;
}

ActiveBlock::ActiveBlock()
    : cube(GeometricTools::UnitCube3D24WNormals,
           GeometricTools::UnitCube3DTopologyTriangles24,
            BufferLayout({{ShaderDataType::Float3, "position"},
                         {ShaderDataType::Float3, "normals"}})) {
    generate();
}

void ActiveBlock::draw(const std::shared_ptr<Shader> & shader) {
        RenderCommands::disableGLDepthTesting();
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                for (int k = 0; k < 3; k++) {
                    if (tiles[i][j][k]) {
                        cube.setTranslation({posx+k, posy+j, posz-i});
                        cube.draw(shader);
                    }
                }
            }
        }
        RenderCommands::enableGLDepthTesting();
}

void ActiveBlock::move(int x, int y, int z) {
    posx += x;
    posy += y;
    posz += z;
    if (checkWallCollision()) {
        posx -= x;
        posy -= y;
        posz -= z;
    }
}

bool ActiveBlock::checkWallCollision() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                if (tiles[i][j][k])
                    if (k + posx > 4 || k + posx < 0 || j + posy > 4 || j + posy < 0)
                        return true;
            }
        }
    }
    return false;
}

bool ActiveBlock::goDown(bool (*squares)[5][5], const bool completely) {
    posz--;
    if (hasCollided(squares)) {
        posz++;
        addBlockToBoard(squares);
        // update board
        return true;
    }

    return false;
}

bool ActiveBlock::hasCollided(bool (*squares)[5][5]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                if (tiles[i][j][k])
                    if (posz-i < 0 || squares[posz-i][posy+j][posx+k]) {
                        addBlockToBoard(squares);
                        return true;
                    }
            }
        }
    }
    return false;
}

void ActiveBlock::addBlockToBoard(bool (*squares)[5][5]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                if (tiles[i][j][k])
                    squares[posz-i+1][posy+j][posx+k] = true;
            }
        }
    }
    generate();
}
