#include "pch.h"
#include "CTestTerrain.h"
#include "CProtoMgr.h"
#include "CRenderer.h"

CTestTerrain::CTestTerrain(LPDIRECT3DDEVICE9 pGraphicDev)
    : Engine::CGameObject(pGraphicDev)
{
}

CTestTerrain::CTestTerrain(const CGameObject& rhs)
    : Engine::CGameObject(rhs)
{
}

CTestTerrain::~CTestTerrain()
{
}

HRESULT CTestTerrain::Ready_GameObject()
{
    if (FAILED(Add_Component()))
        return E_FAIL;

    //m_pTransformCom->Set_Sca

    return S_OK;
}

_int CTestTerrain::Update_GameObject(const _float& fTimeDelta)
{
    _uint iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);

    return iExit;
}

void CTestTerrain::LateUpdate_GameObject(const _float& fTimeDelta)
{

    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);


}

void CTestTerrain::Render_GameObject()
{
    D3DXMATRIX matWorld;
    m_pTransformCom->Get_World(&matWorld);
    m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

    m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
    m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
    
    m_pBufferCom->Render_Buffer();

    m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}

HRESULT CTestTerrain::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pBufferCom = dynamic_cast<Engine::CTerrainCol*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_TerrainCol"));
    if (nullptr == pComponent)
        return E_FAIL;
    m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Transform"));
    if (nullptr == pComponent)
        return E_FAIL;
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

    return S_OK;
}


CTestTerrain* CTestTerrain::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CTestTerrain* pTestTerrain = new CTestTerrain(pGraphicDev);

    if (FAILED(pTestTerrain->Ready_GameObject()))
    {
        Safe_Release(pTestTerrain);
        MSG_BOX("pTestTerrain Create Failed");
        return nullptr;
    }

    return pTestTerrain;
}

void CTestTerrain::Free()
{
    Engine::CGameObject::Free();
}
