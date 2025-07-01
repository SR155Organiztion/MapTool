#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CCubeTex;
	class CTransform;
	class CTexture;
}

class CBlock : public Engine::CGameObject
{
private:
	explicit CBlock(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBlock(const CGameObject& rhs);
	virtual ~CBlock();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

private:
	HRESULT		Add_Component();
	HRESULT		Set_Metarial();

private:
	Engine::CCubeTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

public:
	static CBlock* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();

};

