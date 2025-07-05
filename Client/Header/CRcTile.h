#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CRcTileTex;
	class CTransform;
	class CTexture;
	class CCalculator;
}

class CRcTile : public Engine::CGameObject
{
private:
	explicit CRcTile(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRcTile(const CGameObject& rhs);
	virtual ~CRcTile();

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
	Engine::CRcTileTex*		m_pBufferCom;
	Engine::CTransform*		m_pTransformCom;
	Engine::CTexture*		m_pTextureCom;
	Engine::CCalculator*	m_pCalculatorCom;
	_uint					m_iTextureNum;
public:
	static CRcTile* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();

};

