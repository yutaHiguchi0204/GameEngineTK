//
// CollisionNode.cpp
// �Փ˔���m�[�h
//

// �w�b�_�t�@�C���̃C���N���[�h
#include "CollisionNode.h"

// ���O���
using namespace DirectX::SimpleMath;

/// <summary>
/// CollisionNode�̐e�ݒ�֐�
/// </summary>
/// <param name="parent">�e</param>
void CollisionNode::SetParent(Obj3d* parent)
{
	m_obj.SetParent(parent);
}

/// <summary>
/// SphereNode�̃R���X�g���N�^
/// </summary>
SphereNode::SphereNode()
{
	m_radius = 1.0f;
}

/// <summary>
/// SphereNode�̏�����
/// </summary>
void SphereNode::Initialize()
{
	// �f�o�b�O�\���p���f����ǂݍ���
	m_obj.LoadModel(L"Resources/sphereNode.cmo");
}

/// <summary>
/// SphereNode�̍X�V
/// </summary>
void SphereNode::Update()
{
	m_obj.SetTranslate(m_trans);
	m_obj.SetScale(Vector3(m_radius));

	m_obj.Update();
}

/// <summary>
/// SphereNode�̕`��
/// </summary>
void SphereNode::Draw()
{
	m_obj.Draw();
}