//
// Obj3d.h
// �R�c�I�u�W�F�N�g�̃N���X
//

// �w�b�_�t�@�C���̃C���N���[�h
#include "Obj3d.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace std;

// �ÓI�����o�̒�`
Microsoft::WRL::ComPtr<ID3D11Device> Obj3d::m_d3dDevice;			// �f�o�C�X
Microsoft::WRL::ComPtr<ID3D11DeviceContext> Obj3d::m_d3dContext;	// �f�o�C�X�R���e�L�X�g
Camera* Obj3d::m_camera;											// �J����
std::unique_ptr<DirectX::CommonStates> Obj3d::m_states;				// �ėp�`��X�e�[�g
std::unique_ptr<DirectX::EffectFactory> Obj3d::m_factory;			// �G�t�F�N�g�t�@�N�g��

/// <summary>
/// �ÓI�����o�ϐ��̏�����
/// </summary>
/// <param name="d3dDevice">�f�o�C�X</param>
/// <param name="d3dContext">�f�o�C�X�R���e�L�X�g</param>
/// <param name="camera">�J����</param>
void Obj3d::InitializeStatic(Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext, Camera* camera)
{
	m_d3dDevice = d3dDevice;
	m_d3dContext = d3dContext;
	m_camera = camera;

	// �ėp�`��X�e�[�g����
	m_states = make_unique<CommonStates>(m_d3dDevice.Get());

	// �G�t�F�N�g�t�@�N�g������
	m_factory = make_unique<EffectFactory>(m_d3dDevice.Get());
	m_factory->SetDirectory(L"Resources");
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
Obj3d::Obj3d()
{
	// �X�P�[���̏�����
	m_scale = Vector3(1.0f, 1.0f, 1.0f);

	// �e�̂R�c�I�u�W�F�N�g��������
	m_pParent = nullptr;
}

/// <summary>
/// �X�V����
/// </summary>
void Obj3d::Update()
{
	// �s��̌v�Z
	{
		// �X�P�[�����O�s��
		Matrix scaleMat = Matrix::CreateScale(m_scale);

		// ��]�s��
		Matrix rotMatZ = Matrix::CreateRotationZ(m_rotate.z);
		Matrix rotMatX = Matrix::CreateRotationX(m_rotate.x);
		Matrix rotMatY = Matrix::CreateRotationY(m_rotate.y);
		Matrix rotMat = rotMatZ * rotMatX * rotMatY;

		// ���s�ړ��s��
		Matrix transMat = Matrix::CreateTranslation(m_translate);

		// ���[���h�s��̍���
		m_world = scaleMat * rotMat * transMat;

		// �e�̍s���������i�q�̏ꍇ�j
		if (m_pParent) m_world *= m_pParent->GetWorldMatrix();
	}
}

/// <summary>
/// �`�揈��
/// </summary>
void Obj3d::Draw()
{
	if (m_model)
	{
		// �R�c���f���̕`��
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
/// ���f���̃��[�h
/// </summary>
/// <param name="fileName">���f���̃t�@�C����</param>
void Obj3d::LoadModel(const wchar_t* fileName)
{
	// ���f���̓ǂݍ���
	m_model = Model::CreateFromCMO(
		m_d3dDevice.Get(),
		fileName,
		*m_factory
	);
}