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
		mSpriteManager(nullptr)
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
}