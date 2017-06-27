#include "pch.h"
#include "Bullet.h"
#include "SpriteManager.h"

namespace DirectXGame
{

	const std::float_t Bullet::sSpawnPositionPlayerAX = 0;//124.0f;
	const std::float_t Bullet::sSpawnPositionPlayerAY = 0;//47.0f;
	const std::float_t Bullet::sSpawnPositionPlayerBX = 0;//890.0f;
	const std::float_t Bullet::sSpawnPositionPlayerBY = 0;//47.0f;

	const std::uint32_t Bullet::sMaxBulletCount = 3;
	const std::float_t Bullet::sBulletSpeed = 520.0f;
	// TODO Understand
	const std::float_t Bullet::sFiringAngleOffset = 65.0f;
	const std::float_t Bullet::sBulletReloadTime = 1;
	// TODO Test
	const std::float_t Bullet::sMaxBulletHeight = 384 + 3;
	const double Bullet::sPI = 3.14159265;

	std::vector<Bullet*> Bullet::sPlayerABullets;
	std::vector<Bullet*> Bullet::sPlayerBBullets;
	bool Bullet::sRequiresReloadA = false;
	bool Bullet::sRequiresReloadB = false;
	double Bullet::sTimeSpentReloadingA = 0;
	double Bullet::sTimeSpentReloadingB = 0;

	// TODO Attach collider
	Bullet::Bullet(BulletOwner owner)
	{
		//AttachCollider(sColliderWidth, sColliderHeight, sColliderOffsetX, sColliderOffsetY);
		InitializeMembers(owner);
		AttachSprite();
		GetSprite()->SetSprite(SpriteName::Bullet);
	}

	void Bullet::Update(const DX::StepTimer & timer)
	{
		GameObject::Update(timer);
		CheckForCanBeFired();
		Reload(timer);
	}
	
	// TODO Test
	void Bullet::Fire(std::float_t angle, BulletOwner owner)
	{
		if (owner == BulletOwner::PlayerA)
		{
			auto it = sPlayerABullets.begin();
			auto end = sPlayerABullets.end();
			if (!sRequiresReloadA)
			{
				for (; it != end; ++it)
				{
					if ((*it)->mCanBeFired)
					{
						FireBullet(angle, **it);
						sRequiresReloadA = true;
						break;
					}
				}
			}
		}
		else if (owner == BulletOwner::PlayerB)
		{
			auto it = sPlayerBBullets.begin();
			auto end = sPlayerBBullets.end();
			if (!sRequiresReloadB)
			{
				for (; it != end; ++it)
				{
					if ((*it)->mCanBeFired)
					{
						FireBullet(angle, **it);
						sRequiresReloadB = true;
						break;
					}
				}
			}
		}
	}

	void Bullet::FireBullet(std::float_t angle, Bullet & bullet)
	{
		bullet.mCanBeFired = false;
		angle += sFiringAngleOffset;
		bullet.SetVelocity(sBulletSpeed * static_cast<float_t>(cos(angle * sPI / 180)), sBulletSpeed * static_cast<float_t>(sin(angle * sPI / 180)));
	}

	void Bullet::Reload(const DX::StepTimer & timer)
	{
		if (sRequiresReloadA)
		{
			std::uint32_t count = static_cast<std::uint32_t>(sPlayerABullets.size());
			sTimeSpentReloadingA += timer.GetElapsedSeconds();
			if (sTimeSpentReloadingA / count >= sBulletReloadTime)
			{
				sTimeSpentReloadingA = 0;
				sRequiresReloadA = false;
			}
		}
		if (sRequiresReloadB)
		{
			std::uint32_t count = static_cast<std::uint32_t>(sPlayerBBullets.size());
			sTimeSpentReloadingB += timer.GetElapsedSeconds();
			if (sTimeSpentReloadingB / count >= sBulletReloadTime)
			{
				sTimeSpentReloadingB = 0;
				sRequiresReloadB = false;
			}
		}
	}

	void Bullet::ResetBullet()
	{
		mCanBeFired = true;
		SetVelocity(0, 0);
		SetPosition(mSpawnPositionX, mSpawnPositionY);
	}

	// TODO Set collider tag
	void Bullet::InitializeMembers(BulletOwner owner)
	{
		switch (owner)
		{
		case Bullet::BulletOwner::PlayerA:
			mOwnerPlayer = BulletOwner::PlayerA;
			mSpawnPositionX = sSpawnPositionPlayerAX;
			mSpawnPositionY = sSpawnPositionPlayerAY;
			mTransform.SetPosition(mSpawnPositionX, mSpawnPositionY);
			sPlayerABullets.push_back(this);
			//GetCollider()->SetColliderTag(Collider::ColliderTag::Player_A_Bullet);
			break;
		case Bullet::BulletOwner::PlayerB:
			mOwnerPlayer = BulletOwner::PlayerB;
			mSpawnPositionX = sSpawnPositionPlayerBX;
			mSpawnPositionY = sSpawnPositionPlayerBY;
			mTransform.SetPosition(mSpawnPositionX, mSpawnPositionY);
			sPlayerBBullets.push_back(this);
			//GetCollider()->SetColliderTag(Collider::ColliderTag::Player_B_Bullet);
			break;
		default:
			break;
		}

	}

	void DirectXGame::Bullet::CheckForCanBeFired()
	{
		if (!mCanBeFired)
		{
			if (mTransform.Position().y > sMaxBulletHeight)
			{
				ResetBullet();
			}
		}
	}
}