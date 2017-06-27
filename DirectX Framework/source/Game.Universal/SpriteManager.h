#pragma once

#include "DrawableGameComponent.h"
#include "MatrixHelper.h"
#include <vector>
#include <map>

namespace DirectXGame
{
	class Sprite;

	enum class SpriteName
	{
		Background,
		PlaneA,
		PlaneB,
		Bullet,
		Turret,
		TurretBase,
		LivesA,
		LivesB,
		GameEnd
	};

	class SpriteManager final : public DX::DrawableGameComponent
	{
	public:
		SpriteManager(const std::shared_ptr<DX::DeviceResources>& deviceResources, const std::shared_ptr<DX::Camera>& camera);
		
		const DirectX::XMFLOAT2& Position() const;
		void SetPositon(const DirectX::XMFLOAT2& position);

		virtual void CreateDeviceDependentResources() override;
		virtual void ReleaseDeviceDependentResources() override;
		virtual void Update(const DX::StepTimer& timer) override;
		virtual void Render(const DX::StepTimer& timer) override;

		/**
		* Register the sprite to the sprite manager.
		*/
		void Register(Sprite& sprite);
		/**
		* Unregister the sprite from the sprite manager.
		*/
		void Unregister(Sprite& sprite);
	private:
		struct VSCBufferPerObject
		{
			DirectX::XMFLOAT4X4 WorldViewProjection;
			DirectX::XMFLOAT4X4 TextureTransform;

			VSCBufferPerObject() :
				WorldViewProjection(DX::MatrixHelper::Identity), TextureTransform(DX::MatrixHelper::Identity)
			{ };

			VSCBufferPerObject(const DirectX::XMFLOAT4X4& wvp, const DirectX::XMFLOAT4X4& textureTransform) :
				WorldViewProjection(wvp), TextureTransform(textureTransform)
			{ }
		};

		struct SpriteData
		{
			std::wstring SpritePath;
			DirectX::XMFLOAT2 DefaultScale;
		};

		void DrawSprite(Sprite& sprite);
		void InitializeVertices();
		void InitializeSprites();


		Microsoft::WRL::ComPtr<ID3D11VertexShader> mVertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> mPixelShader;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> mInputLayout;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mIndexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mVSCBufferPerObject;
		Microsoft::WRL::ComPtr<ID3D11SamplerState> mTextureSampler;
		Microsoft::WRL::ComPtr<ID3D11BlendState> mAlphaBlending;
		VSCBufferPerObject mVSCBufferPerObjectData;
		std::uint32_t mIndexCount;
		bool mLoadingComplete;

		/**
		* Delete the sprites which were queued for deletion in previous frame.
		*/
		void ClearDeleteQueue();

		std::vector<Sprite*> mSprites;
		std::vector<Sprite*> mDeleteQueue;
		DirectX::XMFLOAT2 mPosition;

		static std::map<SpriteName, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> sSpriteSheets;

	public:
		const static std::map<SpriteName, SpriteData> sSpriteData;
	};
}