#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CTerrainTex;
	class CTransform;
	class CTexture;
	class CCalculator;
}

class CTerrain : public Engine::CGameObject
{
private:
	explicit CTerrain(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTerrain(const CGameObject& rhs);
	virtual ~CTerrain();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

	void		Set_Enable() { m_bEnable ? m_bEnable = false : m_bEnable = true; }
private:
	HRESULT		Add_Component();
	HRESULT		Set_Material();
	

private:
	Engine::CTerrainTex* m_pBufferCom;
	Engine::CTransform*	 m_pTransformCom;
	Engine::CTexture*	 m_pTextureCom;
	Engine::CCalculator* m_pCalculatorCom;

	_bool	m_bEnable;
	
public:
	static CTerrain* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();

};

