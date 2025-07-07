#include "pch.h"
#include "CBlock.h"
#include "CCollisionMgr.h"
#include "CDynamicCamera.h"
#include "CFontMgr.h"
#include "CLightMgr.h"
#include "CMapToolMgr.h"
#include "CProtoMgr.h"
#include "CShowBox.h"
#include "CShowRcTile.h"
#include "CSkyBox.h"
#include "CStage.h"
#include "CTerrain.h"
#include <iomanip> 
#include "CImguiMgr.h"

CStage::CStage(LPDIRECT3DDEVICE9 pGraphicDev)
    : Engine::CScene(pGraphicDev)
{
}

CStage::~CStage()
{
}

HRESULT CStage::Ready_Scene()
{
    if (FAILED(Ready_Environment_Layer(L"Environment_Layer")))
        return E_FAIL;

    if (FAILED(Ready_Tile_Layer(L"Tile_Layer")))
        return E_FAIL;

    if (FAILED(Ready_Block_Layer(L"Block_Layer")))
        return E_FAIL;

    if (FAILED(Ready_GameObject_Layer(L"GameObject_Layer")))
        return E_FAIL;

    if (FAILED(Ready_UI_Layer(L"UI_Layer")))
        return E_FAIL;

    CImguiMgr::GetInstance()->Ready_Imgui(m_pGraphicDev, g_hWnd);

    return S_OK;
}

HRESULT CStage::Ready_Environment_Layer(const _tchar* pLayerTag)
{
    Engine::CLayer* pLayer = CLayer::Create();
    if (nullptr == pLayer)
        return E_FAIL;

    Engine::CGameObject* pGameObject = nullptr;

    m_mapLayer.insert({ pLayerTag, pLayer });

    return S_OK;
}

HRESULT CStage::Ready_Tile_Layer(const _tchar* pLayerTag)
{
    Engine::CLayer* pLayer = CLayer::Create();
    if (nullptr == pLayer)
        return E_FAIL;

    Engine::CGameObject* pGameObject = nullptr;

    m_mapLayer.insert({ pLayerTag, pLayer });

    return S_OK;
}

HRESULT CStage::Ready_Block_Layer(const _tchar* pLayerTag)
{
    Engine::CLayer* pLayer = CLayer::Create();
    if (nullptr == pLayer)
        return E_FAIL;

    Engine::CGameObject* pGameObject = nullptr;

    m_mapLayer.insert({ pLayerTag, pLayer });

    return S_OK;
}

HRESULT CStage::Ready_GameObject_Layer(const _tchar* pLayerTag)
{
    Engine::CLayer* pLayer = CLayer::Create();
    if (nullptr == pLayer)
        return E_FAIL;

    Engine::CGameObject* pGameObject = nullptr;

    // dynamicCamera
    _vec3	vEye{ 0.f, 10.f, -10.f };
    _vec3	vAt{ 0.f, 0.f, 1.f };
    _vec3	vUp{ 0.f , 1.f, 0.f };
    pGameObject = CDynamicCamera::Create(m_pGraphicDev, &vEye, &vAt, &vUp);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"DynamicCamera", pGameObject)))
        return E_FAIL;

    pGameObject = CTerrain::Create(m_pGraphicDev);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Terrain", pGameObject)))
        return E_FAIL;

    pGameObject = CShowBox::Create(m_pGraphicDev);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"ShowBox", pGameObject)))
        return E_FAIL;

    pGameObject = CShowRcTile::Create(m_pGraphicDev);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"ShowRcTile", pGameObject)))
        return E_FAIL;

    m_mapLayer.insert({ pLayerTag, pLayer });

    return S_OK;
}

HRESULT CStage::Ready_UI_Layer(const _tchar* pLayerTag)
{
    Engine::CLayer* pLayer = CLayer::Create();
    if (nullptr == pLayer)
        return E_FAIL;

    Engine::CGameObject* pGameObject = nullptr;



    m_mapLayer.insert({ pLayerTag, pLayer });

    return S_OK;
}

_int CStage::Update_Scene(const _float& fTimeDelta)
{
    CCollisionMgr::GetInstance()->Compute_Ray(m_pGraphicDev, g_hWnd);
    CImguiMgr::GetInstance()->Update_Imgui();

    return Engine::CScene::Update_Scene(fTimeDelta);
}

void CStage::LateUpdate_Scene(const _float& fTimeDelta)
{
    Engine::CScene::LateUpdate_Scene(fTimeDelta);
}

void CStage::Render_Scene()
{
    CImguiMgr::GetInstance()->Render_Imgui();
}


CStage* CStage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CStage* pLogo = new CStage(pGraphicDev);

    if (FAILED(pLogo->Ready_Scene()))
    {
        Safe_Release(pLogo);
        MSG_BOX("Logo Create Failed");
        return nullptr;
    }

    return pLogo;
}

void CStage::Free()
{
    Engine::CScene::Free();
}
