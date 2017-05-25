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

	std::unique_ptr<DirectX::Model>		m_model;															// ３Ｄモデル

	DirectX::SimpleMath::Vector3		m_scale;															// スケーリング
	DirectX::SimpleMath::Vector3		m_rotate;															// 回転角
	DirectX::SimpleMath::Vector3		m_translate;														// 平行移動

	DirectX::SimpleMath::Matrix			m_world;															// ワールド行列
	Obj3d*								m_pParent;															// 親の３Ｄオブジェクト

public:
	Obj3d();																								// コンストラクタ

	void Update();																							// 更新
	void Draw();																							// 描画

	void LoadModel(const wchar_t* fileName);																// モデルのロード

	void SetScale(const DirectX::SimpleMath::Vector3& scale)			{ m_scale = scale; };				// スケーリングの設定
	void SetRotate(const DirectX::SimpleMath::Vector3& rotate)			{ m_rotate = rotate; };				// 回転角の設定
	void SetTranslate(const DirectX::SimpleMath::Vector3& translate)	{ m_translate = translate; };		// 平行移動の設定

	void SetParent(Obj3d* obj)											{ m_pParent = obj; };				// 親の３Ｄオブジェクトを設定

	const DirectX::SimpleMath::Vector3 GetScale()						{ return m_scale; };				// スケーリングを取得
	const DirectX::SimpleMath::Vector3 GetRotate()						{ return m_rotate; };				// 回転角を取得
	const DirectX::SimpleMath::Vector3 GetTranslate()					{ return m_translate; };			// 平行移動の取得

	const DirectX::SimpleMath::Matrix GetWorldMatrix()					{ return m_world; };				// ワールド行列の取得

	Obj3d* GetParent()													{ return m_pParent; };				// 親の３Ｄオブジェクトの取得
};