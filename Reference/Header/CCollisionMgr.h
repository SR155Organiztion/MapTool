#pragma once
#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CCollisionMgr : public CBase
{
	DECLARE_SINGLETON(CCollisionMgr)

private:
	explicit CCollisionMgr();
	virtual ~CCollisionMgr();

public:
	void		Compute_Ray(LPDIRECT3DDEVICE9 pGraphicDev, HWND hWnd);
	void		Get_Ray(_vec3* vRayPos, _vec3* vRayDir);
	void		Set_ColPos(_vec3 _v);
	_vec3		Get_ColPos();

private:
	_vec3 m_vRayPos;
	_vec3 m_vRayDir;
	_vec3 m_vColPos;

private:
	virtual void		Free();

};

END
