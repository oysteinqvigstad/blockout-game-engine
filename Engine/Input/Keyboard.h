
#ifndef PROG2002_KEYBOARD_H
#define PROG2002_KEYBOARD_H


#include <GLFW/glfw3.h>

class Keyboard {
private:
    int m_keyboardKey;
    int m_millisecondRepeatMax;
    float m_millisecondRepeatTimer;
    float m_secondsLastPressed;
public:
    explicit Keyboard(int key, int ms = 0);
    bool isPressed(GLFWwindow*);

};





#endif //PROG2002_KEYBOARD_H
