#include "pch.h"
#include "Plane.h"
#include "SpriteManager.h"
#include "GameManager.h"

namespace DirectXGame
{
	const float_t Plane::sSpawnPositionA1X = -512+50;//-100.0f;
	const float_t Plane::sSpawnPositionA1Y = 350;//700.0f;
	const float_t Plane::sSpawnPositionA2X = -512+50;//-100.0f;
	const float_t Plane::sSpawnPositionA2Y = 250;//500.0f;
	const float_t Plane::sSpawnPositionB1X = 512-50;//1024.0f;
	const float_t Plane::sSpawnPositionB1Y = 350;//700.0f;
	const float_t Plane::sSpawnPositionB2X = 512-50;//1024.0f;
	const float_t Plane::sSpawnPositionB2Y = 250;//500.0f;

	// TODO Attach collider
	Plane::Plane(PlaneID id)
	{
		// Attach collider
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

	void Plane::Update(const DX::StepTimer & timer)
	{
		GameObject::Update(timer);
	}

	Plane::PlaneID Plane::GetPlaneID()
	{
		return mPlaneID;
	}

	// TODO Check if sprite sizes need handling
	void Plane::InitializeMembers(PlaneID planeID)
	{
		mPlaneID = planeID;

		switch (mPlaneID)
		{
		case PlaneID::PlaneA_1:
			mSpawnPositionX = sSpawnPositionA1X;
			mSpawnPositionY = sSpawnPositionA1Y;
			//mPlaneWidth = sWidthPlaneA;
			//mPlaneHeight = sHeightPlaneA;
			//mSpritePath = sSpritePathPlaneA;
			//GetCollider()->SetColliderTag(Collider::ColliderTag::Player_A_Plane);
			break;
		case PlaneID::PlaneA_2:
			mSpawnPositionX = sSpawnPositionA2X;
			mSpawnPositionY = sSpawnPositionA2Y;
			//mPlaneWidth = sWidthPlaneA;
			//mPlaneHeight = sHeightPlaneA;
			//mSpritePath = sSpritePathPlaneA;
			//GetCollider()->SetColliderTag(Collider::ColliderTag::Player_A_Plane);
			break;
		case PlaneID::PlaneB_1:
			mSpawnPositionX = sSpawnPositionB1X;
			mSpawnPositionY = sSpawnPositionB1Y;
			//mPlaneWidth = sWidthPlaneB;
			//mPlaneHeight = sHeightPlaneB;
			//mSpritePath = sSpritePathPlaneB;
			//GetCollider()->SetColliderTag(Collider::ColliderTag::Player_B_Plane);
			break;
		case PlaneID::PlaneB_2:
			mSpawnPositionX = sSpawnPositionB2X;
			mSpawnPositionY = sSpawnPositionB2Y;
			//mPlaneWidth = sWidthPlaneB;
			//mPlaneHeight = sHeightPlaneB;
			//mSpritePath = sSpritePathPlaneB;
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
}

