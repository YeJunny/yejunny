#pragma once

#include <vector>

class Engine;
class Monster;

class MonsterManager
{
public:
	void Init(Engine* engine);
	void CleanUp();

public:
	std::vector<Monster*> const& GetMonsters() const { return mMonsters; }

private:
	std::vector<Monster*>				mMonsters;

	// Core
	Engine*								mEngine;
};

