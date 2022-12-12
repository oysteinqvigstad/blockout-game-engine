//
// Created by oyqv on 24.11.22.
//

#include "LightManager.h"
#include <ErrorHandler.h>
#include <algorithm>

void LightManager::addSpotLight(const Spotlight & light) {
        if (m_spotlights.size() == m_maxNumberOfLights)
            raiseException("Unable to add more lights to light-manager. Reason: full");
        m_spotlights.push_back(light);

}
LightManager::Spotlight* LightManager::getSpotLight(const std::string &str) {
    auto spot = std::find_if(m_spotlights.begin(), m_spotlights.end(), [&str](const auto &light){
        return str == light.m_name;
    });
    return (spot != m_spotlights.end()) ? &(*spot) : nullptr;
}

void LightManager::moveLight(const std::string & name, const glm::vec3 displacement) {
    auto spotlight = getSpotLight(name);
    if (spotlight)
        spotlight->m_position += displacement;
    else
        raiseException("no lights defined by that name");
}

void LightManager::setPosition(const std::string &name, glm::vec3 newPos) {
    auto spotlight = getSpotLight(name);
    if (spotlight)
        spotlight->m_position = newPos;
    else
        raiseException("no lights defined by that name");
}
