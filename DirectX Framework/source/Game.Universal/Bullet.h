#pragma once

#include "GameObject.h"
#include <windows.h>
#include <vector>

namespace DX
{
	class StepTimer;
}

namespace DirectXGame
{
	class Bullet final : public GameObject
	{
	public:
		enum class BulletOwner
		{
			PlayerA,
			PlayerB
		};
		/**
		* Initializes members for the specific owner player, adds the sprite with appropriate data.
		*/
		Bullet(BulletOwner owner);
		/**
		* Default destructor.
		*/
		~Bullet() = default;
		/**
		* Update method for each frame.
		*/
		virtual void Update(const DX::StepTimer& timer) override;
		/**
		* Request the bullet manager to fire a bullet.
		* @param angle The angle at which the bullet needs to be fired.
		* @param owner The player which has to fire the bullet.
		*/
		static void Fire(std::float_t angle, BulletOwner owner);

		// TODO Implement
		//virtual void InCollision(Engine::Collider& otherCollider) override;

	private:
		/**
		* Fires the bullet.
		* @param angle the angle at which the bullet has to be fired.
		* @param bullet A reference to the bullet which has to be fired.
		*/
		static void FireBullet(std::float_t angle, Bullet& bullet);
		/**
		* Spend time in reloading if required.
		*/
		static void Reload(const DX::StepTimer& timer);
		/**
		* Resets the bullet.
		*/
		void ResetBullet();
		/**
		* Initializes the position data and register the bullet to the bullet manager.
		*/
		void InitializeMembers(BulletOwner owner);
		/**
		* Check if a bullet can be fired again. ( Once it is out of display)
		*/
		void CheckForCanBeFired();
		/**
		* The player which owns this bullet.
		*/
		BulletOwner mOwnerPlayer;
		/**
		* The spawn position of this bullet along X.
		*/
		std::float_t mSpawnPositionX;
		/**
		* The spawn position of this bullet along Y.
		*/
		std::float_t mSpawnPositionY;
		/**
		* True if the bullet can be fired.
		*/
		bool mCanBeFired;

		/* Bullet manager related members. */
		/**
		* List of bullet pointers to bullets associated with player A.
		*/
		static std::vector<Bullet*> sPlayerABullets;
		/**
		* List of bullet pointers to bullets associated with player B.
		*/
		static std::vector<Bullet*> sPlayerBBullets;
		/**
		* If turret A needs to spend time to reload.
		*/
		static bool sRequiresReloadA;
		/**
		* If turret B needs to spend time to reload.
		*/
		static bool sRequiresReloadB;
		/**
		* Time spent by player A reloading.
		*/
		static double sTimeSpentReloadingA;
		/**
		* Time spent by player B reloading.
		*/
		static double sTimeSpentReloadingB;

		/**
		* Default spawn position data.
		*/
		static const std::float_t sSpawnPositionPlayerAX;
		static const std::float_t sSpawnPositionPlayerAY;
		static const std::float_t sSpawnPositionPlayerBX;
		static const std::float_t sSpawnPositionPlayerBY;
		static const double sPI;
		/**
		* Collider related data.
		*/
		//static const std::float_t sColliderOffsetX;
		//static const std::float_t sColliderOffsetY;
		//static const std::uint32_t sColliderWidth;
		//static const std::uint32_t sColliderHeight;
	public:
		/**
		* Bullet related data.
		*/
		static const std::uint32_t sMaxBulletCount;
		static const std::float_t sBulletSpeed;
		static const std::float_t sFiringAngleOffset;
		static const std::float_t sBulletReloadTime;
		static const std::float_t sMaxBulletHeight;
	};
}