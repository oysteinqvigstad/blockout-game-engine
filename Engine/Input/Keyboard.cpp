#include "Keyboard.h"


Keyboard::Keyboard(int key, int ms)
                : m_keyboardKey(key),
                  m_millisecondRepeatMax(ms),
                  m_millisecondRepeatTimer(0.0f),
                  m_secondsLastPressed(0.0f) {
}


bool Keyboard::isPressed(GLFWwindow *window) {

    // update time
    auto secondsElapsedTime = static_cast<float>(glfwGetTime());
    m_millisecondRepeatTimer -= (secondsElapsedTime - m_secondsLastPressed) * 1000;
    m_secondsLastPressed = secondsElapsedTime;

    // check if key should be triggered
    auto isPressed = (glfwGetKey(window, m_keyboardKey) == GLFW_PRESS);
    auto shouldRepeat = m_millisecondRepeatTimer < 0.0f;

    if (isPressed && shouldRepeat) {
        m_millisecondRepeatTimer = static_cast<float>(m_millisecondRepeatMax);
        return true;
    } else if (!isPressed) {
        m_millisecondRepeatTimer = 0.0f; // key has been released
    }
    return false;
}

