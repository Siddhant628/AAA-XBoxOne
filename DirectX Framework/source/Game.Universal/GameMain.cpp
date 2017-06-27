#include "pch.h"
#include "GameMain.h"
#include "SpriteManager.h"
#include "GameObjectManager.h"
#include "GameManager.h"
#include "GameObject.h"

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

		GameObjectManager::CreateInstance();
		GameManager::CreateInstance();
		
		auto camera = make_shared<OrthographicCamera>(mDeviceResources, 1024.0f, 768.0f);
		mComponents.push_back(camera);
		camera->SetPosition(0, 0, 1);

		CoreWindow^ window = CoreWindow::GetForCurrentThread();
		mKeyboard = make_shared<KeyboardComponent>(mDeviceResources);		
		mKeyboard->Keyboard()->SetWindow(window);
		mComponents.push_back(mKeyboard);

		mMouse = make_shared<MouseComponent>(mDeviceResources);		
		mMouse->Mouse()->SetWindow(window);
		mComponents.push_back(mMouse);

		mGamePad = make_shared<GamePadComponent>(mDeviceResources);
		mComponents.push_back(mGamePad);

		auto fpsTextRenderer = make_shared<FpsTextRenderer>(mDeviceResources);
		mComponents.push_back(fpsTextRenderer);

		mSpriteManager = make_shared<SpriteManager>(mDeviceResources, camera);
		mComponents.push_back(mSpriteManager);

		mTimer.SetFixedTimeStep(true);
		mTimer.SetTargetElapsedSeconds(1.0 / 60);

		GameObjectManager::GetInstance()->Initialize();
		
		GameManager::GetInstance()->Initialize();
		GameManager::SetSpriteManager(*mSpriteManager);
		
		GameObject* object2 = new GameObject();
		object2->AttachSprite();
		object2->GetSprite()->SetSpriteName(SpriteName::PlaneA);
		object2->SetPosition(-512 + 50, 384 - 28);
		object2->SetVelocity(10, 0);

		GameObject* object3 = new GameObject();
		object3->AttachSprite();
		object3->GetSprite()->SetSpriteName(SpriteName::PlaneA);
		object3->SetPosition(-512 + 150, 384 - 84);

		GameObject* object = new GameObject();
		object->AttachSprite();

		
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
			GameManager::GetInstance()->Update(mTimer);

			if (mKeyboard->WasKeyPressedThisFrame(Keys::Escape) ||
				mMouse->WasButtonPressedThisFrame(MouseButtons::Middle) ||
				mGamePad->WasButtonPressedThisFrame(GamePadButtons::Back))
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
		context->OMSetRenderTargets(1, targets, mDeviceResources->GetDepthStencilView());

		// Clear the back buffer and depth stencil view.
		context->ClearRenderTargetView(mDeviceResources->GetBackBufferRenderTargetView(), DirectX::Colors::Black);
		context->ClearDepthStencilView(mDeviceResources->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

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

		for (auto& component : mComponents)
		{
			component->ReleaseDeviceDependentResources();
		}
	}

	// Notifies renderers that device resources may now be recreated.
	void GameMain::OnDeviceRestored()
	{
		GameObjectManager::CreateInstance();
		
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