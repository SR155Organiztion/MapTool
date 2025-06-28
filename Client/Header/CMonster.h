#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CTriCol;
	class CTransform;
}

class CMonster : public Engine::CGameObject
{
private:
	explicit CMonster(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMonster(const CGameObject& rhs);
	virtual ~CMonster();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

private:
	HRESULT		Add_Component();
	HRESULT		Set_Metarial();
private:
	Engine::CTriCol*		m_pBufferCom;
	Engine::CTransform*		m_pTransformCom;

public:
	static CMonster* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();

};

