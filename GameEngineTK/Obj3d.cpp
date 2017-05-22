//
// Obj3d.h
// ３Ｄオブジェクトのクラス
//

// ヘッダファイルのインクルード
#include "Obj3d.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace std;

// 静的メンバの定義
Microsoft::WRL::ComPtr<ID3D11Device> Obj3d::m_d3dDevice;			// デバイス
Microsoft::WRL::ComPtr<ID3D11DeviceContext> Obj3d::m_d3dContext;	// デバイスコンテキスト
Camera* Obj3d::m_camera;											// カメラ
std::unique_ptr<DirectX::CommonStates> Obj3d::m_states;				// 汎用描画ステート
std::unique_ptr<DirectX::EffectFactory> Obj3d::m_factory;			// エフェクトファクトリ

/// <summary>
/// 静的メンバ変数の初期化
/// </summary>
/// <param name="d3dDevice">デバイス</param>
/// <param name="d3dContext">デバイスコンテキスト</param>
/// <param name="camera">カメラ</param>
void Obj3d::InitializeStatic(Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext, Camera* camera)
{
	m_d3dDevice = d3dDevice;
	m_d3dContext = d3dContext;
	m_camera = camera;

	// 汎用描画ステート生成
	m_states = make_unique<CommonStates>(m_d3dDevice.Get());

	// エフェクトファクトリ生成
	m_factory = make_unique<EffectFactory>(m_d3dDevice.Get());
	m_factory->SetDirectory(L"Resources");
}

/// <summary>
/// コンストラクタ
/// </summary>
Obj3d::Obj3d()
{

}

/// <summary>
/// 更新処理
/// </summary>
void Obj3d::Update()
{

}

/// <summary>
/// 描画処理
/// </summary>
void Obj3d::Draw()
{

}

void Obj3d::LoadModel(const wchar_t * fileName)
{
}
