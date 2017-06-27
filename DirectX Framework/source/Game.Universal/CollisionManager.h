#pragma once

namespace DX
{
	class StepTimer;
}

namespace DirectXGame
{
	class Collider;

	class CollisionManager
	{
	public:
		/**
		* Default destructor.
		*/
		~CollisionManager() = default;
		/**
		* Create the singleton instance of this class.
		*/
		static CollisionManager* CreateInstance();
		/**
		* Get the singleton instance.
		*/
		static CollisionManager* GetInstance();
		/**
		* Initialize this singleton.
		*/
		void Initialize();
		/**
		* Update method for each frame.
		*/
		void Update(const DX::StepTimer& timer);
		/**
		* Register the collider to the collision manager.
		*/
		void Register(Collider& collider);
		/**
		* Unregister the collider from the collision manager.
		*/
		void Unregister(Collider& collider);
		/**
		* Clear the memory allocated to the singleton.
		*/
		void Shutdown();
	private:
		/**
		* Default constructor.
		*/
		CollisionManager() = default;
		/**
		* Remove the colliders which were queued for removal in previous frame.
		*/
		void ClearDeleteQueue();
		/**
		* The singleton instance of the class.
		*/
		static CollisionManager* sInstance;
		/**
		* The list of colliders registered in the game.
		*/
		std::vector<Collider*> mColliders;
		/**
		* The list of colliders queued for deletion in previous frame.
		*/
		std::vector<Collider*> mDeleteQueue;
	};
}