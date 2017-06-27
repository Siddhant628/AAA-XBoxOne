#include "pch.h"
#include "Plane.h"
#include "SpriteManager.h"
#include "GameManager.h"

namespace DirectXGame
{
	const std::float_t Plane::sSpawnPositionA1X = -512 - 50;
	const std::float_t Plane::sSpawnPositionA1Y = 316;
	const std::float_t Plane::sSpawnPositionA2X = -512 - 50;
	const std::float_t Plane::sSpawnPositionA2Y = 116;
	const std::float_t Plane::sSpawnPositionB1X = 512 + 50;
	const std::float_t Plane::sSpawnPositionB1Y = 316;
	const std::float_t Plane::sSpawnPositionB2X = 512 + 50;
	const std::float_t Plane::sSpawnPositionB2Y = 116;
	const std::float_t Plane::sMinSpawnPositionVarianceY = 5.0f;
	const std::float_t Plane::sMaxSpawnPositionVarianceY = 50.0f;

	const std::float_t Plane::sMinimumSpeed = 200.0f;
	const std::float_t Plane::sMaximumSpeed = 300.0f;
	const std::float_t Plane::sMinimumSpeedVariance = 25.0f;
	const std::float_t Plane::sMaximumSpeedVariance = 65.0f;
	const std::float_t Plane::sTimeForMaximumSpeed = 60.0f;

	// TODO Attach collider
	Plane::Plane(PlaneID id)
	{
		InitializeMembers(id);
		AttachSprite();
		if (id == PlaneID::PlaneA_1 || id == PlaneID::PlaneA_2)
		{
			GetSprite()->SetSprite(SpriteName::PlaneA);
		}
		else
		{
			GetSprite()->SetSprite(SpriteName::PlaneB);
		}
		RegisterToGameManager();
		SetPosition(mSpawnPositionX, mSpawnPositionY);
	}

	void Plane::Respawn(const DX::StepTimer& timer)
	{
		SetVelocity(GetSpeed(timer), 0);
		SetPosition(mSpawnPositionX, mSpawnPositionY + GetRangedRandom(sMinSpawnPositionVarianceY, sMaxSpawnPositionVarianceY) - sMaxSpawnPositionVarianceY / 2);
	}

	void Plane::Update(const DX::StepTimer & timer)
	{
		GameObject::Update(timer);
	}

	void Plane::ResetPlane()
	{
		SetPosition(mSpawnPositionX, mSpawnPositionY);
		SetVelocity(0, 0);
	}

	bool Plane::RequiresRespawn()
	{
		bool isReadyForRespawn = false;
		DirectX::XMFLOAT2 position = mTransform.Position();
		if (position.x <= sSpawnPositionA1X || position.x >= sSpawnPositionB1X)
		{
			isReadyForRespawn = true;
		}
		return isReadyForRespawn;
	}

	Plane::PlaneID Plane::GetPlaneID()
	{
		return mPlaneID;
	}

	std::float_t Plane::GetRangedRandom(std::float_t min, std::float_t max)
	{
		int r = rand();
		std::float_t r1 = (std::float_t)r / (std::float_t)RAND_MAX;
		r1 *= (max - min);
		r1 += min;
		return r1;
	}

	// TODO Set collider tags
	void Plane::InitializeMembers(PlaneID planeID)
	{
		mPlaneID = planeID;

		switch (mPlaneID)
		{
		case PlaneID::PlaneA_1:
			mSpawnPositionX = sSpawnPositionA1X;
			mSpawnPositionY = sSpawnPositionA1Y;
			//GetCollider()->SetColliderTag(Collider::ColliderTag::Player_A_Plane);
			break;
		case PlaneID::PlaneA_2:
			mSpawnPositionX = sSpawnPositionA2X;
			mSpawnPositionY = sSpawnPositionA2Y;
			//GetCollider()->SetColliderTag(Collider::ColliderTag::Player_A_Plane);
			break;
		case PlaneID::PlaneB_1:
			mSpawnPositionX = sSpawnPositionB1X;
			mSpawnPositionY = sSpawnPositionB1Y;
			//GetCollider()->SetColliderTag(Collider::ColliderTag::Player_B_Plane);
			break;
		case PlaneID::PlaneB_2:
			mSpawnPositionX = sSpawnPositionB2X;
			mSpawnPositionY = sSpawnPositionB2Y;
			//GetCollider()->SetColliderTag(Collider::ColliderTag::Player_B_Plane);
			break;
		}
	}

	void Plane::RegisterToGameManager()
	{
		GameManager* gameManager = GameManager::GetInstance();
		assert(gameManager != nullptr);
		gameManager->RegisterPlane(*this);
	}

	void Plane::ShotDown()
	{
		SetVelocity(0, 0);
		SetPosition(mSpawnPositionX, mSpawnPositionY);
	}

	std::float_t Plane::GetSpeed(const DX::StepTimer& timer)
	{
		// Estimate base speed based on the current time
		std::float_t timeRatio = static_cast<std::float_t>(timer.GetTotalSeconds() / sTimeForMaximumSpeed);
		if (timeRatio > 1.0f)
		{
			timeRatio = 1.0f;
		}
		std::float_t speed = sMinimumSpeed + timeRatio * (sMaximumSpeed - sMinimumSpeed);
		// Add deflection to the base speed
		speed = speed + GetRangedRandom(sMinimumSpeedVariance, sMaximumSpeedVariance) - sMaximumSpeedVariance / 2;
		// Return the speed based on the plane team
		if (mPlaneID == PlaneID::PlaneA_1 || mPlaneID == PlaneID::PlaneA_2)
		{
			return speed;
		}
		else
		{
			return -speed;
		}
	}
}