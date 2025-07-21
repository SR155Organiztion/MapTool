#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CCubeTex;
	class CTransform;
	class CTexture;
	class CCalculator;
}

class CEnvCube : public Engine::CGameObject
{
private:
	explicit CEnvCube(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEnvCube(const CGameObject& rhs);
	virtual ~CEnvCube();

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
	Engine::CCubeTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;

	_uint					m_iTextureNum;
public:
	static CEnvCube* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual		void		Free();

};

