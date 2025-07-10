#include "pch.h"
#include "CHexTile.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"

CHexTile::CHexTile(LPDIRECT3DDEVICE9 pGraphicDev)
    : Engine::CGameObject(pGraphicDev)
{
}

CHexTile::CHexTile(const CGameObject& rhs)
    : Engine::CGameObject(rhs)
{
}

CHexTile::~CHexTile()
{
}

HRESULT CHexTile::Ready_GameObject()
{
    if (FAILED(Add_Component()))
        return E_FAIL;

    //m_pTransformCom->m_vScale = { 2.f, 2.f, 2.f };

    return S_OK;
}

_int CHexTile::Update_GameObject(const _float& fTimeDelta)
{
    _vec3 vPos;
    m_pTransformCom->Get_Info(INFO_POS, &vPos);
    _vec3 min = *(m_pBufferCom->Get_Min());
    min = { min.x + vPos.x, min.y + vPos.y ,min.z + vPos.z };

    _vec3 max = *(m_pBufferCom->Get_Max());
    max = { max.x + vPos.x, max.y + vPos.y ,max.z + vPos.z };

    m_pCalculatorCom->Calculate_AABB(&min, &max);

    _uint iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);

    return iExit;
}

void CHexTile::LateUpdate_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);

    return;
}

void CHexTile::Render_GameObject()
{
    D3DXMATRIX matWorld;
    m_pTransformCom->Get_World(&matWorld);
    m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

    m_pTextureCom->Set_Texture(m_iTextureNum);

    if (FAILED(Set_Metarial()))
        return;

    m_pBufferCom->Render_Buffer();

    // m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CHexTile::Set_TextureNum(_uint _iID)
{
    m_iTextureNum = _iID;
}

HRESULT CHexTile::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pBufferCom = dynamic_cast<Engine::CHexTileTex*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_HexTileTex"));
    if (nullptr == pComponent)
        return E_FAIL;
    m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_HexTileTexture"));
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

HRESULT CHexTile::Set_Metarial()
{
    D3DMATERIAL9 tMetarial;
    ZeroMemory(&tMetarial, sizeof(D3DMATERIAL9));

    tMetarial.Diffuse = D3DXCOLOR(1.f, 0.f, 1.f, 1.f);
    tMetarial.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
    tMetarial.Ambient = D3DXCOLOR(0.6f, 0.6f, 0.6f, 0.6f);

    tMetarial.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
    tMetarial.Power = 0.f;

    m_pGraphicDev->SetMaterial(&tMetarial);

    return S_OK;
}

CHexTile* CHexTile::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CHexTile* pHexTile = new CHexTile(pGraphicDev);

    if (FAILED(pHexTile->Ready_GameObject()))
    {
        Safe_Release(pHexTile);
        MSG_BOX("pHexTile Create Failed");
        return nullptr;
    }

    return pHexTile;
}

void CHexTile::Free()
{
    Engine::CGameObject::Free();
}
