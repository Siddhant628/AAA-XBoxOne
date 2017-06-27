#include "pch.h"
#include "GameMain.h"
#include "SpriteManager.h"
#include "GameObjectManager.h"
#include "GameManager.h"
#include "GameObject.h"
#include "KeyboardComponent.h"
#include "MouseComponent.h"
#include "GamePadComponent.h"
#include "InputManager.h"

using namespace DX;
using namespace std;
using namespace DirectX;
using namespace Windows::Foundation;
using namespace Windows::System::Threading;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::UI::Core;
using namespace Concurrency;

namespace DirectXGame
{
	// Loads and initializes application assets when the application is loaded.
	GameMain::GameMain(const shared_ptr<DX::DeviceResources>& deviceResources) :
		mDeviceResources(deviceResources)
	{
		// Register to be notified if the Device is lost or recreated
		mDeviceResources->RegisterDeviceNotify(this);

		auto camera = make_shared<OrthographicCamera>(mDeviceResources, 1024.0f, 768.0f);
		mComponents.push_back(camera);
		camera->SetPosition(0, 0, 1);

		CoreWindow^ window = CoreWindow::GetForCurrentThread();

		mGamePad = make_shared<GamePadComponent>(mDeviceResources, 0);
		mComponents.push_back(mGamePad);

		mGamePad2 = make_shared<GamePadComponent>(mDeviceResources, 1);
		mComponents.push_back(mGamePad2);

		auto fpsTextRenderer = make_shared<FpsTextRenderer>(mDeviceResources);
		mComponents.push_back(fpsTextRenderer);

		mSpriteManager = make_shared<SpriteManager>(mDeviceResources, camera);
		mComponents.push_back(mSpriteManager);

		mTimer.SetFixedTimeStep(true);
		mTimer.SetTargetElapsedSeconds(1.0 / 60);

		GameObjectManager::CreateInstance();
		InputManager::CreateInstance();
		GameManager::CreateInstance();
		
		InputManager::GetInstance()->SetGamePads(*mGamePad, *mGamePad2);
		GameManager::SetSpriteManager(*mSpriteManager);

		GameObjectManager::GetInstance()->Initialize();
		InputManager::GetInstance()->Initialize();
		GameManager::GetInstance()->Initialize();

		
		IntializeResources();
	}

	GameMain::~GameMain()
	{
		mDeviceResources->RegisterDeviceNotify(nullptr);
	}

	// Updates application state when the window size changes (e.g. device orientation change)
	void GameMain::CreateWindowSizeDependentResources()
	{
		for (auto& component : mComponents)
		{
			component->CreateWindowSizeDependentResources();
		}
	}

	// Updates the application state once per frame.
	void GameMain::Update()
	{
		// Update scene objects.
		mTimer.Tick([&]()
		{
			for (auto& component : mComponents)
			{
				component->Update(mTimer);	
			}
			GameObjectManager::GetInstance()->Update(mTimer);
			InputManager::GetInstance()->Update(mTimer);
			GameManager::GetInstance()->Update(mTimer);

			if (mGamePad->WasButtonPressedThisFrame(GamePadButtons::Back) || mGamePad2->WasButtonPressedThisFrame(GamePadButtons::Back))
			{
				CoreApplication::Exit();
			}
		});
	}

	// Renders the current frame according to the current application state.
	// Returns true if the frame was rendered and is ready to be displayed.
	bool GameMain::Render()
	{
		// Don't try to render anything before the first Update.
		if (mTimer.GetFrameCount() == 0)
		{
			return false;
		}

		auto context = mDeviceResources->GetD3DDeviceContext();

		// Reset the viewport to target the whole screen.
		auto viewport = mDeviceResources->GetScreenViewport();
		context->RSSetViewports(1, &viewport);

		// Reset render targets to the screen.
		ID3D11RenderTargetView *const targets[1] = { mDeviceResources->GetBackBufferRenderTargetView() };
		//context->OMSetRenderTargets(1, targets, mDeviceResources->GetDepthStencilView());
		context->OMSetRenderTargets(1, targets, nullptr);
		
		// Clear the back buffer and depth stencil view.
		context->ClearRenderTargetView(mDeviceResources->GetBackBufferRenderTargetView(), DirectX::Colors::CornflowerBlue);
		//context->ClearDepthStencilView(mDeviceResources->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		for (auto& component : mComponents)
		{
			auto drawableComponent = dynamic_pointer_cast<DrawableGameComponent>(component);
			if (drawableComponent != nullptr && drawableComponent->Visible())
			{
				drawableComponent->Render(mTimer);
			}
		}

		return true;
	}

	// Notifies renderers that device resources need to be released.
	void GameMain::OnDeviceLost()
	{
		GameObjectManager::GetInstance()->Shutdown();
		GameManager::GetInstance()->Shutdown();
		InputManager::GetInstance()->Shutdown();

		for (auto& component : mComponents)
		{
			component->ReleaseDeviceDependentResources();
		}
	}

	// TODO Handle input manager pointers being restored.

	// Notifies renderers that device resources may now be recreated.
	void GameMain::OnDeviceRestored()
	{
		GameObjectManager::CreateInstance();
		InputManager::CreateInstance();

		GameManager::CreateInstance();
		GameManager::SetSpriteManager(*mSpriteManager);

		IntializeResources();
	}

	void GameMain::IntializeResources()
	{
		for (auto& component : mComponents)
		{
			component->CreateDeviceDependentResources();
		}

		CreateWindowSizeDependentResources();
	}
}