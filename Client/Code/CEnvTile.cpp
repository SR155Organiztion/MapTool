#include "pch.h"
#include "CEnvTile.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CMapToolMgr.h"

CEnvTile::CEnvTile(LPDIRECT3DDEVICE9 pGraphicDev)
    : Engine::CGameObject(pGraphicDev)
{
}

CEnvTile::CEnvTile(const CGameObject& rhs)
    : Engine::CGameObject(rhs)
{
}

CEnvTile::~CEnvTile()
{
}

HRESULT CEnvTile::Ready_GameObject()
{
    if (FAILED(Add_Component()))
        return E_FAIL;

    return S_OK;
}

_int CEnvTile::Update_GameObject(const _float& fTimeDelta)
{
    _uint iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    return iExit;
}

void CEnvTile::LateUpdate_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);

    return;
}

void CEnvTile::Render_GameObject()
{
    D3DXMATRIX matWorld;
    m_pTransformCom->Get_World(&matWorld);
    m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

    m_pTextureCom->Set_Texture(m_iTextureNum);

    m_pBufferCom->Render_Buffer();

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CEnvTile::Set_TextureNum(_uint _iID = 0)
{
    m_iTextureNum = _iID;
}

void CEnvTile::Set_Angle(_float _fAngle)
{
    m_pTransformCom->m_vAngle.y = _fAngle;
}

void CEnvTile::Set_Scale(_vec3 _vScale)
{
    m_pTransformCom->m_vScale = _vScale;
}

HRESULT CEnvTile::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTileTex*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_RcTileTex"));
    if (nullptr == pComponent)
        return E_FAIL;
    m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_EnvTileTexture"));
    if (nullptr == pComponent)
        return E_FAIL;
    m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

    pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Transform"));
    if (nullptr == pComponent)
        return E_FAIL;
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });


    return S_OK;
}

HRESULT CEnvTile::Set_Metarial()
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

CEnvTile* CEnvTile::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CEnvTile* pEnvTile = new CEnvTile(pGraphicDev);

    if (FAILED(pEnvTile->Ready_GameObject()))
    {
        Safe_Release(pEnvTile);
        MSG_BOX("pEnvTile Create Failed");
        return nullptr;
    }

    return pEnvTile;
}

void CEnvTile::Free()
{
    Engine::CGameObject::Free();
}
