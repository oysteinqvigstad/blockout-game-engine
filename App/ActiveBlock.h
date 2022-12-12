//
// Created by oyqv on 12.12.22.
//

#ifndef PROG2002_ACTIVEBLOCK_H
#define PROG2002_ACTIVEBLOCK_H


#include <Model.h>

class ActiveBlock {
private:
    int posx, posy, posz;
    char color;
    bool tiles[3][3][3];

    Model cube;


    bool checkWallCollision();
public:
    ActiveBlock();
    void generate();
    void move(int x, int y, int z);
    bool goDown(bool squares[10][5][5], bool completely = false);
    void draw(const std::shared_ptr<Shader> &shader);
    bool hasCollided(bool squares[10][5][5]);
    void addBlockToBoard(bool squares[10][5][5]);

};


#endif //PROG2002_ACTIVEBLOCK_H
