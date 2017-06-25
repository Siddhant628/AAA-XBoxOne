#include "pch.h"
#include "Sprite.h"

using namespace DX;
using namespace DirectX;

namespace DirectXGame
{
	Sprite::Sprite(uint32_t spriteIndex, const Transform2D & transform, Moods mood, const XMFLOAT4X4& textureTransform) :
		mSpriteIndex(spriteIndex), mTransform(transform), mMood(mood), mTextureTransform(textureTransform)
	{
	}

	uint32_t Sprite::SpriteIndex() const
	{
		return mSpriteIndex;
	}

	void Sprite::SetSpriteIndex(const uint32_t spriteIndex)
	{
		mSpriteIndex = spriteIndex;
	}

	const Transform2D& Sprite::Transform() const
	{
		return mTransform;
	}

	void Sprite::SetTransform(const Transform2D& transform)
	{
		mTransform = transform;
	}

	Sprite::Moods Sprite::Mood() const
	{
		return mMood;
	}

	void Sprite::SetMood(const Moods mood)
	{
		mMood = mood;
	}

	const XMFLOAT4X4& Sprite::TextureTransform() const
	{
		return mTextureTransform;
	}

	void Sprite::SetTextureTransform(const XMFLOAT4X4& transform)
	{
		mTextureTransform = transform;
	}
}