#pragma once

#include <windows.h>

namespace DX
{
	class GamePadComponent;
	class StepTimer;
}

namespace DirectXGame
{
	class Turret;

	class InputManager
	{
		friend class Turret;
	public:
		/**
		* Default destructor.
		*/
		~InputManager() = default;
		/**
		* Create the singleton instance of this class.
		*/
		static InputManager *CreateInstance();
		/**
		* Get the singleton instance.
		*/
		static InputManager *GetInstance();
		/**
		* Initialize this singleton.
		*/
		void Initialize();
		/**
		* Update method for each frame.
		*/
		void Update(const DX::StepTimer& timer);
		/**
		* Clear the memory allocated to the singleton.
		*/
		void Shutdown();
		/**
		* Set the game pads for this game.
		*/
		void SetGamePads(const DX::GamePadComponent& pad1, const DX::GamePadComponent& pad2);
	private:
		/**
		* Default constructor.
		*/
		InputManager();
		/**
		* Register player game object (turret) to the Input Manager.
		*/
		void AddPlayer(Turret& turret);
		/**
		* Handle the input associated with this application.
		*/
		void HandleInputAppication();
		/**
		* Handle the input associated with the player A.
		*/
		void HandlePlayerAInput(const DX::StepTimer& timer);
		/**
		* Handle the input associated with the player B.
		*/
		void HandlePlayerBInput(const DX::StepTimer& timer);
		/**
		* The singleton instance of the class.
		*/
		static InputManager *sInstance;
		/**
		* Pointer to the player 1 game object.
		*/
		Turret* mTurretPlayer1;
		/**
		* Pointer to the player 2 game object.
		*/
		Turret* mTurretPlayer2;
		/**
		* The gamepad associated with player 1.
		*/
		const DX::GamePadComponent* mGamepad1;
		/**
		* The gamepad associated with player 2.
		*/
		const DX::GamePadComponent* mGamepad2;
	};
}