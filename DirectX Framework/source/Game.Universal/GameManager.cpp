#include "pch.h"
#include "GameManager.h"
#include "GameObject.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "Plane.h"

#include <cassert>

namespace DirectXGame
{
	GameManager* GameManager::sInstance = nullptr;

	// TODO Implement
	GameManager::GameManager() : 
		mGameIsRunning(false),
		mSpriteManager(nullptr),
		mEndGameScreen(nullptr)
	{

	}

	void GameManager::LoadBackground()
	{
		GameObject* object = new GameObject();
		object->AttachSprite();
		object->GetSprite()->SetSprite(SpriteName::Background);
	}

	void GameManager::LoadPlanes()
	{
		new Plane(Plane::PlaneID::PlaneA_1);
		new Plane(Plane::PlaneID::PlaneA_2);
		new Plane(Plane::PlaneID::PlaneB_1);
		new Plane(Plane::PlaneID::PlaneB_2);
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
		LoadPlanes();
		LoadBackground();
	}

	// TODO Implement
	void GameManager::Update(const DX::StepTimer& timer)
	{
		PlanesUpdate(timer);
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
	
	void GameManager::RegisterPlane(Plane& plane)
	{
		if (plane.GetPlaneID() == Plane::PlaneID::PlaneA_1 || plane.GetPlaneID() == Plane::PlaneID::PlaneA_2)
		{
			mPlanesPlayerA.push_back(&plane);
		}
		else if (plane.GetPlaneID() == Plane::PlaneID::PlaneB_1 || plane.GetPlaneID() == Plane::PlaneID::PlaneB_2)
		{
			mPlanesPlayerB.push_back(&plane);
		}
	}
	
	// TODO Decrement health
	void GameManager::PlanesUpdate(const DX::StepTimer& timer)
	{
		// Check if all planes are ready for respawning.
		auto it = mPlanesPlayerA.begin();
		auto end = mPlanesPlayerA.end();

		for (; it != end; ++it)
		{
			if (!(*it)->RequiresRespawn())
			{
				return;
			}
		}
		it = mPlanesPlayerB.begin();
		end = mPlanesPlayerB.end();
		for (; it != end; ++it)
		{
			if (!(*it)->RequiresRespawn())
			{
				return;
			}
		}
		// Check which planes did enter the enemy territory, update health accordingly, then respawn them.
		it = mPlanesPlayerB.begin();
		for (; it != end; ++it)
		{
			if ((*it)->GetPosition().x < Plane::sSpawnPositionA1X)
			{
				//DecrementHealth(PlayerEnum::PlayerA);
			}
			(*it)->Respawn(timer);
		}

		it = mPlanesPlayerA.begin();
		end = mPlanesPlayerA.end();
		for (; it != end; ++it)
		{
			if ((*it)->GetPosition().x > Plane::sSpawnPositionB1X)
			{
				//DecrementHealth(PlayerEnum::PlayerB);
			}
			(*it)->Respawn(timer);
		}
	}

	void GameManager::ResetPlanes()
	{
		auto end = mPlanesPlayerA.end();
		for (auto it = mPlanesPlayerA.begin(); it != end; ++it)
		{
			(*it)->ResetPlane();
		}
		end = mPlanesPlayerB.end();
		for (auto it = mPlanesPlayerB.begin(); it != end; ++it)
		{
			(*it)->ResetPlane();
		}
	}
}