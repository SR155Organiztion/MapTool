#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CTerrainCol;
	class CTransform;
	class CTexture;
}

class CTestTerrain : public Engine::CGameObject
{
private:
	explicit CTestTerrain(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTestTerrain(const CGameObject& rhs);
	virtual ~CTestTerrain();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

private:
	HRESULT		Add_Component();

private:
	Engine::CTerrainCol *m_pBufferCom;
	Engine::CTransform* m_pTransformCom;

public:
	static CTestTerrain* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();

};

