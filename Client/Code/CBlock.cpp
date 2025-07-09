#include "pch.h"
#include "CBlock.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CMapToolMgr.h"

CBlock::CBlock(LPDIRECT3DDEVICE9 pGraphicDev)
    : Engine::CGameObject(pGraphicDev), m_iTextureNum(0), m_bShow(false)
{
}

CBlock::CBlock(const CGameObject& rhs)
    : Engine::CGameObject(rhs)
{
}

CBlock::~CBlock()
{
}

HRESULT CBlock::Ready_GameObject()
{
    m_bShow = false;
         
    if (FAILED(Add_Component()))
        return E_FAIL;


    return S_OK;
}

_int CBlock::Update_GameObject(const _float& fTimeDelta)
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

void CBlock::LateUpdate_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);

    return;
}

void CBlock::Render_GameObject()
{
    D3DXMATRIX matWorld;
    m_pTransformCom->Get_World(&matWorld);
    m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    m_pTextureCom->Set_Texture(m_iTextureNum);
    

    if (FAILED(Set_Metarial()))
        return;

    m_pBufferCom->Render_Buffer();

    if(m_bShow){
        matWorld._42 += 0.501f;

        D3DXMATRIX matScale;
        D3DXMatrixScaling(&matScale, 0.5f, 0.5f, 0.5f);
        matWorld = matScale * matWorld;

        m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
        
        m_pRcTileTextureCom->Set_Texture(m_iTileTexNum);
        m_pRcTileCom->Render_Buffer();
    }

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CBlock::Set_TextureNum(_uint _iID)
{
    m_iTextureNum = _iID;

}

void CBlock::Set_Create(_uint _eID)
{
    m_bShow = true;

    CComponent* pComponent = nullptr;

    pComponent = m_pRcTileCom = dynamic_cast<Engine::CRcTileTex*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_RcTileTex"));
    if (nullptr == pComponent)
        return;
    m_mapComponent[ID_STATIC].insert({ L"Com_TileBuffer", pComponent });

    pComponent = m_pRcTileTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_CreateTexture"));
    if (nullptr == pComponent)
        return;
    m_mapComponent[ID_STATIC].insert({ L"Com_TileTexture", pComponent });

    m_iTileTexNum = _eID;
}

void CBlock::Set_Tools(_uint _eID)
{
    m_bShow = true;

    CComponent* pComponent = nullptr;

    pComponent = m_pRcTileCom = dynamic_cast<Engine::CRcTileTex*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_RcTileTex"));
    if (nullptr == pComponent)
        return;
    m_mapComponent[ID_STATIC].insert({ L"Com_TileBuffer", pComponent });

    pComponent = m_pRcTileTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_ToolTexture"));
    if (nullptr == pComponent)
        return;
    m_mapComponent[ID_STATIC].insert({ L"Com_TileTexture", pComponent });

    m_iTileTexNum = _eID - 1;
}

HRESULT CBlock::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pBufferCom = dynamic_cast<Engine::CCubeTex*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_CubeTex"));
    if (nullptr == pComponent)
        return E_FAIL;
    m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_StationTexture"));
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

HRESULT CBlock::Set_Metarial()
{
    D3DMATERIAL9 tMetarial;
    ZeroMemory(&tMetarial, sizeof(D3DMATERIAL9));

    tMetarial.Diffuse = D3DXCOLOR(1.f, 0.f, 1.f, 0.5f);
    tMetarial.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
    tMetarial.Ambient = D3DXCOLOR(0.6f, 0.6f, 0.6f, 0.6f);

    tMetarial.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
    tMetarial.Power = 0.f;

    m_pGraphicDev->SetMaterial(&tMetarial);

    return S_OK;
}

CBlock* CBlock::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CBlock* pBlock = new CBlock(pGraphicDev);

    if (FAILED(pBlock->Ready_GameObject()))
    {
        Safe_Release(pBlock);
        MSG_BOX("pBlock Create Failed");
        return nullptr;
    }

    return pBlock;
}

void CBlock::Free()
{
    Engine::CGameObject::Free();
}
