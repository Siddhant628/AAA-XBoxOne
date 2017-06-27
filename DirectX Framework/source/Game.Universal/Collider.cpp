#include "pch.h"
#include "Collider.h"
#include "CollisionManager.h"
#include "GameObject.h"

namespace DirectXGame
{
	Collider::~Collider()
	{
		CollisionManager::GetInstance()->Unregister(*this);
	}

	void Collider::SetDimensions(uint32_t width, uint32_t height)
	{
		mWidth = width;
		mHeight = height;
	}

	void Collider::SetOffset(std::float_t offsetX, std::float_t offsetY)
	{
		mOffsetX = offsetX;
		mOffsetY = offsetY;
	}

	void Collider::SetOwner(GameObject& object)
	{
		mOwnerObject = &object;
	}

	GameObject* Collider::GetOwner()
	{
		return mOwnerObject;
	}

	void Collider::SetColliderTag(Collider::ColliderTag tag)
	{
		mColliderTag = tag;
	}

	Collider::ColliderTag Collider::GetColliderTag()
	{
		return mColliderTag;
	}

	Collider::Collider(std::uint32_t width, std::uint32_t height, std::float_t offsetX, std::float_t offsetY) : mWidth(width), mHeight(height), mOffsetX(offsetX), mOffsetY(offsetY), mColliderTag(ColliderTag::Default)
	{
		CollisionManager::GetInstance()->Register(*this);
	}

	bool Collider::OnCollision(Collider& other)
	{
		if (this != &other)
		{
			DirectX::XMFLOAT2 thisPositon = GetOwner()->GetPosition();
			DirectX::XMFLOAT2 otherPosition = other.GetOwner()->GetPosition();

			std::float_t thisLeft = thisPositon.x + mOffsetX;
			std::float_t thisRight = thisLeft + mWidth;
			std::float_t thisBottom = thisPositon.y + mOffsetY;
			std::float_t thisTop = thisBottom + mHeight;

			std::float_t otherLeft = otherPosition.x + other.mOffsetX;
			std::float_t otherRight = otherLeft + other.mWidth;
			std::float_t otherBottom = otherPosition.y + other.mOffsetY;
			std::float_t otherTop = otherBottom + other.mHeight;

			if (thisLeft < otherRight && thisRight > otherLeft && thisTop > otherBottom && thisBottom < otherTop)
			{
				return true;
			}
		}
		return false;
	}
}