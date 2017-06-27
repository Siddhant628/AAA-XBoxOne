#include "pch.h"
#include "Sprite.h"
#include "SpriteManager.h"
#include "GameManager.h"

using namespace DX;
using namespace DirectX;

namespace DirectXGame
{
	Sprite::Sprite(const Transform2D & transform, const XMFLOAT4X4& textureTransform) :
		mTransform(transform),
		mTextureTransform(textureTransform),
		mSpriteName(SpriteName::Background),
		mIsEnabled(true)
	{
		GameManager::GetSpriteManager()->Register(*this);
	}

	Sprite::~Sprite()
	{
		GameManager::GetSpriteManager()->Unregister(*this);
	}

	void Sprite::Enable()
	{
		mIsEnabled = true;
	}

	void Sprite::Disable()
	{
		mIsEnabled = false;
	}

	bool Sprite::IsEnabled()
	{
		return mIsEnabled;
	}

	const Transform2D& Sprite::Transform() const
	{
		return mTransform;
	}

	void Sprite::SetTransform(const Transform2D& transform)
	{
		mTransform = transform;
	}

	const XMFLOAT4X4& Sprite::TextureTransform() const
	{
		return mTextureTransform;
	}

	void Sprite::SetTextureTransform(const XMFLOAT4X4& transform)
	{
		mTextureTransform = transform;
	}

	const SpriteName Sprite::GetSprite() const
	{
		return mSpriteName;
	}

	void Sprite::SetSprite(SpriteName spriteName)
	{
		mSpriteName = spriteName;
	}
	
	const GameObject* Sprite::GetOwner() const
	{
		return mOwnerObject;
	}

	void Sprite::SetOwner(const GameObject& object)
	{
		mOwnerObject = &object;
	}
}