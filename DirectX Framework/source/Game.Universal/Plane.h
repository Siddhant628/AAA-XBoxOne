#pragma once
#include "GameObject.h"
#include <windows.h>

namespace DX
{
	class StepTimer;
}

namespace DirectXGame
{
	class Plane final : public DirectXGame::GameObject
	{
	public:
		enum class PlaneID
		{
			PlaneA_1,
			PlaneA_2,
			PlaneB_1,
			PlaneB_2
		};
		/**
		* Initializes members for the specific owner player, adds the sprite with appropriate data.
		*/
		Plane(PlaneID id);
		/**
		* Default destructor.
		*/
		~Plane() = default;

		void Respawn(const double& totalTime);
		/**
		* Update method for each frame.
		*/
		virtual void Update(const DX::StepTimer& timer) override;

		// TODO Implement
		//virtual void InCollision(Engine::Collider& otherCollider) override;
		
		/**
		* Reset the plane to its spawn position with 0 velocity.
		*/
		void ResetPlane();
		/**
		* Check if a plane is supposed to respawn.
		*/
		bool RequiresRespawn();
		/**
		* Get the ID enum value for this plane.
		*/
		PlaneID GetPlaneID();
	private:
		/**
		* Get a random number between two values.
		*/
		std::float_t GetRangedRandom(std::float_t min, std::float_t max);
		/**
		* Initializes all the member variables for this plane.
		*/
		void InitializeMembers(PlaneID planeID);
		/**
		* Register this plane to the level manager.
		*/
		void RegisterToGameManager();
		/**
		* Resets the plane when it is shot down.
		*/
		void ShotDown();
		/**
		* Get the speed with which a plabe should be respawned.
		*/
		std::float_t GetSpeed(const double& totalTime);
		/**
		* The spawn position of this plane along X.
		*/
		std::float_t mSpawnPositionX;
		/**
		* The spawn position of this plane along Y.
		*/
		std::float_t mSpawnPositionY;
		/**
		* The ID for this plane.
		*/
		PlaneID mPlaneID;
	public:
		/**
		* Spawn positions of planes.
		*/
		static const std::float_t sSpawnPositionA1X;
		static const std::float_t sSpawnPositionA1Y;
		static const std::float_t sSpawnPositionA2X;
		static const std::float_t sSpawnPositionA2Y;
		static const std::float_t sSpawnPositionB1X;
		static const std::float_t sSpawnPositionB1Y;
		static const std::float_t sSpawnPositionB2X;
		static const std::float_t sSpawnPositionB2Y;
		static const std::float_t sMinSpawnPositionVarianceY;
		static const std::float_t sMaxSpawnPositionVarianceY;
		/**
		* Collider related data.
		*/
		//static const float_t sColliderOffsetX;
		//static const float_t sColliderOffsetY;
		//static const uint32_t sColliderWidth;
		//static const uint32_t sColliderHeight;
		/**
		* File paths for plane sprites.
		*/
		//static const std::string sSpritePathPlaneA;
		//static const std::string sSpritePathPlaneB;
		/**
		* Dimensions of sprites.
		*/
		//static const uint32_t sWidthPlaneA;
		//static const uint32_t sHeightPlaneA;
		//static const uint32_t sWidthPlaneB;
		//static const uint32_t sHeightPlaneB;
		/**
		* Mean speeds of planes along x-axis.
		*/
		static const std::float_t sMinimumSpeed;
		static const std::float_t sMaximumSpeed;
		static const std::float_t sMinimumSpeedVariance;
		static const std::float_t sMaximumSpeedVariance;
		static const std::float_t sTimeForMaximumSpeed;
	};
}