#pragma once

#include "Transform2D.h"
#include "MatrixHelper.h"

namespace DirectXGame
{
	class Sprite final
	{
	public:
		Sprite(const DX::Transform2D& transform, const DirectX::XMFLOAT4X4& textureTransform = DX::MatrixHelper::Identity);

		const DX::Transform2D& Transform() const;
		void SetTransform(const DX::Transform2D& transform);

		const DirectX::XMFLOAT4X4& TextureTransform() const;
		void SetTextureTransform(const DirectX::XMFLOAT4X4& transform);

	private:
		DirectX::XMFLOAT4X4 mTextureTransform;
		DX::Transform2D mTransform;
	};
}