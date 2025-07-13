#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CRcTex;
	class CTransform;
	class CTexture;
	class CCalculator;
}

class CEnvObject : public Engine::CGameObject
{
private:
	explicit CEnvObject(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEnvObject(const CGameObject& rhs);
	virtual ~CEnvObject();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

	void						Set_TextureNum(_uint _iID);
private:
	HRESULT		Add_Component();
	HRESULT		Set_Metarial();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture*   m_pTextureCom;

	_uint					m_iTextureNum;
public:
	static CEnvObject* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();

};

