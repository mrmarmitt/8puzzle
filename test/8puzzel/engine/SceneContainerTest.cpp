#include <gtest/gtest.h>

#include "8puzzle/engine/IRouter.h"
#include "8puzzle/engine/IScene.h"
#include "8puzzle/engine/ServiceContainer.h"

class MockScene final : public IScene {
public:

    void onEnter() override {}
    void draw() override {}
    void input() override {}
    void onExit() override {}
};


TEST(SceneContainerTest, RegisterFactoryAndGetScene) {
    SceneContainer& container = SceneContainer::get();
    container.unloadAll();

    container.registerFactory("dummy-0", [&]() { return std::make_unique<MockScene>(); });
    container.registerFactory("dummy-1", [&]() { return std::make_unique<MockScene>(); });
    container.registerFactory("dummy-2", [&]() { return std::make_unique<MockScene>(); });

    const IScene& dummy_0 = container.getScene("dummy-0");
    const IScene& dummy_1 = container.getScene("dummy-1");
    const IScene& dummy_2 = container.getScene("dummy-2");

    EXPECT_NE(&dummy_0, nullptr);
    EXPECT_NE(&dummy_1, nullptr);
    EXPECT_NE(&dummy_2, nullptr);

    EXPECT_EQ(&dummy_0, &container.getScene("dummy-0"));
    EXPECT_EQ(&dummy_1, &container.getScene("dummy-1"));
    EXPECT_EQ(&dummy_2, &container.getScene("dummy-2"));
}

TEST(SceneContainerTest, RegisterFactoryAndGetUnRegistredScene) {
    SceneContainer& container = SceneContainer::get();
    container.unloadAll();

    container.registerFactory("dummy-0", [&]() { return std::make_unique<MockScene>(); });

    EXPECT_THROW({
        container.getScene("non-existent-scene");
    }, std::runtime_error);
}

TEST(SceneContainerTest, RegisterFactoryAndUnloadAllScene) {
    SceneContainer& container = SceneContainer::get();
    container.unloadAll();

    container.registerFactory("dummy-0", [&]() { return std::make_unique<MockScene>(); });
    container.registerFactory("dummy-1", [&]() { return std::make_unique<MockScene>(); });
    container.registerFactory("dummy-2", [&]() { return std::make_unique<MockScene>(); });

    const IScene& dummy_0 = container.getScene("dummy-0");
    const IScene& dummy_1 = container.getScene("dummy-1");
    const IScene& dummy_2 = container.getScene("dummy-2");

    EXPECT_NE(&dummy_0, nullptr);
    EXPECT_NE(&dummy_1, nullptr);
    EXPECT_NE(&dummy_2, nullptr);

    EXPECT_EQ(&dummy_0, &container.getScene("dummy-0"));
    EXPECT_EQ(&dummy_1, &container.getScene("dummy-1"));
    EXPECT_EQ(&dummy_2, &container.getScene("dummy-2"));

    container.unloadAll();

    const IScene& dummy_0_new = container.getScene("dummy-0");
    const IScene& dummy_1_new = container.getScene("dummy-1");
    const IScene& dummy_2_new = container.getScene("dummy-2");

    EXPECT_NE(&dummy_0, &dummy_0_new);
    EXPECT_NE(&dummy_1, &dummy_1_new);
    EXPECT_NE(&dummy_2, &dummy_2_new);

    container.unloadAll();
}

TEST(SceneContainerTest, RegisterFactoryAndUnloadSceneScene) {
    SceneContainer& container = SceneContainer::get();
    container.unloadAll();

    container.registerFactory("dummy-0", [&]() { return std::make_unique<MockScene>(); });

    const IScene& dummy_0 = container.getScene("dummy-0");

    EXPECT_NE(&dummy_0, nullptr);
    EXPECT_EQ(&dummy_0, &container.getScene("dummy-0"));

    container.unloadScene("dummy-0");

    const IScene& dummy_0_new = container.getScene("dummy-0");

    EXPECT_NE(&dummy_0, &dummy_0_new);
    container.unloadAll();
}