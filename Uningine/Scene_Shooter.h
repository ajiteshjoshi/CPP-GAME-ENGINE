#pragma once

#include "Scene.h"
#include <memory>
#include "SFML/Graphics.hpp"

class Scene_Shooter : public Scene
{
public:
	Scene_Shooter()
	{
	}
	Scene_Shooter(GameEngine* gameEngine = nullptr);
	void init();
	void update() override;
	void handleInput();
	void sMovement();
	void sEnemySpawnner();
	void sRender() override;
	void sDoAction(const Action& action) override;

protected:
	void OnEnd();

private:
	std::shared_ptr<Entity> player;
	std::vector<std::shared_ptr<Entity>> enemies;
	std::vector<std::shared_ptr<Entity>> bullets;

	size_t enemySpawnTimer = 0;
	size_t enemySpawnInterval = 60;

	float LBoundary, RBoundary, TBoundary, BBoundary;
};