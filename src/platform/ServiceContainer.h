#pragma once
#include <memory>

#include "8puzzle/service/ConfigurationService.h"
#include "8puzzle/service/GamePlayService.h"
#include "8puzzle/service/RecordService.h"

class ServiceContainer {
    std::shared_ptr<RecordService> m_recordService;
    std::shared_ptr<ConfigurationService> m_configService;
    std::shared_ptr<GamePlayService> m_gamePlayService;

    ServiceContainer()
        : m_recordService(std::make_shared<RecordService>()),
          m_configService(std::make_shared<ConfigurationService>()),
          m_gamePlayService(std::make_shared<GamePlayService>())
    {}

public:
    ServiceContainer(const ServiceContainer&) = delete;
    ServiceContainer& operator=(const ServiceContainer&) = delete;

    static ServiceContainer& get() {
        static ServiceContainer instance;
        return instance;
    }

    std::shared_ptr<RecordService> getRecordService() { return m_recordService; }
    std::shared_ptr<ConfigurationService> getConfigService() { return m_configService; }
    std::shared_ptr<GamePlayService> getGameService() { return m_gamePlayService; }
};