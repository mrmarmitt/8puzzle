#pragma once

#include <memory>
#include <stdexcept>
#include <unordered_map>

enum class StateEnum;
class IScreen;
class ScreenContainer {
    std::unordered_map<std::string, IScreen*> m_screens;

    ScreenContainer() = default;
public:
    ScreenContainer(const ScreenContainer&) = delete;
    ScreenContainer& operator=(const ScreenContainer&) = delete;

    static ScreenContainer& get() {
        static ScreenContainer instance;
        return instance;
    }

    void registerScreen(const std::string& name, IScreen* screen) {
        m_screens[name] = screen;
    }

    IScreen& getScreen(const std::string& name) {
        auto it = m_screens.find(name);

        if (it != m_screens.end()) {
            return *(it->second);
        }
        throw std::runtime_error("Screen does not exist");
    }
};