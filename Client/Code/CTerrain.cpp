#include "pch.h"
#include "CTerrain.h"
#include "CProtoMgr.h"
#include "CRenderer.h"

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pGraphicDev)
    : Engine::CGameObject(pGraphicDev), m_bEnable(true)
{
}

CTerrain::CTerrain(const CGameObject& rhs)
    : Engine::CGameObject(rhs), m_bEnable(true)
{
}

CTerrain::~CTerrain()
{
}

HRESULT CTerrain::Ready_GameObject()
{
    if (FAILED(Add_Component()))
        return E_FAIL;

    //m_pTransformCom->Set_Sca

    return S_OK;
}

_int CTerrain::Update_GameObject(const _float& fTimeDelta)
{
    if (!m_bEnable)
        return 0;

    m_pCalculatorCom->Calculate_AABB(m_pBufferCom->Get_Min(), m_pBufferCom->Get_Max());

    _uint iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);

    return iExit;
}

void CTerrain::LateUpdate_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
    
}

void CTerrain::Render_GameObject()
{
    if (!m_bEnable)
        return;

    D3DXMATRIX matWorld;
    m_pTransformCom->Get_World(&matWorld);
    m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

    m_pTextureCom->Set_Texture(1);

    if (FAILED(Set_Material()))
        return;

    m_pBufferCom->Render_Buffer();
    
}

HRESULT CTerrain::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pBufferCom = dynamic_cast<Engine::CTerrainTex*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_TerrainTex"));
    if (nullptr == pComponent)
        return E_FAIL;
    m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_TerrainTexture"));
    if (nullptr == pComponent)
        return E_FAIL;
    m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

    pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Transform"));
    if (nullptr == pComponent)
        return E_FAIL;
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

    pComponent = m_pCalculatorCom = dynamic_cast<Engine::CCalculator*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Calculator"));
    if (nullptr == pComponent)
        return E_FAIL;
    m_mapComponent[ID_DYNAMIC].insert({ L"Proto_Calculator", pComponent });

    return S_OK;
}

HRESULT CTerrain::Set_Material()
{
    D3DMATERIAL9 tMetarial;
    ZeroMemory(&tMetarial, sizeof(D3DMATERIAL9));

    tMetarial.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
    tMetarial.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
    tMetarial.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 0.2f);

    tMetarial.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
    tMetarial.Power = 0.f;

    m_pGraphicDev->SetMaterial(&tMetarial);

    return S_OK;
}


CTerrain* CTerrain::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CTerrain* pPlayer = new CTerrain(pGraphicDev);

    if (FAILED(pPlayer->Ready_GameObject()))
    {
        Safe_Release(pPlayer);
        MSG_BOX("pPlayer Create Failed");
        return nullptr;
    }
    
    return pPlayer;
}

void CTerrain::Free()
{
    Engine::CGameObject::Free();
}
