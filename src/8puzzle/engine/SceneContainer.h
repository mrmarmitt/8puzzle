#pragma once

#include <functional>
#include <memory>
#include <stdexcept>
#include <unordered_map>

#include "IScene.h"

enum class StateEnum;

class SceneContainer {
    std::unordered_map<std::string, std::unique_ptr<IScene>> m_scenes;
    std::unordered_map<std::string, std::function<std::unique_ptr<IScene>()>> m_factories;

    SceneContainer() = default;
public:
    SceneContainer(const SceneContainer&) = delete;
    SceneContainer& operator=(const SceneContainer&) = delete;

    static SceneContainer& get() {
        static SceneContainer instance;
        return instance;
    }

    void registerFactory(const std::string& name, std::function<std::unique_ptr<IScene>()> factory) {
        m_factories[name] = std::move(factory);
    }

    IScene& getScene(const std::string& name) {
        // Se já estiver instanciada, retorna
        const auto it = m_scenes.find(name);
        if (it != m_scenes.end()) {
            return *(it->second);
        }

        // Caso contrário, instancia sob demanda via factory
        const auto factoryIt = m_factories.find(name);
        if (factoryIt != m_factories.end()) {
            m_scenes[name] = factoryIt->second();
            return *(m_scenes[name]);
        }

        throw std::runtime_error("Scene not found: " + name);
    }

    void unloadScene(const std::string& name) {
        m_scenes.erase(name);
    }

    void unloadAll() {
        m_scenes.clear();
    }
};