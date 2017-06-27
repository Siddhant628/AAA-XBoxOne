#pragma once

#include "Transform2D.h"
#include "MatrixHelper.h"

namespace DirectXGame
{
	class Collider;

	class GameObject
	{
	public:
		/**
		* Default constructor.
		*/
		GameObject();
		/**
		* Delete the collider and sprite associated with this game object.
		*/
		virtual ~GameObject();
		/**
		* Set the position of this game object.
		* @param x The x-coordinate.
		* @param y The y-coordinate.
		*/
		void SetPosition(std::float_t x, std::float_t y);
		/**
		* Get the position of this game object.
		*/
		const DirectX::XMFLOAT2& GetPosition() const;
		/**
		* Set the velocity of this game object.
		* @param x The x-coordinate.
		* @param y The y-coordinate.
		*/
		void SetVelocity(std::float_t x, std::float_t y);
		/**
		* Get the velocity of this game object.
		*/
		const DirectX::XMFLOAT2& GetVelocity() const;
		/**
		* Set the rotation of this object in degrees.
		*/
		void SetRotation(std::float_t rotation);
		/**
		* Get the rotation of this game object in degrees.
		*/
		const std::float_t GetRotation() const;
		/**
		* Set the transform associated with this game object.
		* @param transform The tranform to set.
		*
		*/
		void SetTransform(DX::Transform2D transform);
		/**
		* Get the transform associated with this game object.
		*/
		const DX::Transform2D& GetTransform() const;
		/**
		* Set the collider of this object.
		*/
		void SetCollider(Collider& collider);
		/**
		* Get the collider of this object.
		*/
		Collider* GetCollider();
		
		/**
		* Set the sprite of this object.
		*/
		void SetSprite(Sprite& sprite);
		/**
		* Get the sprite of this object.
		*/
		Sprite* GetSprite();
		/**
		* Attach a new sprite to this game object. (Removes the preexisting one)
		*/
		void AttachSprite();
		/**
		* Attach a new collider to this game object. (Removes the preexisting one)
		*/
		void AttachCollider(std::uint32_t width, std::uint32_t height, float_t offsetX, float_t offsetY);
		/**
		* Update method for each frame.
		*/
		virtual void Update(const DX::StepTimer& timer);
		/**
		* The method which is called for this game object when it collides with another game object.
		*/
		virtual void InCollision(Collider& otherCollider);
	
		protected:
		/**
		* Update the position of this game object based on its current position.
		* @param deltaSeconds The amount of thime since previous frame.
		*/
		void UpdatePosition(double deltaSeconds);
		/**
		* The transform associated with this game object.
		*/
		DX::Transform2D mTransform;
		/**
		* The velocity of this game object.
		*/
		DirectX::XMFLOAT2 mVelocity;
		/**
		* The collider associated with this game object.
		*/
		Collider* mCollider;
		/**
		* The sprite associated with this game object.
		*/
		Sprite* mSprite;
	};
}