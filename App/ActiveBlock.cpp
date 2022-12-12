//
// Created by oyqv on 12.12.22.
//

#include "ActiveBlock.h"
#include <GeometricTools.h>
#include <random>
#include <RenderCommands.h>

void ActiveBlock::generate() {
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
    posx = 1; posy = 1; posz = 8;
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
                    if (k + posx > 4 || k + posx < 0 || j + posy > 4 || j + posy < 0 || posz-i < 0)
                        return true;
            }
        }
    }
    return false;
}