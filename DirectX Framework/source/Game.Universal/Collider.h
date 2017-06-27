#pragma once

namespace DirectXGame
{
	class Collider
	{
		friend class GameObject;
		friend class CollisionManager;
	public:
		enum class ColliderTag
		{
			Default,
			Player_A_Bullet,
			Player_B_Bullet,
			Player_A_Plane,
			Player_B_Plane
		};

		/**
		* Deregisters the collider from the collision manager.
		*/
		~Collider();
		/**
		* Set the dimensions of this collider.
		* @param width The width of the collider.
		* @param height The height of the collider.
		*/
		void SetDimensions(uint32_t width, uint32_t height);
		/**
		* Set the position offsets of the collider.
		*/
		void SetOffset(std::float_t offsetX, std::float_t offsetY);
		/**
		* Set the owner game object.
		*/
		void SetOwner(GameObject& object);
		/**
		* Get the owner game object.
		*/
		GameObject* GetOwner();
		/**
		* Set the collider tag for this collider.
		*/
		void SetColliderTag(ColliderTag tag);
		/**
		* Get the collider tag for this collider.
		*/
		ColliderTag GetColliderTag();
	protected:
		/**
		* Constructor.
		* @param width The width of the collider.
		* @param height The height of the collider.
		*/
		Collider(std::uint32_t width, std::uint32_t height, std::float_t offsetX, std::float_t offsetY);
		/**
		* Check if this collider is colliding with another collider.
		* @param other The collider to check if this is colliding with.
		* @param return Ture if the two colliders are colliding.
		*/
		bool OnCollision(Collider& other);
	private:
		/**
		* The width of this collider.
		*/
		std::uint32_t mWidth;
		/**
		* The height of this collider.
		*/
		std::uint32_t mHeight;
		/**
		* The position offset of collider from the game object along X.
		*/
		std::float_t mOffsetX;
		/**
		* The position offset of collider from the game object along Y.
		*/
		std::float_t mOffsetY;
		/**
		* The game object with which this collider is associated.
		*/
		GameObject* mOwnerObject;
		/**
		* The collider tag for this collider.
		*/
		ColliderTag mColliderTag;
	};
}