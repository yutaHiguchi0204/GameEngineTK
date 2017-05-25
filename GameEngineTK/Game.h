//
// Game.h
//

#pragma once

#include <CommonStates.h>
#include <Effects.h>
#include <Keyboard.h>
#include <Model.h>
#include <PrimitiveBatch.h>
#include <SimpleMath.h>
#include <vector>
#include <VertexTypes.h>

#include "Obj3d.h"
#include "FollowCamera.h"
#include "DebugCamera.h"

#include "StepTimer.h"


// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game
{
public:

	enum PLAYER_PARTS
	{
		PARTS_TANK,
		PARTS_BODY,
		PARTS_HEAD,
		PARTS_WEAPON,

		PARTS_NUM
	};

	wchar_t* PARTS_NAME[PARTS_NUM] = {
		L"tank",
		L"body",
		L"head",
		L"weapon"
	};

    Game();

    // Initialization and management
    void Initialize(HWND window, int width, int height);

    // Basic game loop
    void Tick();

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowSizeChanged(int width, int height);

    // Properties
    void GetDefaultSize( int& width, int& height ) const;

private:

    void Update(DX::StepTimer const& timer);
    void Render();

    void Clear();
    void Present();

    void CreateDevice();
    void CreateResources();

    void OnDeviceLost();

    // Device resources.
    HWND                                            m_window;
    int                                             m_outputWidth;
    int                                             m_outputHeight;

    D3D_FEATURE_LEVEL                               m_featureLevel;
    Microsoft::WRL::ComPtr<ID3D11Device>            m_d3dDevice;
    Microsoft::WRL::ComPtr<ID3D11Device1>           m_d3dDevice1;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext>     m_d3dContext;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext1>    m_d3dContext1;

    Microsoft::WRL::ComPtr<IDXGISwapChain>          m_swapChain;
    Microsoft::WRL::ComPtr<IDXGISwapChain1>         m_swapChain1;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  m_renderTargetView;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_depthStencilView;

    // Rendering loop timer.
    DX::StepTimer                                   m_timer;

	//std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_batch;	// プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionNormal>> m_batch;	// プリミティブバッチ
	std::unique_ptr<DirectX::BasicEffect> m_effect;										// エフェクト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;							// レイアウト

	DirectX::SimpleMath::Matrix m_world;												// ワールド行列
	DirectX::SimpleMath::Matrix m_view;													// ビュー行列
	DirectX::SimpleMath::Matrix m_proj;													// プロジェクション行列

	std::unique_ptr<FollowCamera> m_camera;												// カメラ
	std::unique_ptr<DebugCamera> m_debugCamera;											// デバッグカメラ
	std::unique_ptr<DirectX::Keyboard> m_keyboard;										// キーボード

	std::unique_ptr<DirectX::EffectFactory> m_factory;									// エフェクトファクトリ
	Obj3d m_objGround;																	// 地面の３Ｄオブジェクト
	Obj3d m_objSkyDome;																	// 天球の３Ｄオブジェクト
	//std::unique_ptr<DirectX::Model> m_modelBall;										// ボールモデル
	//std::unique_ptr<DirectX::Model> m_modelTeaPot;									// ティーポットモデル
	Obj3d m_objTank;																	// タンクの３Ｄオブジェクト
	Obj3d m_objBody;																	// 胴体の３Ｄオブジェクト

	DirectX::SimpleMath::Vector3 m_tankPos;												// タンクの位置
	float m_tankRotate;																	// タンクの角度

	//DirectX::SimpleMath::Matrix m_worldBall;											// 球ワールド行列
	//DirectX::SimpleMath::Matrix m_worldTeaPot;										// ティーポットワールド行列
	int m_time;																			// 時間

	std::vector<Obj3d> m_objPlayer;														// 自機の３Ｄオブジェクト

	//int m_distance[20];																// ティーポット用距離
	//int m_digree[20];																	// ティーポット用角度
};