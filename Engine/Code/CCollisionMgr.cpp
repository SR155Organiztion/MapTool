#include "CCollisionMgr.h"

IMPLEMENT_SINGLETON(CCollisionMgr)

CCollisionMgr::CCollisionMgr()
{
    ZeroMemory(&m_vRayPos, sizeof(_vec3));
    ZeroMemory(&m_vRayDir, sizeof(_vec3));
    ZeroMemory(&m_vColPos, sizeof(_vec3));
}

CCollisionMgr::~CCollisionMgr()
{
    Free();
}

void CCollisionMgr::Compute_Ray(LPDIRECT3DDEVICE9 pGraphicDev, HWND hWnd)
{
	POINT	ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);

	// 뷰포트 -> 투영

	_vec3		vMousePos;

	D3DVIEWPORT9		ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));

	pGraphicDev->GetViewport(&ViewPort);

	vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vMousePos.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
	vMousePos.z = 1.f;

	// 투영 -> 뷰 스페이스
	D3DXMATRIX	matProj;
	pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, 0, &matProj);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	// 뷰 스페이스 -> 월드

	D3DXMATRIX	matView;
	pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, 0, &matView);

	_vec3		vRayPos{ 0.f, 0.f, 0.f };
	_vec3		vRayDir = vMousePos - vRayPos;

	D3DXVec3TransformCoord(&m_vRayPos, &vRayPos, &matView);
	D3DXVec3TransformNormal(&m_vRayDir, &vRayDir, &matView);
}

void CCollisionMgr::Get_Ray(_vec3* vRayPos, _vec3* vRayDir)
{
	*vRayPos = m_vRayPos;
	*vRayDir = m_vRayDir;
}

void CCollisionMgr::Set_ColPos(_vec3 _v)
{
    m_vColPos = _v;
}

_vec3 CCollisionMgr::Get_ColPos()
{
    return m_vColPos;
}

void CCollisionMgr::Free()
{

}
