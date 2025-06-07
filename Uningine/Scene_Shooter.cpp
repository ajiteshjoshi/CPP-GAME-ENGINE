#include "Scene_Shooter.h"
#include "GameEngine.h"
#include "Physics.h"
#include "SFML/Graphics.hpp"
#include <iostream>

Scene_Shooter::Scene_Shooter(GameEngine* gameEngine)
	: Scene(gameEngine)
{
	m_entityManager = EntityManager();
	init();
}

void Scene_Shooter::init()
{
	registerAction(sf::Keyboard::W, "MOVE_UP");
	registerAction(sf::Keyboard::A, "MOVE_LEFT");
	registerAction(sf::Keyboard::S, "MOVE_DOWN");
	registerAction(sf::Keyboard::D, "MOVE_RIGHT");
	registerAction(sf::Keyboard::Space, "SHOOT");

	LBoundary = m_game->window().getSize().x / 2 - m_game->assets().getTexture("map").getSize().x * 1.5 + 80;
	RBoundary = m_game->window().getSize().x / 2 + m_game->assets().getTexture("map").getSize().x * 1.5 - 80;
	TBoundary = m_game->window().getSize().y / 2 - m_game->assets().getTexture("map").getSize().y * 1.5 + 80;
	BBoundary = m_game->window().getSize().y / 2 + m_game->assets().getTexture("map").getSize().y * 1.5 - 80;

	auto entityBg = m_entityManager.addEntity("Background");
	entityBg->add<CTransform>(Vec2(0, 0));
	entityBg->add<CAnimation>(m_game->assets().getAnimation("background"), false);

	auto entityMap = m_entityManager.addEntity("Map");
	entityMap->add<CTransform>(Vec2(m_game->window().getSize().x / 2, m_game->window().getSize().y / 2), Vec2(0, 0), Vec2(3, 3), 0);
	entityMap->add<CAnimation>(m_game->assets().getAnimation("map"), false);

	auto entityWall = m_entityManager.addEntity("Wall");
	entityWall->add<CTransform>(Vec2(m_game->window().getSize().x / 2, m_game->window().getSize().y / 2), Vec2(0, 0), Vec2(3, 3), 0);
	entityWall->add<CAnimation>(m_game->assets().getAnimation("wall"), false);

	player = m_entityManager.addEntity("Player");
	player->add<CTransform>(Vec2(900, 400), Vec2(0, 0), Vec2(4, 4), 0);
	player->add<CAnimation>(m_game->assets().getAnimation("player_walk"), true);
	player->add<CBoundingBox>(Vec2(900, 400), Vec2(0, 0), Vec2(player->get<CAnimation>().animation.getSprite().getGlobalBounds().width * 2, player->get<CAnimation>().animation.getSprite().getGlobalBounds().height * 3));


	sf::Sprite backgroundSprite = entityBg->get<CAnimation>().animation.getSprite();
	backgroundSprite.setTextureRect(sf::IntRect(0, 0, m_game->window().getSize().x, m_game->window().getSize().y));



}

void Scene_Shooter::update()
{
	m_entityManager.update();
	Physics physics;

	if (player->get<CTransform>().pos.x < LBoundary && player->get<CTransform>().velocity.x < 0)
	{
		player->get<CTransform>().velocity.x = 0;
	}
	if (player->get<CTransform>().pos.x > RBoundary && player->get<CTransform>().velocity.x > 0)
	{
		player->get<CTransform>().velocity.x = 0;
	}
	if (player->get<CTransform>().pos.y < TBoundary && player->get<CTransform>().velocity.y < 0)
	{
		player->get<CTransform>().velocity.y = 0;
	}
	if (player->get<CTransform>().pos.y > BBoundary && player->get<CTransform>().velocity.y > 0)
	{
		player->get<CTransform>().velocity.y = 0;
	}

	for (auto& enemy : m_entityManager.getEntities("Enemy"))
	{
		for (auto& bullet : m_entityManager.getEntities("Bullet"))
		{
			if (physics.GetOverlap(enemy, bullet).x > 0 && physics.GetOverlap(enemy, bullet).y > 0)
			{
				enemy->Destroy();
				bullet->Destroy();
			}
		}
	}

	for (auto& enemyA : m_entityManager.getEntities("Enemy"))
	{
		for (auto& enemyB : m_entityManager.getEntities("Enemy"))
		{
			if (enemyA->id() != enemyB->id())
			{
				physics.ResolveCollision(enemyA, enemyB);
			}
		}
	}

	sEnemySpawnner();
	sMovement();
	sRender();
}

void Scene_Shooter::handleInput()
{
}

void Scene_Shooter::sMovement()
{
	for (const auto& entity : m_entityManager.getEntity())
	{
		if (entity->get<CTransform>().has)
		{
			entity->get<CTransform>().pos += entity->get<CTransform>().velocity;
			if (entity->get<CShape>().has)
			{
				entity->get<CShape>().Shape.setPosition(entity->get<CTransform>().pos.x, entity->get<CTransform>().pos.y);
			}
		}
	}

	for (const auto& enemy : m_entityManager.getEntities("Enemy"))
	{
		Vec2 dir = player->get<CTransform>().pos - enemy->get<CTransform>().pos;
		dir.normalize();
		enemy->get<CTransform>().velocity = dir;
	}

	for (const auto& bullet : m_entityManager.getEntities("Bullet"))
	{
		Vec2 pos = bullet->get<CTransform>().pos;
		if (pos.x < LBoundary || pos.x > RBoundary || pos.y < TBoundary || pos.y > BBoundary)
		{
			bullet->Destroy();
		}
	}
}

void Scene_Shooter::sEnemySpawnner()
{
	enemySpawnTimer++;
	if (enemySpawnTimer >= enemySpawnInterval)
	{
		auto enemy = m_entityManager.addEntity("Enemy");

		int offset = 150;
		float LBoundary = m_game->window().getSize().x / 2 - m_game->assets().getTexture("map").getSize().x * 1.5 + offset;
		float RBoundary = m_game->window().getSize().x / 2 + m_game->assets().getTexture("map").getSize().x * 1.5 - offset;
		float TBoundary = m_game->window().getSize().y / 2 - m_game->assets().getTexture("map").getSize().y * 1.5 + offset;
		float BBoundary = m_game->window().getSize().y / 2 + m_game->assets().getTexture("map").getSize().y * 1.5 - offset;

		Vec2 spawnPos;

		int side = rand() % 4;
		switch (side)
		{
		case 0:
			spawnPos = Vec2(rand() % (int) (RBoundary - LBoundary) + LBoundary, TBoundary);
			break;
		case 1:
			spawnPos = Vec2(rand() % (int) (RBoundary - LBoundary) + LBoundary, BBoundary);
			break;
		case 2:
			spawnPos = Vec2(LBoundary, rand() % (int) (BBoundary - TBoundary) + TBoundary);
			break;
		case 3:
			spawnPos = Vec2(RBoundary, rand() % (int) (BBoundary - TBoundary) + TBoundary);
			break;
		}

		enemy->add<CTransform>(spawnPos, Vec2(0, 0), Vec2(4, 4), 0);
		enemy->add<CAnimation>(m_game->assets().getAnimation("enemy_move"), true);
		enemy->add<CBoundingBox>(spawnPos, Vec2(0, 0), Vec2(enemy->get<CAnimation>().animation.getSprite().getGlobalBounds().width * 3, enemy->get<CAnimation>().animation.getSprite().getGlobalBounds().height * 3));
		enemySpawnTimer = 0;
	}
}

void Scene_Shooter::sRender()
{
	m_game->window().clear();
	for (const auto& entity : m_entityManager.getEntity())
	{


		if (entity->get<CAnimation>().has)
		{
			if (entity->tag() == "Background")
			{
				m_entityManager.getEntities("Background")[0]->get<CAnimation>().animation.getSprite().setTextureRect(sf::IntRect(0, 0, m_game->window().getSize().x, m_game->window().getSize().y));
			}
			auto& transform = entity->get<CTransform>();
			auto& animation = entity->get<CAnimation>().animation;
			animation.update();
			animation.getSprite().setRotation(transform.angle);
			animation.getSprite().setPosition(transform.pos.x, transform.pos.y);
			animation.getSprite().setScale(transform.scale.x, transform.scale.y);
			animation.getSprite().setColor(sf::Color::White);
			if (entity->get<CTransform>().has)
			{
				if (entity->get<CTransform>().velocity.x < 0)
					animation.getSprite().setScale(-1 * transform.scale.x, transform.scale.y);
			}
			m_game->window().draw(animation.getSprite());
		}

		if (entity->get<CShape>().has)
		{
			m_game->window().draw(entity->get<CShape>().Shape);
		}
	}
}

void Scene_Shooter::sDoAction(const Action& action)
{
	if (action.name() == "MOVE_UP" && action.type() == "START") {
		player->get<CTransform>().velocity.y = -5;
	}
	if (action.name() == "MOVE_DOWN" && action.type() == "START") {
		player->get<CTransform>().velocity.y = 5;
	}
	if (action.name() == "MOVE_LEFT" && action.type() == "START") {
		player->get<CTransform>().velocity.x = -5;
	}
	if (action.name() == "MOVE_RIGHT" && action.type() == "START") {
		player->get<CTransform>().velocity.x = 5;
	}
	if (action.name() == "MOVE_UP" && action.type() == "END") {
		player->get<CTransform>().velocity.y = 0;
	}
	if (action.name() == "MOVE_DOWN" && action.type() == "END") {
		player->get<CTransform>().velocity.y = 0;
	}
	if (action.name() == "MOVE_LEFT" && action.type() == "END") {
		player->get<CTransform>().velocity.x = 0;
	}
	if (action.name() == "MOVE_RIGHT" && action.type() == "END") {
		player->get<CTransform>().velocity.x = 0;
	}
	if (action.name() == "SHOOT" && action.type() == "START") {
		auto bullet = m_entityManager.addEntity("Bullet");
		Vec2 dir = player->get<CTransform>().velocity;
		dir.normalize();
		bullet->add<CTransform>(player->get<CTransform>().pos, dir * 10, Vec2(4, 4), 0);
		bullet->add<CAnimation>(m_game->assets().getAnimation("bullet"), false);
		bullet->add<CBoundingBox>(player->get<CTransform>().pos, Vec2(0, 0), Vec2(bullet->get<CAnimation>().animation.getSprite().getGlobalBounds().width * 3, bullet->get<CAnimation>().animation.getSprite().getGlobalBounds().height * 3));
	}
}

void Scene_Shooter::OnEnd()
{
}
