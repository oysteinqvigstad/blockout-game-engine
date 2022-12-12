//
// Created by oyqv on 12.12.22.
//

#include "ActiveBlock.h"
#include <GeometricTools.h>
#include <random>
#include <RenderCommands.h>

void ActiveBlock::generate() {
    posx = 1; posy = 1; posz = 9;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                tiles[i][j][k] = false;
            }
        }
    }
    switch (rand() % 5) {
        case 0:
            tiles[0][0][1] = true;     //
            tiles[0][1][1] = true;     //    ##
            tiles[0][1][2] = true;     //    #
            break;
        case 1:
            tiles[0][0][1] = true;    //    #
            tiles[0][1][1] = true;    //    #
            break;
        case 2:
            tiles[0][0][1] = true;    //
            tiles[0][1][1] = true;    //    #
            tiles[0][1][0] = true;    //    ##
            tiles[0][2][0] = true;    //     #
            break;
        case 3:
            tiles[0][1][1] = true;    //    #
            break;
        case 4:
            tiles[0][0][1] = true;    //
            tiles[0][0][2] = true;    //    #
            tiles[0][1][1] = true;    //    #
            tiles[0][2][1] = true;    //    ##
            break;

    }

}

ActiveBlock::ActiveBlock()
    : cube(GeometricTools::UnitCube3D24WNormals,
           GeometricTools::UnitCube3DTopologyTriangles24,
            BufferLayout({{ShaderDataType::Float3, "position"},
                         {ShaderDataType::Float3, "normals"}})) {
    freeze = false;
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

void ActiveBlock::moveSideways(const bool (*squares)[5][5], int x, int y) {
    if (!freeze) {
        posx += x;
        posy += y;
        if (checkWallCollision() || hasCollided(squares)) {
            posx -= x;
            posy -= y;
        }
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

bool ActiveBlock::goDown(bool (*squares)[5][5], float &timer, const bool completely) {
    timer = 0.0f;
    if (!freeze) {
        do {
            posz--;
            if (hasCollided(squares)) {
                addBlockToBoard(squares);
                return true;
            }
        } while (completely);
    }
    return false;
}

bool ActiveBlock::hasCollided(const bool (*squares)[5][5]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                if (tiles[i][j][k])
                    if (posz-i < 0 || squares[posz-i][posy+j][posx+k]) {
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
    if (hasCollided(squares))
        freeze = true;
}
