#include "pch.h"
#include "GameManager.h"
#include "GameObject.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "Plane.h"
#include "Bullet.h"
#include "Turret.h"

#include <cassert>
#include <time.h>

namespace DirectXGame
{
	const std::float_t GameManager::sPositionTurretBaseAX = -512 + 29 + 100;
	const std::float_t GameManager::sPositionTurretBaseAY = -384 + 35 + 27;
	const std::float_t GameManager::sPositionTurretBaseBX = 512 - 29 - 100;
	const std::float_t GameManager::sPositionTurretBaseBY = -384 + 35 + 27;

	const std::float_t GameManager::sEndGamePositionX = 0;
	const std::float_t GameManager::sEndGamePositionY = 384 - 250;

	const uint32_t GameManager::sLivesCount = 5;
	const float_t GameManager::sLivesPositionOffsetX = 30;
	const float_t GameManager::sLivesPositionOffsetY = -384 + 10 + 5;

	GameManager* GameManager::sInstance = nullptr;

	GameManager::GameManager() :
		mGameIsRunning(true),
		mSpriteManager(nullptr),
		mEndGameScreen(nullptr),
		mGameTime(0)
	{
		srand(static_cast<std::uint32_t>(time(nullptr)));
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

	void GameManager::LoadLivesSprites()
	{
		for (uint32_t i = 0; i < sLivesCount; ++i)
		{
			mLivesPlayerA.push_back(new GameObject);
			auto it = mLivesPlayerA.end();
			--it;
			(*it)->AttachSprite();
			(*it)->GetSprite()->SetSprite(SpriteName::LivesA);
			(*it)->SetPosition(-512 + 10 + 3 + sLivesPositionOffsetX * i, sLivesPositionOffsetY);

			mLivesPlayerB.push_back(new GameObject);
			it = mLivesPlayerB.end();
			--it;
			(*it)->AttachSprite();
			(*it)->GetSprite()->SetSprite(SpriteName::LivesB);
			(*it)->SetPosition(512 - 10 - 3 - sLivesPositionOffsetX * i, sLivesPositionOffsetY);
		}
		mCurrentHealthA = --mLivesPlayerA.end();
		mCurrentHealthB = mLivesPlayerB.begin();
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

	void GameManager::Initialize()
	{
		LoadBackground();
		LoadPlanes();
		LoadBullets();
		LoadTurrets();
		LoadLivesSprites();
		LoadEndGameScreen();
	}

	void GameManager::Update(const DX::StepTimer& timer)
	{
		mGameTime += timer.GetElapsedSeconds();
		if (mGameIsRunning)
		{
			PlanesUpdate();
		}
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

	bool GameManager::IsGameRunning()
	{
		return mGameIsRunning;
	}

	void GameManager::RestartGame()
	{
		assert(GameManager::GetInstance() != nullptr);
		if (!GameManager::GetInstance()->IsGameRunning())
		{
			ResetLives();
			ResetPlanes();
			mGameTime = 0;
			mEndGameScreen->GetSprite()->Disable();
			mGameIsRunning = true;
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

	void GameManager::PlanesUpdate()
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
				DecrementHealth(PlayerEnum::PlayerA);
			}
			(*it)->Respawn(mGameTime);
		}

		it = mPlanesPlayerA.begin();
		end = mPlanesPlayerA.end();
		for (; it != end; ++it)
		{
			if ((*it)->GetPosition().x > Plane::sSpawnPositionB1X)
			{
				DecrementHealth(PlayerEnum::PlayerB);
			}
			(*it)->Respawn(mGameTime);
		}
	}

	void GameManager::DecrementHealth(PlayerEnum playerEnum)
	{
		if (mGameIsRunning)
		{
			switch (playerEnum)
			{
			case PlayerEnum::PlayerA:
				(*mCurrentHealthA)->GetSprite()->Disable();
				if (mCurrentHealthA == mLivesPlayerA.begin())
				{
					EndGame();
					return;
				}
				mCurrentHealthA--;
				break;
			case PlayerEnum::PlayerB:
				(*mCurrentHealthB)->GetSprite()->Disable();
				mCurrentHealthB++;
				if (mCurrentHealthB == mLivesPlayerB.end())
				{
					EndGame();
				}
				break;
			}
		}
	}

	void GameManager::EndGame()
	{
		mEndGameScreen->GetSprite()->Enable();
		mGameIsRunning = false;
	}

	void GameManager::ResetLives()
	{
		// Enable all the health sprites
		auto end = mLivesPlayerA.end();
		for (auto it = mLivesPlayerA.begin(); it != end; ++it)
		{
			(*it)->GetSprite()->Enable();
		}
		end = mLivesPlayerB.end();
		for (auto it = mLivesPlayerB.begin(); it != end; ++it)
		{
			(*it)->GetSprite()->Enable();
		}
		// Reset the current health pointers
		mCurrentHealthA = --mLivesPlayerA.end();
		mCurrentHealthB = mLivesPlayerB.begin();
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