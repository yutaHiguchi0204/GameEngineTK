//
// CollisionNode.h
// �Փ˔���m�[�h
//
#pragma once

// �w�b�_�t�@�C���̃C���N���[�h
#include "Collision.h"
#include "Obj3d.h"

// �N���X�̒�`
class CollisionNode
{
protected:
	Obj3d m_obj;
	DirectX::SimpleMath::Vector3 m_trans;

public:
	virtual void Initialize() = 0;				// ����������
	virtual void Update() = 0;					// �X�V����
	virtual void Draw() = 0;					// �`�揈��

	void SetParent(Obj3d* parent);

	void SetTrans(const DirectX::SimpleMath::Vector3& trans) { m_trans = trans; };
};

// ���̓����蔻��m�[�h
class SphereNode : public CollisionNode, public Sphere
{
protected:
	float m_radius;					// ���[�J�����a

public:
	SphereNode();

	void Initialize();				// ����������
	void Update();					// �X�V����
	void Draw();					// �`�揈��

	void SetLocalRadius(float radius) { m_radius = radius; };
};