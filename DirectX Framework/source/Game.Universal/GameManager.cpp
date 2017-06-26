#include "pch.h"
#include "GameManager.h"
#include <cassert>

namespace DirectXGame
{
	GameManager* GameManager::sInstance = nullptr;

	// TODO Implement
	GameManager::GameManager() : 
		mGameIsRunning(false),
		mSpriteManager(nullptr)
	{

	}

	GameManager* GameManager::CreateInstance()
	{
		if (!sInstance)
		{
			sInstance = new GameManager;
		}
		return sInstance;
	}

	GameManager* GameManager::GetInstance()
	{
		return sInstance;
	}

	// TODO Implement
	void GameManager::Initialize()
	{

	}

	// TODO Implement
	void GameManager::Update(const DX::StepTimer& timer)
	{
		timer;
	}

	void GameManager::Shutdown()
	{
		delete sInstance;
		sInstance = nullptr;
	}
	
	SpriteManager* GameManager::GetSpriteManager()
	{
		assert(sInstance != nullptr);
		assert(sInstance->mSpriteManager != nullptr);
		return sInstance->mSpriteManager;
	}

	void GameManager::SetSpriteManager(SpriteManager& spriteManager)
	{
		assert(sInstance != nullptr);
		sInstance->mSpriteManager = &spriteManager;
	}
}