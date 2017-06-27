#include "pch.h"
#include "Turret.h"
#include "SpriteManager.h"
#include "GameManager.h"

namespace DirectXGame
{
	const std::float_t Turret::sTurretAPositionX = -512 + 127 + 29;
	const std::float_t Turret::sTurretAPositionY = -384 + 27 + 50;
	const std::float_t Turret::sTurretBPositionX = 512 - 73 - 29;
	const std::float_t Turret::sTurretBPositionY = -384 + 27 + 50;

	// TODO Check all rotation values and speed
	const std::float_t Turret::sRotationTurretA = 5.0f;
	const std::float_t Turret::sRotationTurretB = 45.0f;

	const std::float_t Turret::sTurretRotationSpeed = 30.0f;
	const std::float_t Turret::sLeftRotationEnd = 5.0f;
	const std::float_t Turret::sRightRotationEnd = 45.0f;

	// TODO Add player to input manager
	Turret::Turret(PlayerEnum turretPlayer)
	{
		InitializeMembers(turretPlayer);
		AttachSprite();
		GetSprite()->SetSprite(SpriteName::Turret);
		//InputManager::GetInstance()->AddPlayer(*this);
	}

	void Turret::Update(const DX::StepTimer & timer)
	{
		GameObject::Update(timer);
	}

	void Turret::InitializeMembers(PlayerEnum turretPlayer)
	{
		switch (turretPlayer)
		{
		case PlayerEnum::PlayerA:
			mTransform.SetPosition(sTurretAPositionX, sTurretAPositionY);
			mTransform.SetRotation(sRotationTurretA);
			break;
		case PlayerEnum::PlayerB:
			mTransform.SetPosition(sTurretBPositionX, sTurretBPositionY);
			mTransform.SetRotation(sRotationTurretB);
			break;
		}
	}
}