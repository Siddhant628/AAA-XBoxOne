#pragma once

namespace DX
{
	class StepTimer;
}

namespace DirectXGame
{
	class Plane;
	class SpriteManager;

	enum class PlayerEnum
	{
		PlayerA,
		PlayerB
	};

	class GameManager
	{
	public:
		/**
		* Default destructor.
		*/
		~GameManager() = default;

		/* Methods associated with the singleton. */
		/**
		* Create a singleton instance.
		*/
		static GameManager* CreateInstance();
		/**
		* Get the instance of singleton.
		*/
		static GameManager* GetInstance();
		/**
		* Initialize the level.
		*/
		void Initialize();
		/**
		* Update call for each frame.
		*/
		void Update(const DX::StepTimer& timer);
		/**
		* Deallocate memory associated with this level.
		*/
		void Shutdown();
		/**
		* Get the sprite manager associated with this game.
		*/
		static SpriteManager* GetSpriteManager();
		/**
		* Set the sprite manager associated with this game.
		*/
		static void SetSpriteManager(SpriteManager& spriteManager);

		/* Other methods. */
		/**
		* Register a plane to this game manager.
		*/
		void RegisterPlane(Plane& plane);

		bool IsGameRunning();
		/**
		* Request the game manager to restart the game.
		*/
		void RestartGame();
	private:
		/**
		* Private constructor for singleton.
		*/
		GameManager();

		/**
		* Pointer to the singleton.
		*/
		static GameManager* sInstance;
		/**
		* Is the game still running.
		*/
		bool mGameIsRunning;
		/**
		* The sprite manager associated with this instance of game.
		*/
		SpriteManager* mSpriteManager;
		/**
		* The amount of time since which game has been running.
		*/
		double mGameTime;


		/* Methods associated with loading of game objects. */
		/**
		* Load the background game object.
		*/
		void LoadBackground();
		/**
		* Load the plane game objects.
		*/
		void LoadPlanes();
		/**
		* Load the bullet game objects.
		*/
		void LoadBullets();
		/**
		* Load the turret game objects.
		*/
		void LoadTurrets();
		/**
		* Load the game objects associated with sprites displaying lives.
		*/
		void LoadLivesSprites();
		/**
		* Loads the sprite associated with the end game screen.
		*/
		void LoadEndGameScreen();


		/* Other methods. */
		/**
		* The level's planes are checked for respawn (re-spawnned if required), and health is decremented if planes cross into enemy territory.
		*/
		void PlanesUpdate();
		/**
		* Decrements the health of the player and checks for lose condition.
		*/
		void DecrementHealth(PlayerEnum playerEnum);
		/**
		* Perform actions associated with level end.
		*/
		void EndGame();
		/**
		* Re-enable the health sprites and update the health sprite iterators.
		*/
		void ResetLives();
		/**
		* Calls reset on each plane on both the teams.
		*/
		void ResetPlanes();

		
		/**
		* Game objects which represent lives of player A.
		*/
		std::vector<DirectXGame::GameObject*> mLivesPlayerA;
		/**
		* Game objects which represent lives of player B.
		*/
		std::vector<DirectXGame::GameObject*> mLivesPlayerB;
		/**
		* Game objects which represent planes of player A.
		*/
		std::vector<Plane*> mPlanesPlayerA;
		/**
		* Game objects which represent planes of player B.
		*/
		std::vector<Plane*> mPlanesPlayerB;
		/**
		* Iterator associated with the current health object of player A.
		*/
		std::vector<DirectXGame::GameObject*>::iterator mCurrentHealthA;
		/**
		* Iterator associated with the current health object of player B.
		*/
		std::vector<DirectXGame::GameObject*>::iterator mCurrentHealthB;
		/**
		* A pointer to the game object loaded at the end of the game.
		*/
		DirectXGame::GameObject* mEndGameScreen;

		//// Turret base related variables.
		static const std::float_t sPositionTurretBaseAX;
		static const std::float_t sPositionTurretBaseAY;
		static const std::float_t sPositionTurretBaseBX;
		static const std::float_t sPositionTurretBaseBY;
		//// End game screen related variables.
		static const std::float_t sEndGamePositionX;
		static const std::float_t sEndGamePositionY;
		//// Lives related variables.
		static const uint32_t sLivesCount;
		static const float_t sLivesPositionOffsetY;
		static const float_t sLivesPositionOffsetX;
	};
}