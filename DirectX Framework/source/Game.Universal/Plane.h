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

		void Respawn(DWORD currentTime);
		/**
		* Update method for each frame.
		*/
		virtual void Update(const DX::StepTimer& timer) override;

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
		* Initializes all the member variables for this plane.
		*/
		void InitializeMembers(PlaneID planeID);
		/**
		* Register this plane to the level manager.
		*/
		void RegisterToGameManager();

		void ShotDown();

		float_t GetSpeed(DWORD currentTime);
		/**
		* The spawn position of this plane along X.
		*/
		float_t mSpawnPositionX;
		/**
		* The spawn position of this plane along Y.
		*/
		float_t mSpawnPositionY;
		/**
		* The file directory for the sprite of this plane.
		*/
		//std::string mSpritePath;
		/**
		* The width of the sprite.
		*/
		uint32_t mPlaneWidth;
		/**
		* The height of the sprite.
		*/
		uint32_t mPlaneHeight;
		/**
		* The ID for this plane.
		*/
		PlaneID mPlaneID;

	public:
		/**
		* Spawn positions of planes.
		*/
		static const float_t sSpawnPositionA1X;
		static const float_t sSpawnPositionA1Y;
		static const float_t sSpawnPositionA2X;
		static const float_t sSpawnPositionA2Y;
		static const float_t sSpawnPositionB1X;
		static const float_t sSpawnPositionB1Y;
		static const float_t sSpawnPositionB2X;
		static const float_t sSpawnPositionB2Y;
		//static const float_t sMinSpawnPositionVarianceY;
		//static const float_t sMaxSpawnPositionVarianceY;
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
		//static const float_t sMinimumSpeed;
		//static const float_t sMaximumSpeed;
		//static const float_t sMinimumSpeedVariance;
		//static const float_t sMaximumSpeedVariance;
		//static const DWORD sTimeForMaximumSpeed;
	};
}