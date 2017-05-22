//
// Obj3d.h
// ３Ｄオブジェクトのクラス
//
#pragma once

// ヘッダファイルのインクルード
#include <d3d11_1.h>
#include <Windows.h>
#include <wrl\client.h>

#include <CommonStates.h>
#include <Effects.h>
#include <SimpleMath.h>
#include <Model.h>

#include "Camera.h"

// クラスの定義
class Obj3d
{
private:
	static Microsoft::WRL::ComPtr<ID3D11Device>            m_d3dDevice;			// デバイス
	static Microsoft::WRL::ComPtr<ID3D11DeviceContext>     m_d3dContext;		// デバイスコンテキスト

	static Camera* m_camera;													// カメラ

	static std::unique_ptr<DirectX::CommonStates> m_states;						// 汎用描画ステート

	static std::unique_ptr<DirectX::EffectFactory> m_factory;					// エフェクトファクトリ

public:
	// 静的メンバ変数の初期化
	static void InitializeStatic(
		Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice, 
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext,
		Camera* camera
	);

private:
	// スケーリング
	// 回転角
	// ワールド行列

public:
	Obj3d();										// コンストラクタ

	void Update();									// 更新
	void Draw();									// 描画

	void LoadModel(const wchar_t* fileName);		// モデルのロード

	// スケーリングの設定
	// 回転角の設定
	// 平行移動の設定

	// スケーリングを取得
	// 回転角を取得
	// 平行移動の取得
	// ワールド行列の取得

};