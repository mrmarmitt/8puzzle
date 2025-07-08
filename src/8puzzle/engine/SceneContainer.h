#pragma once

#include <memory>
#include <stdexcept>
#include <unordered_map>

enum class StateEnum;
class IScene;
class SceneContainer {
    std::unordered_map<std::string, IScene*> m_scenes;

    SceneContainer() = default;
public:
    SceneContainer(const SceneContainer&) = delete;
    SceneContainer& operator=(const SceneContainer&) = delete;

    static SceneContainer& get() {
        static SceneContainer instance;
        return instance;
    }

    void registerScene(const std::string& name, IScene* scene) {
        m_scenes[name] = scene;
    }

    IScene& getScene(const std::string& name) {
        auto it = m_scenes.find(name);

        if (it != m_scenes.end()) {
            return *(it->second);
        }
        throw std::runtime_error("Screen does not exist");
    }
};