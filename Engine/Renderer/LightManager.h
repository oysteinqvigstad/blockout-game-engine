
#ifndef PROG2002_LIGHTMANAGER_H
#define PROG2002_LIGHTMANAGER_H

#include <glm/glm.hpp>
#include <utility>
#include <string>
#include <vector>

class LightManager {
public:
    struct Spotlight {
        std::string m_name;
        glm::vec3 m_position;
        glm::vec3 m_ambient;
        glm::vec3 m_diffuse;
        glm::vec3 m_specular;
        float m_constant;
        float m_linear;
        float m_quadratic;
        Spotlight(std::string name, glm::vec3 pos, float constant, float linear, float quad,
                  glm::vec3 amb = {1.0, 1.0, 1.0}, glm::vec3 diff = {1.0, 1.0, 1.0},
                  glm::vec3 spec = {1.0, 1.0, 1.0})
                : m_name(std::move(name)), m_position(pos), m_ambient(amb), m_diffuse(diff),
                  m_specular(spec), m_constant(constant), m_linear(linear), m_quadratic(quad) {};
    };

private:
    inline static LightManager* m_instance = nullptr;
    unsigned int m_maxNumberOfLights = 10;
    std::vector<Spotlight> m_spotlights;

public:
    static LightManager* GetInstance() {
        return m_instance != nullptr ? m_instance : m_instance = new LightManager();
    }

    void addSpotLight(const Spotlight & light);
    Spotlight* getSpotLight(const std::string & str);
    void moveLight(const std::string &name, glm::vec3 displacement);
    void setPosition(const std::string &name, glm::vec3 newPos);


};


#endif //PROG2002_LIGHTMANAGER_H
