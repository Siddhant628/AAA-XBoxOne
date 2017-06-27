#include "pch.h"
#include "CollisionManager.h"
#include "Collider.h"
#include "GameObject.h"

namespace DirectXGame
{
	CollisionManager* CollisionManager::sInstance = nullptr;

	CollisionManager* CollisionManager::CreateInstance()
	{
		if (!sInstance)
		{
			sInstance = new CollisionManager();
		}
		return sInstance;
	}

	CollisionManager* CollisionManager::GetInstance()
	{
		return sInstance;
	}

	void CollisionManager::Initialize()
	{

	}

	void CollisionManager::Update(const DX::StepTimer& timer)
	{
		timer;
		ClearDeleteQueue();

		auto it = mColliders.begin();
		auto end = mColliders.end();
		auto itRest = it;

		for (; it != end; ++it)
		{

			for (itRest = it; itRest != end; ++itRest)
			{
				Collider* itCollider = *it;
				Collider* itRestCollider = *itRest;
				if (itCollider->OnCollision(*itRestCollider))
				{
					itRestCollider->GetOwner()->InCollision(*itCollider);
					itCollider->GetOwner()->InCollision(*itRestCollider);
				}
			}
		}
	}

	void CollisionManager::Register(Collider& collider)
	{
		mColliders.push_back(&collider);
	}

	void CollisionManager::Unregister(Collider& collider)
	{
		mDeleteQueue.push_back(&collider);
	}

	void CollisionManager::Shutdown()
	{
		delete sInstance;
		sInstance = nullptr;
	}

	void CollisionManager::ClearDeleteQueue()
	{
		std::vector<Collider*>::iterator end = mDeleteQueue.end();
		for (auto it = mDeleteQueue.begin(); it != end; ++it)
		{
			auto itDelete = std::find(mColliders.begin(), mColliders.end(), *it);
			mColliders.erase(itDelete);
		}
		mDeleteQueue.clear();
	}
}