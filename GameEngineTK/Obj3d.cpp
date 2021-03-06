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
	// スケールの初期化
	m_scale = Vector3(1.0f, 1.0f, 1.0f);

	// 親の３Ｄオブジェクトを初期化
	m_pParent = nullptr;

	// デフォルトではオイラー角で回転角を計算
	m_useQuaternion = false;
}

/// <summary>
/// 更新処理
/// </summary>
void Obj3d::Update()
{
	// 行列の計算
	{
		// スケーリング行列
		Matrix scaleMat = Matrix::CreateScale(m_scale);

		// 回転行列
		Matrix rotMat;
		if (m_useQuaternion)			// クォータニオンで計算
		{
			rotMat = Matrix::CreateFromQuaternion(m_quaternion);
		}
		else							// オイラー角で計算
		{
			Matrix rotMatZ = Matrix::CreateRotationZ(m_rotate.z);
			Matrix rotMatX = Matrix::CreateRotationX(m_rotate.x);
			Matrix rotMatY = Matrix::CreateRotationY(m_rotate.y);
			rotMat = rotMatZ * rotMatX * rotMatY;
		}

		// 平行移動行列
		Matrix transMat = Matrix::CreateTranslation(m_translate);

		// ワールド行列の合成
		m_world = scaleMat * rotMat * transMat;

		// 親の行列をかける（子の場合）
		if (m_pParent) m_world *= m_pParent->GetWorldMatrix();
	}
}

/// <summary>
/// 描画処理
/// </summary>
void Obj3d::Draw()
{
	if (m_model)
	{
		// ３Ｄモデルの描画
		m_model->Draw(
			m_d3dContext.Get(),
			*m_states,
			m_world,
			m_camera->GetViewMatrix(),
			m_camera->GetProjectionMatrix()
		);
	}
}

/// <summary>
/// モデルのロード
/// </summary>
/// <param name="fileName">モデルのファイル名</param>
void Obj3d::LoadModel(const wchar_t* fileName)
{
	// モデルの読み込み
	m_model = Model::CreateFromCMO(
		m_d3dDevice.Get(),
		fileName,
		*m_factory
	);
}

/// <summary>
/// オブジェクトのライティングを無効にする
/// </summary>
void Obj3d::DisableLighting()
{
	if (m_model)
	{
		// モデル内の全メッシュ分回す
		ModelMesh::Collection::const_iterator it_mesh = m_model->meshes.begin();
		for (; it_mesh != m_model->meshes.end(); it_mesh++)
		{
			ModelMesh* modelmesh = it_mesh->get();
			assert(modelmesh);

			// メッシュ内の全メッシュパーツ分回す
			std::vector<std::unique_ptr<ModelMeshPart>>::iterator it_meshpart = modelmesh->meshParts.begin();
			for (; it_meshpart != modelmesh->meshParts.end(); it_meshpart++)
			{
				ModelMeshPart* meshpart = it_meshpart->get();
				assert(meshpart);

				// メッシュパーツにセットされたエフェクトをBasicEffectとして取得
				std::shared_ptr<IEffect> ieff = meshpart->effect;
				BasicEffect* eff = dynamic_cast<BasicEffect*>(ieff.get());
				if (eff != nullptr)
				{
					// 自己発光を最大値に
					eff->SetEmissiveColor(Vector3(1, 1, 1));

					// エフェクトに含む全ての平行光源分について処理する
					for (int i = 0; i < BasicEffect::MaxDirectionalLights; i++)
					{
						// ライトを無効にする
						eff->SetLightEnabled(i, false);
					}
				}
			}
		}
	}
}