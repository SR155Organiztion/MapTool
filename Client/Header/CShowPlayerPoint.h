#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CCubeTex;
	class CTransform;
	class CTexture;
}

class CShowPlayerPoint : public Engine::CGameObject
{
private:
	explicit CShowPlayerPoint(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CShowPlayerPoint(const CGameObject& rhs);
	virtual ~CShowPlayerPoint();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

private:
	HRESULT		Add_Component();
	HRESULT		Set_Metarial();

	void		Set_Greed(_vec3 _v);

private:
	Engine::CCubeTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

public:
	static CShowPlayerPoint* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();

};

