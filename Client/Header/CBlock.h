#pragma once
#include "CGameObject.h"
#include "CRcTile.h"
namespace Engine
{
	class CCubeTex;
	class CTransform;
	class CTexture;
	class CCalculator;
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

	void						Set_TextureNum(_uint _eID);
	void						Set_Create(_uint _eID);
	void						Set_Tools(_uint _eID);
private:
	HRESULT		Add_Component();
	HRESULT		Set_Metarial();

private:
	Engine::CCubeTex*		m_pBufferCom;
	Engine::CTransform*		m_pTransformCom;
	Engine::CTexture*		m_pTextureCom;
	Engine::CCalculator*	m_pCalculatorCom;
	_uint					m_iTextureNum;

	_bool				m_bShow;
	Engine::CRcTileTex* m_pRcTileCom;
	Engine::CTexture*	m_pRcTileTextureCom;
	_uint				m_iTileTexNum;
	
public:
	static CBlock* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();

};

