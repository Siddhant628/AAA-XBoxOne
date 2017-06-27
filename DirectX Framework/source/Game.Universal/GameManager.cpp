#include "pch.h"
#include "GameManager.h"
#include "GameObject.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "Plane.h"
#include "Bullet.h"
#include "Turret.h"

#include <cassert>

namespace DirectXGame
{
	const std::float_t GameManager::sPositionTurretBaseAX = -512 + 29 + 100;
	const std::float_t GameManager::sPositionTurretBaseAY = -384 + 35;
	const std::float_t GameManager::sPositionTurretBaseBX = 512 - 29 - 100;
	const std::float_t GameManager::sPositionTurretBaseBY = -384 + 35;

	const std::float_t GameManager::sEndGamePositionX = 0;
	const std::float_t GameManager::sEndGamePositionY = 384 - 250;

	const uint32_t GameManager::sLivesCount = 5;
	const float_t GameManager::sLivesPositionOffsetX = 30.0f;
	const float_t GameManager::sLivesPositionOffsetY = 5.0f;

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

	void GameManager::LoadBullets()
	{
		new Bullet(Bullet::BulletOwner::PlayerA);
		new Bullet(Bullet::BulletOwner::PlayerA);
		new Bullet(Bullet::BulletOwner::PlayerA);
		new Bullet(Bullet::BulletOwner::PlayerB);
		new Bullet(Bullet::BulletOwner::PlayerB);
		new Bullet(Bullet::BulletOwner::PlayerB);
	}

	void GameManager::LoadTurrets()
	{
		new Turret(PlayerEnum::PlayerA);
		new Turret(PlayerEnum::PlayerB);
		GameObject* turretBaseA = new GameObject;
		turretBaseA->AttachSprite();
		turretBaseA->SetPosition(sPositionTurretBaseAX, sPositionTurretBaseAY);
		turretBaseA->GetSprite()->SetSprite(SpriteName::TurretBase);
		GameObject* turretBaseB = new GameObject;
		turretBaseB->AttachSprite();
		turretBaseB->SetPosition(sPositionTurretBaseBX, sPositionTurretBaseBY);
		turretBaseB->GetSprite()->SetSprite(SpriteName::TurretBase);
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
		LoadEndGameScreen();
		//
		LoadBullets();
		LoadTurrets();
		
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
	
	void GameManager::LoadEndGameScreen()
	{
		mEndGameScreen = new GameObject;
		mEndGameScreen->SetPosition(sEndGamePositionX, sEndGamePositionY);
		mEndGameScreen->AttachSprite();
		mEndGameScreen->GetSprite()->Disable();
		mEndGameScreen->GetSprite()->SetSprite(SpriteName::GameEnd);
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