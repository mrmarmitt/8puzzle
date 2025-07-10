#include <gtest/gtest.h>

#include "8puzzle/engine/ServiceContainer.h"

TEST(ServiceContainerTest, SingletonReturnsSameInstance) {
    ServiceContainer& instance1 = ServiceContainer::get();
    ServiceContainer& instance2 = ServiceContainer::get();
    EXPECT_EQ(&instance1, &instance2);
}

TEST(ServiceContainerTest, ServicesAreNotNull) {
    ServiceContainer& container = ServiceContainer::get();
    EXPECT_NE(container.getRouter(), nullptr);
    EXPECT_NE(container.getRecordService(), nullptr);
    EXPECT_NE(container.getConfigService(), nullptr);
    EXPECT_NE(container.getGameService(), nullptr);
}

TEST(ServiceContainerTest, ServicesAreSingletons) {
    ServiceContainer& container = ServiceContainer::get();

    const auto router1 = container.getRouter();
    const auto router2 = container.getRouter();
    EXPECT_EQ(router1, router2);

    const auto record1 = container.getRecordService();
    const auto record2 = container.getRecordService();
    EXPECT_EQ(record1, record2);

    const auto config1 = container.getConfigService();
    const auto config2 = container.getConfigService();
    EXPECT_EQ(config1, config2);

    const auto game1 = container.getGameService();
    const auto game2 = container.getGameService();
    EXPECT_EQ(game1, game2);
}