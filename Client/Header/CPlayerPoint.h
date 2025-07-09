#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CCubeTex;
	class CTransform;
	class CTexture;
	class CCalculator;
}

class CPlayerPoint : public Engine::CGameObject
{
private:
	explicit CPlayerPoint(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPlayerPoint(const CGameObject& rhs);
	virtual ~CPlayerPoint();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

	void						Set_PlayerNum(_uint _eID);
	void						Set_Pos(_vec3 _vPos);
	void						Set_Plant(_bool _b);
private:
	HRESULT		Add_Component();
	HRESULT		Set_Metarial();

private:
	Engine::CCubeTex*	m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture*	m_pTextureCom;
	
	_uint				m_iPlayerNum;
	_bool				m_bPlanted;

public:
	static CPlayerPoint* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();

};

