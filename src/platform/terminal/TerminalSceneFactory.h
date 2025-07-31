#pragma once
#include <memory>

class SceneRepository;
class GameRouter;
class ConfigurationService;
class GamePlayService;
class RecordService;

class TerminalSceneFactory {
public:
    static void populateTerminalScreens(
        const std::shared_ptr<SceneRepository> &sceneRepository,
        const std::shared_ptr<GameRouter> &gameRouter,
        const std::shared_ptr<ConfigurationService> &configurationService,
        const std::shared_ptr<GamePlayService> &gamePlayService,
        const std::shared_ptr<RecordService> &recordService);
};
