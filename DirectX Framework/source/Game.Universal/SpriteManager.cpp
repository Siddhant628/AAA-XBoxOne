#include "pch.h"
#include "SpriteManager.h"
#include "GameObject.h"

using namespace std;
using namespace DX;
using namespace DirectX;
using namespace Microsoft::WRL;

namespace DirectXGame
{
	const std::map<SpriteName, SpriteManager::SpriteData> SpriteManager::sSpriteData =
	{
		{SpriteName::Background,	{ L"Content\\Textures\\Background.png",		XMFLOAT2(512, 384) } },
		{SpriteName::Bullet,		{ L"Content\\Textures\\Bullet.png" ,		XMFLOAT2(3, 3) } },
		{SpriteName::GameEnd,		{ L"Content\\Textures\\GameEnd.png" ,		XMFLOAT2(512, 250) } },
		{SpriteName::LivesA,		{ L"Content\\Textures\\LivesA.png" ,		XMFLOAT2(10, 10) } },
		{SpriteName::LivesB,		{ L"Content\\Textures\\LivesB.png" ,		XMFLOAT2(10, 10) } },
		{SpriteName::PlaneA,		{ L"Content\\Textures\\PlaneA.png" ,		XMFLOAT2(50, 28) } },
		{SpriteName::PlaneB,		{ L"Content\\Textures\\PlaneB.png" ,		XMFLOAT2(50, 24) } },
		{SpriteName::Turret,		{ L"Content\\Textures\\Turret.png" ,		XMFLOAT2(22, 39) } },
		{SpriteName::TurretBase,	{ L"Content\\Textures\\TurretBase.png" ,	XMFLOAT2(29, 27) } }
	};

	std::map<SpriteName, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> SpriteManager::sSpriteSheets;


	SpriteManager::SpriteManager(const shared_ptr<DX::DeviceResources>& deviceResources, const shared_ptr<Camera>& camera) :
		DrawableGameComponent(deviceResources, camera),
		mLoadingComplete(false), mIndexCount(0),
		mPosition(0.0f, 0.0f)
	{
	}

	const XMFLOAT2& SpriteManager::Position() const
	{
		return mPosition;
	}

	void SpriteManager::SetPositon(const XMFLOAT2& position)
	{
		mPosition = position;
	}

	// TODO Use a static hashmap to load all the sprites
	void SpriteManager::CreateDeviceDependentResources()
	{
		auto loadVSTask = ReadDataAsync(L"SpriteRendererVS.cso");
		auto loadPSTask = ReadDataAsync(L"SpriteRendererPS.cso");

		// After the vertex shader file is loaded, create the shader and input layout.
		auto createVSTask = loadVSTask.then([this](const std::vector<byte>& fileData) {
			ThrowIfFailed(
				mDeviceResources->GetD3DDevice()->CreateVertexShader(
					&fileData[0],
					fileData.size(),
					nullptr,
					mVertexShader.ReleaseAndGetAddressOf()
				)
			);

			// Create an input layout
			ThrowIfFailed(
				mDeviceResources->GetD3DDevice()->CreateInputLayout(
					VertexPositionTexture::InputElements,
					VertexPositionTexture::InputElementCount,
					&fileData[0],
					fileData.size(),
					mInputLayout.ReleaseAndGetAddressOf()
				)
			);

			CD3D11_BUFFER_DESC constantBufferDesc(sizeof(VSCBufferPerObject), D3D11_BIND_CONSTANT_BUFFER);
			ThrowIfFailed(
				mDeviceResources->GetD3DDevice()->CreateBuffer(
					&constantBufferDesc,
					nullptr,
					mVSCBufferPerObject.ReleaseAndGetAddressOf()
				)
			);
		});

		// After the pixel shader file is loaded, create the shader and texture sampler state.
		auto createPSTask = loadPSTask.then([this](const std::vector<byte>& fileData) {
			ThrowIfFailed(
				mDeviceResources->GetD3DDevice()->CreatePixelShader(
					&fileData[0],
					fileData.size(),
					nullptr,
					mPixelShader.ReleaseAndGetAddressOf()
				)
			);

			D3D11_SAMPLER_DESC samplerStateDesc;
			ZeroMemory(&samplerStateDesc, sizeof(samplerStateDesc));
			samplerStateDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			samplerStateDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
			samplerStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
			samplerStateDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
			samplerStateDesc.MinLOD = -FLT_MAX;
			samplerStateDesc.MaxLOD = FLT_MAX;
			samplerStateDesc.MipLODBias = 0.0f;
			samplerStateDesc.MaxAnisotropy = 1;
			samplerStateDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
			ThrowIfFailed(mDeviceResources->GetD3DDevice()->CreateSamplerState(&samplerStateDesc, mTextureSampler.ReleaseAndGetAddressOf()));

			D3D11_BLEND_DESC blendStateDesc = { 0 };
			blendStateDesc.RenderTarget[0].BlendEnable = true;
			blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
			blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
			blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
			blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			blendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

			ThrowIfFailed(mDeviceResources->GetD3DDevice()->CreateBlendState(&blendStateDesc, mAlphaBlending.ReleaseAndGetAddressOf()));
		});

		// Load the sprite sheets after creating of pixel and vertex shader.
		auto loadSpriteSheetAndCreateSpritesTask = (createPSTask && createVSTask).then([this]() {
			
			for(std::int32_t spriteName = static_cast<std::int32_t>(SpriteName::Background); spriteName < static_cast<std::int32_t>(SpriteName::GameEnd) + 1; ++spriteName)
			{
				CreateWICTextureFromFile(mDeviceResources->GetD3DDevice(), sSpriteData.at(static_cast<SpriteName>(spriteName)).SpritePath.c_str(), nullptr, sSpriteSheets[static_cast<SpriteName>(spriteName)].ReleaseAndGetAddressOf());
			}

			InitializeVertices();
			InitializeSprites();
		});

		// Object is ready to be rendered.
		loadSpriteSheetAndCreateSpritesTask.then([this]() {
			mLoadingComplete = true;
		});
	}

	void SpriteManager::ReleaseDeviceDependentResources()
	{
		mLoadingComplete = false;
		mVertexShader.Reset();
		mPixelShader.Reset();
		mInputLayout.Reset();
		mVertexBuffer.Reset();
		mIndexBuffer.Reset();
		mVSCBufferPerObject.Reset();
		for (std::int32_t spriteName = static_cast<std::int32_t>(SpriteName::Background); spriteName != static_cast<std::int32_t>(SpriteName::GameEnd); ++spriteName)
		{
			sSpriteSheets.at(static_cast<SpriteName>(spriteName)).Reset();
		}
		mTextureSampler.Reset();
	}

	void SpriteManager::Update(const StepTimer& timer)
	{
		timer;
		if (!mLoadingComplete)
		{
			return;
		}
		ClearDeleteQueue();
	}

	void SpriteManager::Render(const StepTimer & timer)
	{
		UNREFERENCED_PARAMETER(timer);

		// Loading is asynchronous. Only draw geometry after it's loaded.
		if (!mLoadingComplete)
		{
			return;
		}

		ID3D11DeviceContext* direct3DDeviceContext = mDeviceResources->GetD3DDeviceContext();
		direct3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		direct3DDeviceContext->IASetInputLayout(mInputLayout.Get());

		static const UINT stride = sizeof(VertexPositionTexture);
		static const UINT offset = 0;
		direct3DDeviceContext->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), &stride, &offset);
		direct3DDeviceContext->IASetIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		direct3DDeviceContext->VSSetShader(mVertexShader.Get(), nullptr, 0);
		direct3DDeviceContext->PSSetShader(mPixelShader.Get(), nullptr, 0);
		direct3DDeviceContext->VSSetConstantBuffers(0, 1, mVSCBufferPerObject.GetAddressOf());
		direct3DDeviceContext->PSSetSamplers(0, 1, mTextureSampler.GetAddressOf());
		direct3DDeviceContext->OMSetBlendState(mAlphaBlending.Get(), 0, 0xFFFFFFFF);

		for (const auto& sprite : mSprites)
		{
			direct3DDeviceContext->PSSetShaderResources(0, 1, sSpriteSheets.at(sprite->GetSprite()).GetAddressOf());
			DrawSprite(*sprite);
		}
	}

	void SpriteManager::Register(Sprite& sprite)
	{
		mSprites.push_back(&sprite);
	}

	void SpriteManager::Unregister(Sprite& sprite)
	{
		mDeleteQueue.push_back(&sprite);
	}

	void SpriteManager::DrawSprite(Sprite& sprite)
	{
		ID3D11DeviceContext* direct3DDeviceContext = mDeviceResources->GetD3DDeviceContext();

		XMFLOAT2 objectPosition = sprite.GetOwner()->GetPosition();
		const XMMATRIX wvp = XMMatrixTranspose(sprite.Transform().WorldMatrix() * XMMatrixTranslation(objectPosition.x, objectPosition.y, 0) * mCamera->ViewProjectionMatrix());
		XMStoreFloat4x4(&mVSCBufferPerObjectData.WorldViewProjection, wvp);
		XMMATRIX textureTransform = XMLoadFloat4x4(&sprite.TextureTransform());
		XMStoreFloat4x4(&mVSCBufferPerObjectData.TextureTransform, XMMatrixTranspose(textureTransform));
		direct3DDeviceContext->UpdateSubresource(mVSCBufferPerObject.Get(), 0, nullptr, &mVSCBufferPerObjectData, 0, 0);

		direct3DDeviceContext->DrawIndexed(mIndexCount, 0, 0);
	}

	void SpriteManager::InitializeVertices()
	{
		VertexPositionTexture vertices[] =
		{
			VertexPositionTexture(XMFLOAT4(-1.0f, -1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f)),
			VertexPositionTexture(XMFLOAT4(-1.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f)),
			VertexPositionTexture(XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f)),
			VertexPositionTexture(XMFLOAT4(1.0f, -1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f)),
		};

		D3D11_BUFFER_DESC vertexBufferDesc = { 0 };
		vertexBufferDesc.ByteWidth = sizeof(VertexPositionTexture) * ARRAYSIZE(vertices);
		vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertexSubResourceData = { 0 };
		vertexSubResourceData.pSysMem = vertices;
		ThrowIfFailed(mDeviceResources->GetD3DDevice()->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, mVertexBuffer.ReleaseAndGetAddressOf()));

		// Create and index buffer
		const uint32_t indices[] =
		{
			0, 1, 2,
			0, 2, 3
		};

		mIndexCount = ARRAYSIZE(indices);

		D3D11_BUFFER_DESC indexBufferDesc = { 0 };
		indexBufferDesc.ByteWidth = sizeof(uint32_t) * mIndexCount;
		indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA indexSubResourceData = { 0 };
		indexSubResourceData.pSysMem = indices;
		ThrowIfFailed(mDeviceResources->GetD3DDevice()->CreateBuffer(&indexBufferDesc, &indexSubResourceData, mIndexBuffer.ReleaseAndGetAddressOf()));
	}

	void SpriteManager::InitializeSprites()
	{
		//const XMFLOAT2 neighborOffset(2.0f, 2.0f);
		//for (uint32_t column = 0; column < mSpriteColumnCount; ++column)		
		//{
		//	for (uint32_t row = 0; row < mSpriteRowCount; ++row)
		//	{
		//		XMFLOAT2 position(mPosition.x + column * neighborOffset.x * 3, mPosition.y + row * neighborOffset.y * 3);
		//		Transform2D transform(position, 0.0f, XMFLOAT2(3.0f, 3.0f));
		//		auto sprite = make_shared<Sprite>(transform);
		//		
		//		XMFLOAT4X4 textureTransform;
		//		XMMATRIX textureTransformMatrix = XMMatrixScaling(UVScalingFactor.x, UVScalingFactor.y, 0) * XMMatrixTranslation(UVScalingFactor.x * 2, UVScalingFactor.y * 1, 0.0f);
		//		XMStoreFloat4x4(&textureTransform, textureTransformMatrix);
		//		sprite->SetTextureTransform(textureTransform);
		//		
		//		mSprites.push_back(move(sprite));
		//	}
		//}
		//Transform2D transform;
		//transform.SetScale(sSpriteData.at(SpriteName::Background).DefaultScale);
		//auto sprite = new Sprite(transform);

		//XMFLOAT4X4 textureTransform;
		//XMMATRIX textureTransformMatrix = XMMatrixScaling(1.0f, 1.0f, 0);
		//XMStoreFloat4x4(&textureTransform, textureTransformMatrix);

		//sprite->SetTextureTransform(textureTransform);
		//mSprites.push_back(move(sprite));

		for (auto it = mSprites.begin(); it != mSprites.end(); ++it)
		{
			Transform2D transform;
			transform.SetScale(sSpriteData.at((*it)->GetSprite()).DefaultScale);
			(*it)->SetTransform(transform);
		}
	}

	void SpriteManager::ClearDeleteQueue()
	{
		std::vector<Sprite*>::iterator end = mDeleteQueue.end();
		for (auto it = mDeleteQueue.begin(); it != end; ++it)
		{
			auto itDelete = std::find(mSprites.begin(), mSprites.end(), *it);
			mSprites.erase(itDelete);
		}
		mDeleteQueue.clear();
	}
}