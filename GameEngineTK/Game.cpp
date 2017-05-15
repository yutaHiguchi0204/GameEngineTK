//
// Game.cpp
//

#include <cstdlib>
#include <ctime>
#include "pch.h"
#include "Game.h"

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

	// カメラの生成
	m_camera = make_unique<FollowCamera>(m_outputWidth, m_outputHeight);

	// デバッグカメラの生成
	m_debugCamera = make_unique<DebugCamera>(m_outputWidth, m_outputHeight);

	// キーボード設定
	m_keyboard = std::make_unique<Keyboard>();

	// エフェクトファクトリの生成
	m_factory = std::make_unique<EffectFactory>(m_d3dDevice.Get());

	// テクスチャの読み取りパス指定
	m_factory->SetDirectory(L"Resources");

	// モデルの読み込み
	m_modelGround = Model::CreateFromCMO(m_d3dDevice.Get(), L"Resources/ground_200m.cmo", *m_factory);
	m_modelSkyDome = Model::CreateFromCMO(m_d3dDevice.Get(), L"Resources/skyDome.cmo", *m_factory);
	//m_modelBall = Model::CreateFromCMO(m_d3dDevice.Get(), L"Resources/ball.cmo", *m_factory);
	//m_modelTeaPot = Model::CreateFromCMO(m_d3dDevice.Get(), L"Resources/teaPot.cmo", *m_factory);
	m_modelTank = Model::CreateFromCMO(m_d3dDevice.Get(), L"Resources/tank.cmo", *m_factory);

	// 時間の初期化
	m_time = 0;

	// ランダムでティーポットの距離と角度を設定
	for (int i = 0; i < 20; i++)
	{
		m_distance[i] = rand() % 100;
		m_digree[i] = rand() % 360;
	}

	m_tankRotate = 0.0f;

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

	//// ビュー行列の取得
	//m_view = m_debugCamera->GetCameraMatrix();
	////Vector3 eyePos(0.0f, 0.0f, 5.0f);				// 視点
	////Vector3 refPos(0.0f, 0.0f, 0.0f);				// 注視点（参照点）
	////Vector3 upVec(0.0f, 1.0f, 0.0f);				// 上方向ベクトル
	////m_view = Matrix::CreateLookAt(eyePos, refPos, upVec);

	//// プロジェクション行列の取得
	//float fovY = XMConvertToRadians(60.0f);						// 垂直方向視野角
	//float aspect = (float)m_outputWidth / m_outputHeight;		// アスペクト比
	//float nearClip = 0.1f;										// 手前の表示限界距離
	//float farClip = 1000.0f;									// 奥の表示限界距離
	//m_proj = Matrix::CreatePerspectiveFieldOfView(fovY, aspect, nearClip, farClip);

	// ゴムひもカメラ設定
	{
		m_camera->SetTargetPos(m_tankPos);
		m_camera->SetTargetAngle(m_tankRotate);

		// カメラの更新
		m_camera->Update();
		m_view = m_camera->GetViewMatrix();
		m_proj = m_camera->GetProjectionMatrix();
	}

	// 球のワールド行列の計算
	Matrix scaleMat = Matrix::CreateScale(2.0f);

	// キー状態を取得
	Keyboard::State kb = m_keyboard->GetState();

	// Ｗキーが押されたら
	if (kb.W)
	{
		// 移動量ベクトルによるタンクの移動
		Vector3 moveV(0.0f, 0.0f, -0.1f);

		// 移動量ベクトルを自機の角度分回転させる
		Matrix rotMat = Matrix::CreateRotationY(XMConvertToRadians(m_tankRotate));
		moveV = Vector3::TransformNormal(moveV, rotMat);

		m_tankPos += moveV;
	}

	// Ｓキーが押されたら
	if (kb.S)
	{
		// 移動量ベクトルによるタンクの移動
		Vector3 moveV(0.0f, 0.0f, 0.1f);

		// 移動量ベクトルを自機の角度分回転させる
		Matrix rotMat = Matrix::CreateRotationY(XMConvertToRadians(m_tankRotate));
		moveV = Vector3::TransformNormal(moveV, rotMat);

		m_tankPos += moveV;
	}

	// Ａキーが押されたら
	if (kb.A)
	{
		// 回転ベクトルによるタンクの回転
		m_tankRotate++;
	}

	// Ｄキーが押されたら
	if (kb.D)
	{
		// 回転ベクトルによるタンクの回転
		m_tankRotate--;
	}

	// 時間計測
	m_time++;
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
	m_modelGround->Draw(m_d3dContext.Get(), states, m_world, m_view, m_proj);
	m_modelSkyDome->Draw(m_d3dContext.Get(), states, m_world, m_view, m_proj);

	// スケーリングの初期設定
	//Matrix scaleMat = Matrix::CreateScale(1.0f);
	// 計算式　-1 ～ 1　→　0 ～ 2　→　0 ～ 4　→　1 ～ 5（倍）
	//Matrix scaleMat = Matrix::CreateScale((sinf(m_time / 90.0f) + 1.0f) * 2.0f + 1.0f);

	// 同心円状に20個配置
	//for (int i = 0; i < 20; i++)
	//{
	//	// 平行移動
	//	Matrix transMat = Matrix::CreateTranslation((i / 10 + 1) * 20.0f, 0.0f, 0.0f);

	//	// 回転
	//	Matrix rotMatZ = Matrix::CreateRotationZ(XMConvertToRadians(0.0f));
	//	Matrix rotMatX = Matrix::CreateRotationX(XMConvertToRadians(0.0f));
	//	Matrix rotMatY = Matrix::CreateRotationY(XMConvertToRadians(i * (360 / 10) + (i / 10 * 2 - 1) * m_time));
	//	Matrix rotMat = rotMatZ * rotMatX * rotMatY;

	//	// ワールド行列を計算
	//	//m_worldBall = scaleMat * transMat * rotMat;

	//	// ボールを描画
	//	//m_modelBall->Draw(m_d3dContext.Get(), states, m_worldBall, m_view, m_proj);
	//}

	//Matrix rotateY = Matrix::CreateRotationY(XMConvertToRadians(m_time));

	// ランダムにティーポットを20個表示
	//for (int i = 0; i < 20; i++)
	//{
	//	Matrix transMat;

	//	// 平行移動
	//	/*transMat = Matrix::CreateTranslation(
	//		cosf(XMConvertToRadians(m_digree[i])) * m_distance[i], 
	//		0.0f, 
	//		sinf(XMConvertToRadians(m_digree[i])) * m_distance[i]
	//	);*/

	//	// 中心に向かって平行移動
	//	if (m_time < 10 * 60.0f)
	//	{
	//		transMat = Matrix::CreateTranslation(
	//			(10 * 60.0f - m_time) / (10 * 60.0f) * (cosf(XMConvertToRadians(m_digree[i])) * m_distance[i]), 
	//			0.0f, 
	//			(10 * 60.0f - m_time) / (10 * 60.0f) * (sinf(XMConvertToRadians(m_digree[i])) * m_distance[i])
	//		);
	//	}
	//	else
	//	{
	//		transMat = Matrix::Identity;
	//	}

	//	// 回転
	//	/*Matrix rotMatZ = Matrix::CreateRotationZ(0.0f);
	//	Matrix rotMatX = Matrix::CreateRotationX(0.0f);
	//	Matrix rotMatY = Matrix::CreateRotationY(m_digree[i]);
	//	Matrix rotMat = rotMatZ * rotMatX * rotMatY;*/

	//	// ワールド行列を計算
	//	m_worldTeaPot = scaleMat * rotateY * transMat;

	//	// ティーポットを描画
	//	m_modelTeaPot->Draw(m_d3dContext.Get(), states, m_worldTeaPot, m_view, m_proj);
	//}

	// タンクのワールド行列を設定
	Matrix rotateMatTank = Matrix::CreateRotationY(XMConvertToRadians(m_tankRotate));
	Matrix transMatTank = Matrix::CreateTranslation(m_tankPos);
	m_worldTank = rotateMatTank * transMatTank;

	// タンクの描画
	m_modelTank->Draw(m_d3dContext.Get(), states, m_worldTank, m_view, m_proj);

	m_batch->Begin();

	/*m_batch->DrawLine(
		VertexPositionColor(Vector3(0, 0, 0), Color(0, 0, 0)), 
		VertexPositionColor(Vector3(m_outputWidth, m_outputHeight, 0), Color(0, 0, 0))
	);*/

	/*VertexPositionColor v1(Vector3(0.f, 0.5f, 0.5f), Colors::Yellow);
	VertexPositionColor v2(Vector3(0.5f, -0.5f, 0.5f), Colors::Yellow);
	VertexPositionColor v3(Vector3(-0.5f, -0.5f, 0.5f), Colors::Yellow);

	m_batch->DrawTriangle(v1, v2, v3);*/

	//m_batch->DrawIndexed(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, indices, 6, vertices, 4);

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

    static const D3D_FEATURE_LEVEL featureLevels [] =
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
            D3D11_MESSAGE_ID hide [] =
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
    ID3D11RenderTargetView* nullViews [] = { nullptr };
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