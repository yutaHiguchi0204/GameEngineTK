//
// Game.cpp
//

#include "Game.h"
#include <cstdlib>
#include <ctime>
#include "pch.h"

extern void ExitGame();

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace std;

using Microsoft::WRL::ComPtr;

Game::Game() :
	m_window(0),
	m_outputWidth(800),
	m_outputHeight(600),
	m_featureLevel(D3D_FEATURE_LEVEL_9_1)
{
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{
	srand(static_cast<unsigned int>(time(nullptr)));

	m_window = window;
	m_outputWidth = max(width, 1);
	m_outputHeight = max(height, 1);

	CreateDevice();

	CreateResources();

	/* 初期化はここへ */

	// カメラの生成
	m_camera = make_unique<FollowCamera>(m_outputWidth, m_outputHeight);

	// キーボード設定
	m_keyboard = std::make_unique<Keyboard>();
	m_keyboardTracker = std::make_unique<Keyboard::KeyboardStateTracker>();

	// カメラにキーボードを設定する
	m_camera->SetKeyboard(m_keyboard.get());

	// ３Ｄオブジェクトの静的メンバを初期化
	Obj3d::InitializeStatic(m_d3dDevice, m_d3dContext, m_camera.get());

	// 地形クラスの初期化
	LandShapeCommonDef def;
	def.pDevice = m_d3dDevice.Get();
	def.pDeviceContext = m_d3dContext.Get();
	def.pCamera = m_camera.get();
	LandShape::InitializeCommon(def);

	//m_batch = make_unique<PrimitiveBatch<VertexPositionColor>>(m_d3dContext.Get());
	m_batch = make_unique<PrimitiveBatch<VertexPositionNormal>>(m_d3dContext.Get());

	m_effect = make_unique<BasicEffect>(m_d3dDevice.Get());

	m_effect->SetVertexColorEnabled(true);

	m_view = Matrix::CreateLookAt(Vector3(0.f, 2.f, 2.f),
		Vector3::Zero, Vector3::UnitY);
	m_proj = Matrix::CreatePerspectiveFieldOfView(XM_PI / 4.f,
		float(m_outputWidth) / float(m_outputHeight), 0.1f, 500.f);

	m_effect->SetView(m_view);
	m_effect->SetProjection(m_proj);

	void const* shaderByteCode;
	size_t byteCodeLength;

	m_effect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

	m_d3dDevice->CreateInputLayout(VertexPositionColor::InputElements,
		VertexPositionColor::InputElementCount,
		shaderByteCode, byteCodeLength,
		m_inputLayout.GetAddressOf());

	// デバッグカメラの生成
	m_debugCamera = make_unique<DebugCamera>(m_outputWidth, m_outputHeight);

	// エフェクトファクトリの生成
	m_factory = std::make_unique<EffectFactory>(m_d3dDevice.Get());

	// テクスチャの読み取りパス指定
	m_factory->SetDirectory(L"Resources");

	// モデルの読み込み
	m_objSkyDome.LoadModel(L"Resources/skyDome.cmo");

	// 地面と天球のライティングを無効にする
	m_objSkyDome.DisableLighting();
	m_landshape.DisableLighting();

	// 自機パーツの読み込み
	m_player = std::make_unique<Player>();
	m_player->Initialize();

	// 敵パーツの読み込み
	m_enemy.resize(NUM_ENEMY);
	for (int i = 0; i < NUM_ENEMY; i++)
	{
		m_enemy[i] = make_unique<Enemy>();
		m_enemy[i]->Initialize();
	}

	// モデルエフェクトマネージャの読み込み
	m_effectManager = ModelEffectManager::getInstance();

	// 地形モデルの読み込み（landshapeファイル名、cmoファイル名）
	m_landshape.Initialize(L"ground_200m", L"ground_200m");

	// カメラにプレイヤーを渡す
	m_camera->SetPlayer(m_player.get());

	// TODO: Change the timer settings if you want something other than the default variable timestep mode.
	// e.g. for 60 FPS fixed timestep update logic, call:
	/*
	m_timer.SetFixedTimeStep(true);
	m_timer.SetTargetElapsedSeconds(1.0 / 60);
	*/
}

// Executes the basic game loop.
void Game::Tick()
{
	m_timer.Tick([&]()
	{
		Update(m_timer);
	});

	Render();
}

// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{
	float elapsedTime = float(timer.GetElapsedSeconds());

	// TODO: Add your game logic here.
	elapsedTime;

	/* 毎フレーム更新したい処理はここへ */
	m_debugCamera->Update();

	// ゴムひもカメラ設定
	{
		// カメラの更新
		m_camera->Update();
		m_view = m_camera->GetViewMatrix();
		m_proj = m_camera->GetProjectionMatrix();
	}

	// フィールドの更新
	m_objSkyDome.Update();

	// 地形の更新
	m_landshape.Update();

	// プレイヤーの更新
	m_player->Update();

	// 敵の更新
	for (std::vector<std::unique_ptr<Enemy>>::iterator itr = m_enemy.begin(); itr != m_enemy.end(); itr++)
	{
		(*itr)->Update();
	}

	// パーティクルの更新
	m_effectManager->Update();

	// 自機の地形へのめり込みを排斥する
	{
		// 自機の当たり判定を取得
		Sphere sphere = m_player->GetCollisionNodeBody();

		// 自機のワールド座標を取得
		Vector3 trans = m_player->GetParts(Player::PLAYER_PARTS::PARTS_BODY).GetTranslate();

		// 球の中心から自機のセンターへのベクトル
		Vector3 sphere2player = trans - sphere.center;

		// めり込み排斥ベクトル
		Vector3 reject;

		// 地形と球の当たり判定
		if (m_landshape.IntersectSphere(sphere, &reject))
		{
			// めり込みを解消する
			sphere.center += reject;
		}

		// 自機の移動
		m_player->GetParts(Player::PLAYER_PARTS::PARTS_BODY).SetTranslate(sphere.center + sphere2player);
	}

	// 自機が地面に立つ処理
	{
		// 自機の頭から足元への線分
		Segment playerSegment;

		// 自機のワールド座標を取得
		Vector3 trans = m_player->GetParts(Player::PLAYER_PARTS::PARTS_BODY).GetTranslate();

		playerSegment.start = trans + Vector3(0, 1, 0);
		playerSegment.end   = trans + Vector3(0, -0.5f, 0);

		// 交点座標
		Vector3 inter;

		// 地形と線分の当たり判定（レイキャスティング）
		if (m_landshape.IntersectSegment(playerSegment, &inter))
		{
			// y座標のみ交点の位置に移動
			trans.y = inter.y;
		}

		// 自機の移動
		m_player->GetParts(Player::PLAYER_PARTS::PARTS_BODY).SetTranslate(trans);
	}

	// 弾丸と敵の当たり判定
	{
		const Sphere& bulletSphere = m_player->GetCollisionNodeBullet();

		// 敵の数だけ処理する
		for (std::vector<std::unique_ptr<Enemy>>::iterator itr = m_enemy.begin(); itr != m_enemy.end();)
		{
			Enemy* enemy = itr->get();

			// 敵の判定球を取得
			const Sphere& enemySphere = enemy->GetCollisionNodeBody();

			// この２つが当たっていたら
			if (CheckSphere2Sphere(bulletSphere, enemySphere))
			{
				// パーティクルを出す
				m_effectManager->Entry(
					L"Resources/effect.cmo", 
					10, 
					Vector3((*itr)->GetParts(Enemy::PARTS_BODY).GetTranslate().x, (*itr)->GetParts(Enemy::PARTS_BODY).GetTranslate().y + 1.0f, (*itr)->GetParts(Enemy::PARTS_BODY).GetTranslate().z),
					Vector3(0.0f),
					Vector3(0.0f),
					(*itr)->GetParts(Enemy::PARTS_BODY).GetRotate(),
					(*itr)->GetParts(Enemy::PARTS_BODY).GetRotate(),
					Vector3(0.0f),
					Vector3(5.0f)
				);

				// 敵を消す
				itr = m_enemy.erase(itr);
			}
			else
			{
				// イテレータを一つ進める
				itr++;
			}
		}
	}

	// 敵を全員倒したらクリア（自動前宙）
	bool isClear = false;
	if (m_enemy.empty())
		isClear = true;

	// キー状態を取得
	Keyboard::State kb = m_keyboard->GetState();
	m_keyboardTracker->Update(kb);

	// オブジェクトステート
	{
		// 移動
		{
			if (!isClear)
			{
				// Ｗキーが押されたら前進
				if (kb.W)
					m_player->SetState(Player::STATE_MOVE_FORWARD, true);
				else
					m_player->SetState(Player::STATE_MOVE_FORWARD, false);

				// Ｓキーが押されたら後退
				if (kb.S)
					m_player->SetState(Player::STATE_MOVE_BACK, true);
				else
					m_player->SetState(Player::STATE_MOVE_BACK, false);

				// Ａキーが押されたら左旋回
				if (kb.A)
					m_player->SetState(Player::STATE_MOVE_LEFT, true);
				else
					m_player->SetState(Player::STATE_MOVE_LEFT, false);

				// Ｄキーが押されたら右旋回
				if (kb.D)
					m_player->SetState(Player::STATE_MOVE_RIGHT, true);
				else
					m_player->SetState(Player::STATE_MOVE_RIGHT, false);

				// Spaceキーが押されたら前方宙返り
				if (kb.Space)
					m_player->SetState(Player::STATE_TURN, true);

				// ↑キーが押されたら花を浮かせる
				if (kb.Up)
					m_player->SetState(Player::STATE_FLOAT, true);
				else
					m_player->SetState(Player::STATE_FLOAT, false);

				// ↓キーが押されたら股割り
				if (kb.Down)
					m_player->SetState(Player::STATE_SPLITS, true);
				else
					m_player->SetState(Player::STATE_SPLITS, false);

				// Ｂキーが押されたら弾を発射
				if (m_keyboardTracker->IsKeyPressed(Keyboard::Keys::B))
				{
					if (Player::m_isFire)	m_player->ResetBurret();
					else					m_player->FireBurret();
				}
			}
			else
			{
				// パーティクルの初期設定
				m_player->SetState(Player::STATE_TURN, true);
			}
		}
	}
}

// Draws the scene.
void Game::Render()
{
	// Don't try to render anything before the first Update.
	if (m_timer.GetFrameCount() == 0)
	{
		return;
	}

	Clear();

	// TODO: Add your rendering code here.

	/* 毎フレーム描画する処理はここへ */
	// 頂点インデックス
	uint16_t indices[] =
	{
		0, 1, 2,
		2, 1, 3
	};

	// 頂点座標
	VertexPositionNormal vertices[] =
	{
		{ Vector3(-1.0f,  1.0f,  0.0f), Vector3(0.0f,  0.0f,  1.0f) },
		{ Vector3(-1.0f, -1.0f,  0.0f), Vector3(0.0f,  0.0f,  1.0f) },
		{ Vector3(1.0f,  1.0f,  0.0f), Vector3(0.0f,  0.0f,  1.0f) },
		{ Vector3(1.0f, -1.0f,  0.0f), Vector3(0.0f,  0.0f,  1.0f) }
	};

	CommonStates states(m_d3dDevice.Get());
	m_d3dContext->OMSetBlendState(states.Opaque(), nullptr, 0xFFFFFFFF);
	m_d3dContext->OMSetDepthStencilState(states.DepthNone(), 0);
	m_d3dContext->RSSetState(states.CullNone());

	m_effect->SetWorld(m_world);
	m_effect->SetView(m_view);
	m_effect->SetProjection(m_proj);

	m_effect->Apply(m_d3dContext.Get());
	m_d3dContext->IASetInputLayout(m_inputLayout.Get());

	// モデルの描画
	m_objSkyDome.Draw();

	// 地形の描画
	m_landshape.Draw();

	// プレイヤーの描画
	m_player->Draw();

	// 敵の描画
	for (std::vector<std::unique_ptr<Enemy>>::iterator itr = m_enemy.begin(); itr != m_enemy.end(); itr++)
	{
		(*itr)->Draw();
	}

	// パーティクルの描画
	m_effectManager->Draw();

	m_batch->Begin();
	m_batch->End();

	Present();
}

// Helper method to clear the back buffers.
void Game::Clear()
{
	// Clear the views.
	m_d3dContext->ClearRenderTargetView(m_renderTargetView.Get(), Colors::CornflowerBlue);
	m_d3dContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	m_d3dContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

	// Set the viewport.
	CD3D11_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(m_outputWidth), static_cast<float>(m_outputHeight));
	m_d3dContext->RSSetViewports(1, &viewport);
}

// Presents the back buffer contents to the screen.
void Game::Present()
{
	// The first argument instructs DXGI to block until VSync, putting the application
	// to sleep until the next VSync. This ensures we don't waste any cycles rendering
	// frames that will never be displayed to the screen.
	HRESULT hr = m_swapChain->Present(1, 0);

	// If the device was reset we must completely reinitialize the renderer.
	if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
	{
		OnDeviceLost();
	}
	else
	{
		DX::ThrowIfFailed(hr);
	}
}

// Message handlers
void Game::OnActivated()
{
	// TODO: Game is becoming active window.
}

void Game::OnDeactivated()
{
	// TODO: Game is becoming background window.
}

void Game::OnSuspending()
{
	// TODO: Game is being power-suspended (or minimized).
}

void Game::OnResuming()
{
	m_timer.ResetElapsedTime();

	// TODO: Game is being power-resumed (or returning from minimize).
}

void Game::OnWindowSizeChanged(int width, int height)
{
	m_outputWidth = max(width, 1);
	m_outputHeight = max(height, 1);

	CreateResources();

	// TODO: Game window is being resized.
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const
{
	// TODO: Change to desired default window size (note minimum size is 320x200).
	width = 800;
	height = 600;
}

// These are the resources that depend on the device.
void Game::CreateDevice()
{
	UINT creationFlags = 0;

#ifdef _DEBUG
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	static const D3D_FEATURE_LEVEL featureLevels[] =
	{
		// TODO: Modify for supported Direct3D feature levels (see code below related to 11.1 fallback handling).
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};

	// Create the DX11 API device object, and get a corresponding context.
	HRESULT hr = D3D11CreateDevice(
		nullptr,                                // specify nullptr to use the default adapter
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		creationFlags,
		featureLevels,
		_countof(featureLevels),
		D3D11_SDK_VERSION,
		m_d3dDevice.ReleaseAndGetAddressOf(),   // returns the Direct3D device created
		&m_featureLevel,                        // returns feature level of device created
		m_d3dContext.ReleaseAndGetAddressOf()   // returns the device immediate context
	);

	if (hr == E_INVALIDARG)
	{
		// DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it.
		hr = D3D11CreateDevice(nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			creationFlags,
			&featureLevels[1],
			_countof(featureLevels) - 1,
			D3D11_SDK_VERSION,
			m_d3dDevice.ReleaseAndGetAddressOf(),
			&m_featureLevel,
			m_d3dContext.ReleaseAndGetAddressOf()
		);
	}

	DX::ThrowIfFailed(hr);

#ifndef NDEBUG
	ComPtr<ID3D11Debug> d3dDebug;
	if (SUCCEEDED(m_d3dDevice.As(&d3dDebug)))
	{
		ComPtr<ID3D11InfoQueue> d3dInfoQueue;
		if (SUCCEEDED(d3dDebug.As(&d3dInfoQueue)))
		{
#ifdef _DEBUG
			d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
			d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
#endif
			D3D11_MESSAGE_ID hide[] =
			{
				D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
				// TODO: Add more message IDs here as needed.
			};
			D3D11_INFO_QUEUE_FILTER filter = {};
			filter.DenyList.NumIDs = _countof(hide);
			filter.DenyList.pIDList = hide;
			d3dInfoQueue->AddStorageFilterEntries(&filter);
		}
	}
#endif

	// DirectX 11.1 if present
	if (SUCCEEDED(m_d3dDevice.As(&m_d3dDevice1)))
		(void)m_d3dContext.As(&m_d3dContext1);

	// TODO: Initialize device dependent objects here (independent of window size).
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateResources()
{
	// Clear the previous window size specific context.
	ID3D11RenderTargetView* nullViews[] = { nullptr };
	m_d3dContext->OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);
	m_renderTargetView.Reset();
	m_depthStencilView.Reset();
	m_d3dContext->Flush();

	UINT backBufferWidth = static_cast<UINT>(m_outputWidth);
	UINT backBufferHeight = static_cast<UINT>(m_outputHeight);
	DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
	DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	UINT backBufferCount = 2;

	// If the swap chain already exists, resize it, otherwise create one.
	if (m_swapChain)
	{
		HRESULT hr = m_swapChain->ResizeBuffers(backBufferCount, backBufferWidth, backBufferHeight, backBufferFormat, 0);

		if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
		{
			// If the device was removed for any reason, a new device and swap chain will need to be created.
			OnDeviceLost();

			// Everything is set up now. Do not continue execution of this method. OnDeviceLost will reenter this method
			// and correctly set up the new device.
			return;
		}
		else
		{
			DX::ThrowIfFailed(hr);
		}
	}
	else
	{
		// First, retrieve the underlying DXGI Device from the D3D Device.
		ComPtr<IDXGIDevice1> dxgiDevice;
		DX::ThrowIfFailed(m_d3dDevice.As(&dxgiDevice));

		// Identify the physical adapter (GPU or card) this device is running on.
		ComPtr<IDXGIAdapter> dxgiAdapter;
		DX::ThrowIfFailed(dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf()));

		// And obtain the factory object that created it.
		ComPtr<IDXGIFactory1> dxgiFactory;
		DX::ThrowIfFailed(dxgiAdapter->GetParent(IID_PPV_ARGS(dxgiFactory.GetAddressOf())));

		ComPtr<IDXGIFactory2> dxgiFactory2;
		if (SUCCEEDED(dxgiFactory.As(&dxgiFactory2)))
		{
			// DirectX 11.1 or later

			// Create a descriptor for the swap chain.
			DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };
			swapChainDesc.Width = backBufferWidth;
			swapChainDesc.Height = backBufferHeight;
			swapChainDesc.Format = backBufferFormat;
			swapChainDesc.SampleDesc.Count = 1;
			swapChainDesc.SampleDesc.Quality = 0;
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.BufferCount = backBufferCount;

			DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc = { 0 };
			fsSwapChainDesc.Windowed = TRUE;

			// Create a SwapChain from a Win32 window.
			DX::ThrowIfFailed(dxgiFactory2->CreateSwapChainForHwnd(
				m_d3dDevice.Get(),
				m_window,
				&swapChainDesc,
				&fsSwapChainDesc,
				nullptr,
				m_swapChain1.ReleaseAndGetAddressOf()
			));

			DX::ThrowIfFailed(m_swapChain1.As(&m_swapChain));
		}
		else
		{
			DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
			swapChainDesc.BufferCount = backBufferCount;
			swapChainDesc.BufferDesc.Width = backBufferWidth;
			swapChainDesc.BufferDesc.Height = backBufferHeight;
			swapChainDesc.BufferDesc.Format = backBufferFormat;
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.OutputWindow = m_window;
			swapChainDesc.SampleDesc.Count = 1;
			swapChainDesc.SampleDesc.Quality = 0;
			swapChainDesc.Windowed = TRUE;

			DX::ThrowIfFailed(dxgiFactory->CreateSwapChain(m_d3dDevice.Get(), &swapChainDesc, m_swapChain.ReleaseAndGetAddressOf()));
		}

		// This template does not support exclusive fullscreen mode and prevents DXGI from responding to the ALT+ENTER shortcut.
		DX::ThrowIfFailed(dxgiFactory->MakeWindowAssociation(m_window, DXGI_MWA_NO_ALT_ENTER));
	}

	// Obtain the backbuffer for this window which will be the final 3D rendertarget.
	ComPtr<ID3D11Texture2D> backBuffer;
	DX::ThrowIfFailed(m_swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf())));

	// Create a view interface on the rendertarget to use on bind.
	DX::ThrowIfFailed(m_d3dDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, m_renderTargetView.ReleaseAndGetAddressOf()));

	// Allocate a 2-D surface as the depth/stencil buffer and
	// create a DepthStencil view on this surface to use on bind.
	CD3D11_TEXTURE2D_DESC depthStencilDesc(depthBufferFormat, backBufferWidth, backBufferHeight, 1, 1, D3D11_BIND_DEPTH_STENCIL);

	ComPtr<ID3D11Texture2D> depthStencil;
	DX::ThrowIfFailed(m_d3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, depthStencil.GetAddressOf()));

	CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
	DX::ThrowIfFailed(m_d3dDevice->CreateDepthStencilView(depthStencil.Get(), &depthStencilViewDesc, m_depthStencilView.ReleaseAndGetAddressOf()));

	// TODO: Initialize windows-size dependent objects here.
}

void Game::OnDeviceLost()
{
	// TODO: Add Direct3D resource cleanup here.

	m_depthStencilView.Reset();
	m_renderTargetView.Reset();
	m_swapChain1.Reset();
	m_swapChain.Reset();
	m_d3dContext1.Reset();
	m_d3dContext.Reset();
	m_d3dDevice1.Reset();
	m_d3dDevice.Reset();

	CreateDevice();

	CreateResources();
}