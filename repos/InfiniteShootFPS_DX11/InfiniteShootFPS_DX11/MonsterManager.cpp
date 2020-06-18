#include "Engine.h"
#include "Monster.h"
#include "MonsterManager.h"
#include "Object.h"

void MonsterManager::Init(Engine* engine)
{
	mEngine = engine;


	for (auto iter = mEngine->GetObjects().begin(); iter != mEngine->GetObjects().end(); ++iter)
	{
		if ((*iter)->GetType() == "Monster")
		{
			mMonsters.push_back(reinterpret_cast<Monster*>(*iter));
		}
	}
}

void MonsterManager::CleanUp()
{
	mMonsters.clear();
}
