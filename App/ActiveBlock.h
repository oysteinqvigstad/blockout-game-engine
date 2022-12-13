//
// Created by oyqv on 12.12.22.
//

#ifndef PROG2002_ACTIVEBLOCK_H
#define PROG2002_ACTIVEBLOCK_H


#include <Model.h>

class ActiveBlock {
private:
    int posx, posy, posz;
    bool tiles[3][3][3];
    bool freeze;
    Model cube;


    bool checkWallCollision();
public:
    ActiveBlock();
    void generate();
    void moveSideways(const bool squares[10][5][5], int x, int y);
    bool goDown(bool squares[10][5][5], float &timer, bool completely = false);
    void draw(const std::shared_ptr<Shader> &shader, bool wireframe = false);
    bool hasCollided(const bool squares[10][5][5]);
    void addBlockToBoard(bool squares[10][5][5]);

};


#endif //PROG2002_ACTIVEBLOCK_H
