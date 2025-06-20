#pragma once

#include <functional>
#include <memory>
#include <stdexcept>

#include "IScreen.h"

class ScreenCache final {
    using CreatorFunc = std::function<std::unique_ptr<IScreen>()>;

    std::unordered_map<GameState, std::unique_ptr<IScreen>> m_cache;
    std::unordered_map<GameState, CreatorFunc> m_creators;

public:
    ScreenCache() = default;
    ~ScreenCache() = default;

    void registerScreen(const GameState& key, CreatorFunc creator) {
        m_creators[key] = std::move(creator);
    }

    IScreen& getScreen(const GameState& key) {
        const auto cachedScreenIterator = m_cache.find(key);
        if (cachedScreenIterator != m_cache.end()) {
            return *cachedScreenIterator->second;
        }

        auto creatorIterator = m_creators.find(key);
        if (creatorIterator == m_creators.end()) {
            throw std::out_of_range("No screen found for key: " + key);
        }

        std::unique_ptr<IScreen> screen = creatorIterator->second();
        IScreen* ptr = screen.get();
        m_cache[key] = std::move(screen);
        return *ptr;
    }

    void clear() {
        m_cache.clear();
    }
};