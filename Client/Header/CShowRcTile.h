#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CRcTileTex;
	class CTransform;
	class CTexture;
}

class CShowRcTile : public Engine::CGameObject
{
private:
	explicit CShowRcTile(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CShowRcTile(const CGameObject& rhs);
	virtual ~CShowRcTile();

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
	Engine::CRcTileTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture*	m_pTextureCom;
	_bool		m_bClicked;

public:
	static CShowRcTile* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();

};

