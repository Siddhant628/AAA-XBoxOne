#include "pch.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include "Sprite.h"

using namespace DirectX;

namespace DirectXGame
{
	GameObject::GameObject()
	{
		SetPosition(0, 0);
		SetVelocity(0, 0);
		GameObjectManager::GetInstance()->Register(*this);
	}

	GameObject::~GameObject()
	{
		delete mSprite;
		GameObjectManager::GetInstance()->Unregister(*this);
		//delete mCollider;
	}

	void GameObject::SetPosition(std::float_t x, std::float_t y)
	{
		mTransform.SetPosition(DirectX::XMFLOAT2(x, y));
	}

	const DirectX::XMFLOAT2& GameObject::GetPosition() const
	{
		return mTransform.Position();
	}

	void GameObject::SetVelocity(std::float_t x, std::float_t y)
	{
		mVelocity = XMFLOAT2(x, y);
	}

	const DirectX::XMFLOAT2& GameObject::GetVelocity() const
	{
		return mVelocity;
	}

	void GameObject::SetRotation(std::float_t rotation)
	{
		mTransform.SetRotation(rotation);
	}

	const std::float_t GameObject::GetRotation() const
	{
		return mTransform.Rotation();
	}

	void GameObject::SetTransform(DX::Transform2D transform)
	{
		mTransform = transform;
	}

	const DX::Transform2D& GameObject::GetTransform() const
	{
		return mTransform;
	}

	void GameObject::SetSprite(Sprite& sprite)
	{
		delete mSprite;
		mSprite = &sprite;
		mSprite->SetOwner(*this);
	}

	Sprite* GameObject::GetSprite()
	{
		return mSprite;
	}

	void GameObject::AttachSprite()
	{
		delete mSprite;
		mSprite = new Sprite();
		mSprite->SetOwner(*this);
	}

	void GameObject::Update(const DX::StepTimer& timer)
	{
		double deltaTime = timer.GetElapsedSeconds();
		UpdatePosition(deltaTime);
		// TODO Remove as not required
		//if (mSprite)
		//{
		//	mSprite->Update();
		//}
	}

	void GameObject::UpdatePosition(double deltaSeconds)
	{
		if (mVelocity.x != 0 || mVelocity.y != 0)
		{
			float_t changeInX = static_cast<std::float_t>(mVelocity.x * deltaSeconds);
			float_t changeInY = static_cast<std::float_t>(mVelocity.y * deltaSeconds);

			XMFLOAT2 position = mTransform.Position();
			mTransform.SetPosition(XMFLOAT2(position.x + changeInX, position.y + changeInY));
		}
	}
}