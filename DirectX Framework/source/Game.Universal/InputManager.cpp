#include "pch.h"
#include "InputManager.h"
#include "GameManager.h"
#include "GamePadComponent.h"
#include "Turret.h"
#include "Bullet.h"

#define DISABLE_ROTATE_ON_FIRE 0

using namespace DX;

namespace DirectXGame
{
	InputManager* InputManager::sInstance = nullptr;

	InputManager* InputManager::CreateInstance()
	{
		if (!sInstance)
		{
			sInstance = new InputManager();
		}
		return sInstance;
	}

	InputManager* InputManager::GetInstance()
	{
		return sInstance;
	}

	void InputManager::Initialize()
	{

	}

	void InputManager::Update(const DX::StepTimer& timer)
	{
		if (GameManager::GetInstance()->IsGameRunning())
		{
			HandlePlayerAInput(timer);
			HandlePlayerBInput(timer);
		}
	}

	void InputManager::Shutdown()
	{
		delete sInstance;
		sInstance = nullptr;
	}

	void InputManager::SetGamePads(const DX::GamePadComponent& pad1, const DX::GamePadComponent& pad2)
	{
		mGamepad1 = &pad1;
		mGamepad2 = &pad2;
	}

	InputManager::InputManager() :
		mTurretPlayer1(nullptr),
		mTurretPlayer2(nullptr),
		mGamepad1(nullptr),
		mGamepad2(nullptr)
	{

	}

	void InputManager::AddPlayer(Turret& turret)
	{
		if (!mTurretPlayer1)
		{
			mTurretPlayer1 = &turret;
		}
		else if (!mTurretPlayer2)
		{
			mTurretPlayer2 = &turret;
		}
	}
	
	void InputManager::HandlePlayerAInput(const DX::StepTimer& timer)
	{
		float_t turretRotation = mTurretPlayer1->GetRotation();
		// Fire bullet
		if (mGamepad1->IsButtonHeldDown(GamePadButtons::A))
		{
			Bullet::Fire(turretRotation, Bullet::BulletOwner::PlayerA);
		}
		// Rotate turret left
#if DISABLE_ROTATE_ON_FIRE
		else
		{
#endif
			if (mGamepad1->IsButtonHeldDown(GamePadButtons::DPadLeft))
			{
				if (turretRotation < Turret::sRightRotationEnd)
				{
					mTurretPlayer1->SetRotation(turretRotation + static_cast<std::float_t>(Turret::sTurretRotationSpeed * timer.GetElapsedSeconds()));
				}
			}
			// Rotate turret right
			if (mGamepad1->IsButtonHeldDown(GamePadButtons::DPadRight))
			{
				if (turretRotation > Turret::sLeftRotationEnd)
				{
					mTurretPlayer1->SetRotation(turretRotation - static_cast<std::float_t>(Turret::sTurretRotationSpeed * timer.GetElapsedSeconds()));
				}
			}
#if DISABLE_ROTATE_ON_FIRE
		}
#endif
	}

	void InputManager::HandlePlayerBInput(const DX::StepTimer& timer)
	{
		float_t turretRotation = mTurretPlayer1->GetRotation();
		// Fire bullet
		if (mGamepad2->IsButtonHeldDown(GamePadButtons::A))
		{
			Bullet::Fire(turretRotation, Bullet::BulletOwner::PlayerB);
		}
		// Rotate turret left
#if DISABLE_ROTATE_ON_FIRE
		else
		{
#endif
			if (mGamepad2->IsButtonHeldDown(GamePadButtons::DPadLeft))
			{
				if (turretRotation < Turret::sRightRotationEnd)
				{
					mTurretPlayer2->SetRotation(turretRotation + static_cast<std::float_t>(Turret::sTurretRotationSpeed * timer.GetElapsedSeconds()));
				}
			}
			// Rotate turret right
			if (mGamepad2->IsButtonHeldDown(GamePadButtons::DPadRight))
			{
				if (turretRotation > Turret::sLeftRotationEnd)
				{
					mTurretPlayer2->SetRotation(turretRotation - static_cast<std::float_t>(Turret::sTurretRotationSpeed * timer.GetElapsedSeconds()));
				}
			}
#if DISABLE_ROTATE_ON_FIRE
		}
#endif
	}
}