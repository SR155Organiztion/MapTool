#include "pch.h"
#include "CDynamicCamera.h"
#include "CDInputMgr.h"
#include "CTransform.h"
#include "CTerrainTex.h"
#include "CManagement.h"
#include "CTerrain.h"
#include "CCalculator.h"
#include "CProtoMgr.h"
#include "CMapToolMgr.h"

CDynamicCamera::CDynamicCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CCamera(pGraphicDev), m_bFix(false), m_bCheck(false)
{
}

CDynamicCamera::~CDynamicCamera()
{
}

HRESULT CDynamicCamera::Ready_GameObject(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar)
{
	m_vEye = *pEye;
	m_vAt = *pAt;
	m_vUp = *pUp;

	m_fFov = fFov;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;

	m_pCalculatorCom = dynamic_cast<Engine::CCalculator*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Calculator"));
	if (nullptr == m_pCalculatorCom)
		return E_FAIL;
	m_mapComponent[ID_STATIC].insert({ L"Com_Calculator", m_pCalculatorCom });

	if (FAILED(Engine::CCamera::Ready_GameObject()))
		return E_FAIL;

	m_fSpeed = 5.f;


	return S_OK;
}

_int CDynamicCamera::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = Engine::CCamera::Update_GameObject(fTimeDelta);

	Key_Input(fTimeDelta);

	return iExit;
}

void CDynamicCamera::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CCamera::LateUpdate_GameObject(fTimeDelta);

	if (false == m_bFix)
	{
		Mouse_Move();
		Mouse_Fix();
	}
}

void CDynamicCamera::Key_Input(const _float& fTimeDelta)
{
	_matrix	matCamWorld;
	D3DXMatrixInverse(&matCamWorld, 0, &m_matView);
	_float fSpeed = m_fSpeed;


	//달리기
	if (CDInputMgr::GetInstance()->Get_DIKeyState(DIK_LSHIFT) & 0x80) {
		fSpeed *= 2.f;
	}
	else {
		fSpeed = m_fSpeed;
	}
	//앞
	if (CDInputMgr::GetInstance()->Get_DIKeyState(DIK_W) & 0x80)
	{
		_vec3	vLook;
		memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));

		_vec3	vLength = *D3DXVec3Normalize(&vLook, &vLook) * fTimeDelta * fSpeed;

		m_vEye += vLength;
		m_vAt += vLength;
	}
	//뒤
	if (CDInputMgr::GetInstance()->Get_DIKeyState(DIK_S) & 0x80)
	{
		_vec3	vLook;
		memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));

		_vec3	vLength = *D3DXVec3Normalize(&vLook, &vLook) * fTimeDelta * fSpeed;

		m_vEye -= vLength;
		m_vAt  -= vLength;
	}
	//오른쪽
	if (CDInputMgr::GetInstance()->Get_DIKeyState(DIK_D) & 0x80)
	{
		_vec3	vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3	vLength = *D3DXVec3Normalize(&vRight, &vRight) * fTimeDelta * fSpeed;

		m_vEye += vLength;
		m_vAt += vLength;
	}
	//왼쪽
	if (CDInputMgr::GetInstance()->Get_DIKeyState(DIK_A) & 0x80)
	{
		_vec3	vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3	vLength = *D3DXVec3Normalize(&vRight, &vRight) * fTimeDelta * fSpeed;

		m_vEye -= vLength;
		m_vAt  -= vLength;
	}  
	//올라가기
	if (CDInputMgr::GetInstance()->Get_DIKeyState(DIK_SPACE) & 0x80)
	{		
		m_vEye.y += fTimeDelta * m_fSpeed;;
		m_vAt.y += fTimeDelta * m_fSpeed;;
	}
	//앉기
	if (CDInputMgr::GetInstance()->Get_DIKeyState(DIK_LCONTROL) & 0x80)
	{
		m_vEye.y -= fTimeDelta * m_fSpeed;;
		m_vAt.y -= fTimeDelta * m_fSpeed;;
	}
	//마우스 고정
	if (CDInputMgr::GetInstance()->Get_DIKeyState(DIK_TAB) & 0x80)
	{
		if (m_bCheck)
			return;

		m_bCheck = true;

		if (m_bFix)
			m_bFix = false;

		else
			m_bFix = true;
	}
	else
	{
		m_bCheck = false;
	}
	//설치
	if (CDInputMgr::GetInstance()->Get_DIMouseState(DIM_LB) & 0x80) {
		//Peeking_Objects();
		CMapToolMgr::GetInstance()->Load_Json();
		_vec3 s = CMapToolMgr::GetInstance()->Get_Data("Stage1").Cam.vAt;
		int a = 2;
	}

	if (CDInputMgr::GetInstance()->Get_DIMouseState(DIM_RB) & 0x80) {
		CMapToolMgr::GetInstance()->Save_Json();
	}

	if (false == m_bFix)
		return;

}

void CDynamicCamera::Mouse_Move()
{
	_matrix	matCamWorld;
	D3DXMatrixInverse(&matCamWorld, 0, &m_matView);


	_long	dwMouseMove(0);

	if (dwMouseMove = CDInputMgr::GetInstance()->Get_DIMouseMove(DIMS_Y))
	{
		_vec3	vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3	vLook = m_vAt - m_vEye;
		_matrix matRot;

		D3DXMatrixRotationAxis(&matRot, &vRight, D3DXToRadian(dwMouseMove / 10.f));
		D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

		m_vAt = m_vEye + vLook;
	}

	if (dwMouseMove = CDInputMgr::GetInstance()->Get_DIMouseMove(DIMS_X))
	{
		_vec3	vUp{ 0.f, 1.f, 0.f };		

		_vec3	vLook = m_vAt - m_vEye;
		_matrix matRot;

		D3DXMatrixRotationAxis(&matRot, &vUp, D3DXToRadian(dwMouseMove / 10.f));
		D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

		m_vAt = m_vEye + vLook;
	}

}

void CDynamicCamera::Mouse_Fix()
{
	POINT	ptMouse{ WINCX >> 1, WINCY >> 1 };

	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);


}

void CDynamicCamera::Peeking_Objects()
{
	POINT	ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	// 뷰포트 -> 투영

	_vec3		vMousePos;

	D3DVIEWPORT9		ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));

	m_pGraphicDev->GetViewport(&ViewPort);

	vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	vMousePos.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
	vMousePos.z = 0.f;

	// 투영 -> 뷰 스페이스
	D3DXMATRIX	matProj;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, 0, &matProj);
	D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	// 뷰 스페이스 -> 월드

	D3DXMATRIX	matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, 0, &matView);

	_vec3		vRayPos{ 0.f, 0.f, 0.f };
	_vec3		vRayDir = vMousePos - vRayPos;
	D3DXVec3Normalize(&vRayDir, &vRayDir);

	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);


}


CDynamicCamera* CDynamicCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFov, const _float& fAspect, const _float& fNear, const _float& fFar)
{
	CDynamicCamera* pCamera = new CDynamicCamera(pGraphicDev);

	if (FAILED(pCamera->Ready_GameObject(pEye, pAt, pUp, fFov, fAspect, fNear, fFar)))
	{
		Safe_Release(pCamera);
		MSG_BOX("DynamicCamera Create Failed");
		return nullptr;
	}

	return pCamera;
}

void CDynamicCamera::Free()
{
	Engine::CCamera::Free();

}
