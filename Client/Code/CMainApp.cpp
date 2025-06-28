#include "pch.h"
#include "CMainApp.h"
#include "CTimerMgr.h"
#include "CFrameMgr.h"
#include "CLogo.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CFontMgr.h"
#include "CDInputMgr.h"
#include "CLightMgr.h"

CMainApp::CMainApp() : m_pDeviceClass(nullptr), m_pGraphicDev(nullptr)
, m_pManagementClass(CManagement::GetInstance())
{
}

CMainApp::~CMainApp()
{
}

HRESULT CMainApp::Ready_MainApp()
{
	if (FAILED(Ready_DefaultSetting(&m_pGraphicDev)))
	{
		MSG_BOX("Ready_DefaultSetting Failed");
		return E_FAIL;
	}

	if (FAILED(Ready_Scene(m_pGraphicDev)))
	{
		MSG_BOX("Ready_Scene Failed");
		return E_FAIL;
	}


	return S_OK;
}

int CMainApp::Update_MainApp(const float& fTimeDelta)
{
	CDInputMgr::GetInstance()->Update_InputDev();


	m_pManagementClass->Update_Scene(fTimeDelta);



	return 0;
}

void CMainApp::LateUpdate_MainApp(const float& fTimeDelta)
{
	m_pManagementClass->LateUpdate_Scene(fTimeDelta);

	//_ulong dwFlag(0);
	//
	//if (dwFlag = CDInputMgr::GetInstance()->Get_DIMouseMove(DIMS_Z))
	//{
	//	int a = 0;
	//}

}

void CMainApp::Render_MainApp()
{
	m_pDeviceClass->Render_Begin(D3DXCOLOR(0.f, 0.f, 1.f, 1.f));

	m_pManagementClass->Render_Scene(m_pGraphicDev);

	m_pDeviceClass->Render_End();

}

HRESULT CMainApp::Ready_DefaultSetting(LPDIRECT3DDEVICE9* ppGraphicDev)
{
	if (FAILED(CGraphicDev::GetInstance()->Ready_GraphicDev(g_hWnd, MODE_WIN, WINCX, WINCY, &m_pDeviceClass)))
	{
		return E_FAIL;
	}
	m_pDeviceClass->AddRef();

	(*ppGraphicDev) = m_pDeviceClass->Get_GraphicDev();
	(*ppGraphicDev)->AddRef();

	(*ppGraphicDev)->SetRenderState(D3DRS_LIGHTING, FALSE);
	
	(*ppGraphicDev)->SetRenderState(D3DRS_ZENABLE, TRUE);		// Z 버퍼에 Z값을 기록, Z 정렬 수행 여부를 묻는 옵션
	(*ppGraphicDev)->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);  // Z 버퍼에 Z값을 기록할지 말지 결정


	(*ppGraphicDev)->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	(*ppGraphicDev)->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	// 폰트 추가

	if (FAILED(CFontMgr::GetInstance()->Ready_Font(*ppGraphicDev, L"Font_Default", L"궁서", 10, 20, FW_HEAVY)))
		return E_FAIL;

	if (FAILED(CFontMgr::GetInstance()->Ready_Font(*ppGraphicDev, L"Font_Batang", L"바탕", 20, 10, FW_THIN)))
		return E_FAIL;

	// dinput

	if (FAILED(CDInputMgr::GetInstance()->Ready_InputDev(g_hInst, g_hWnd)))
		return E_FAIL;



	//D3DXPlaneFromPoints();


	return S_OK;
}

HRESULT CMainApp::Ready_Scene(LPDIRECT3DDEVICE9 pGraphicDev)
{	
	Engine::CScene* pScene = CLogo::Create(pGraphicDev);

	if (nullptr == pScene)
		return E_FAIL;

	if (FAILED(m_pManagementClass->Set_Scene(pScene)))
	{
		MSG_BOX("Set_Scene Failed");
		return E_FAIL;
	}


	return S_OK;
}

CMainApp* CMainApp::Create()
{
	CMainApp* pInstance = new CMainApp;

	if (FAILED(pInstance->Ready_MainApp()))
	{
		Engine::Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CMainApp::Free()
{
	Safe_Release(m_pGraphicDev);
	Safe_Release(m_pDeviceClass);

	CLightMgr::GetInstance()->DestroyInstance();
	CDInputMgr::GetInstance()->DestroyInstance();
	CFontMgr::GetInstance()->DestroyInstance();
	CRenderer::GetInstance()->DestroyInstance();
	CProtoMgr::GetInstance()->DestroyInstance();
	CTimerMgr::GetInstance()->DestroyInstance();
	CFrameMgr::GetInstance()->DestroyInstance();
	m_pManagementClass->DestroyInstance();
	CGraphicDev::GetInstance()->DestroyInstance();

}
