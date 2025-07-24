#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CRcTileTex;
	class CTransform;
	class CTexture;
	class CCalculator;
}

class CEnvTile : public Engine::CGameObject
{
private:
	explicit CEnvTile(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEnvTile(const CGameObject& rhs);
	virtual ~CEnvTile();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

	void						Set_TextureNum(_uint _iID);
	void						Set_Angle(_float _fAngle);
	void						Set_Scale(_vec3  _vScale);
private:
	HRESULT		Add_Component();
	HRESULT		Set_Metarial();

private:
	Engine::CRcTileTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

	_uint					m_iTextureNum;
public:
	static CEnvTile* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();

};

