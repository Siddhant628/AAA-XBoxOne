#pragma once

#include "Transform2D.h"
#include "MatrixHelper.h"

namespace DirectXGame
{
	class GameObject;
	enum class SpriteName;

	class Sprite final
	{
		friend class GameObject;
	public:
		Sprite(const DX::Transform2D& transform = DX::Transform2D(), const DirectX::XMFLOAT4X4& textureTransform = DX::MatrixHelper::Identity);
		/**
		* Destructor.
		* Unregisters the sprite from Sprite Manager.
		*/
		~Sprite();

		const DX::Transform2D& Transform() const;
		void SetTransform(const DX::Transform2D& transform);

		const DirectX::XMFLOAT4X4& TextureTransform() const;
		void SetTextureTransform(const DirectX::XMFLOAT4X4& transform);

		const SpriteName GetSpriteName() const;
		void SetSpriteName(SpriteName spriteName);
	protected:
		/**
		* Get the owner game object.
		*/
		const GameObject* GetOwner() const;
		/**
		* Set the owner game object.
		*/
		void SetOwner(const GameObject& object);
		/**
		* The texture transform associated with this sprite.
		*/
		DirectX::XMFLOAT4X4 mTextureTransform;
		/**
		* The transform associated with this sprite.
		*/
		DX::Transform2D mTransform;
		/**
		* The game object with which this sprite is associated.
		*/
		const GameObject* mOwnerObject;
		/**
		* The enum value for the sprite in the sprite manager which is associated with this sprite.
		*/
		SpriteName mSpriteName;
	};
}