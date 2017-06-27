#pragma once

#include "GameObject.h"
#include <string>

namespace DX
{
	class StepTimer;
}

namespace DirectXGame
{
	enum class PlayerEnum;

	class Turret final : public GameObject
	{
	public:
		/**
		* Initializes the members of the turret based on the player it belongs to.
		*/
		Turret(PlayerEnum turretPlayer);
		/**
		* Default destructor.
		*/
		~Turret() = default;
		/**
		* Update for each frame.
		*/
		virtual void Update(const DX::StepTimer& timer) override;
	private:
		/**
		* Initializes the members of object.
		*/
		void InitializeMembers(PlayerEnum turretPlayer);

		// The positions of turrets on the display.
		static const std::float_t sTurretAPositionX;
		static const std::float_t sTurretAPositionY;
		static const std::float_t sTurretBPositionX;
		static const std::float_t sTurretBPositionY;
		// Default rotations of turrets.
		static const std::float_t sRotationTurretA;
		static const std::float_t sRotationTurretB;
	public:
		// Turret rotation data.
		static const std::float_t sTurretRotationSpeed;
		static const std::float_t sLeftRotationEnd;
		static const std::float_t sRightRotationEnd;
	};
}