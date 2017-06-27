#include "pch.h"
#include "InputManager.h"
#include "GameManager.h"
#include "GamePadComponent.h"

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
	
	// TODO Implement
	void InputManager::HandlePlayerAInput(const DX::StepTimer& timer)
	{
		timer;
	}

	// TODO Implement
	void InputManager::HandlePlayerBInput(const DX::StepTimer& timer)
	{
		timer;
	}
}