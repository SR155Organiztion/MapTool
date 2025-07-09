#include "pch.h"
#include "CPlayerPoint.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CMapToolMgr.h"

CPlayerPoint::CPlayerPoint(LPDIRECT3DDEVICE9 pGraphicDev)
    : Engine::CGameObject(pGraphicDev)
{
}

CPlayerPoint::CPlayerPoint(const CGameObject& rhs)
    : Engine::CGameObject(rhs)
{
}

CPlayerPoint::~CPlayerPoint()
{
}

HRESULT CPlayerPoint::Ready_GameObject()
{
    if (FAILED(Add_Component()))
        return E_FAIL;

    m_bPlanted = false;

    return S_OK;
}

_int CPlayerPoint::Update_GameObject(const _float& fTimeDelta)
{
    _uint iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);

    return iExit;
}

void CPlayerPoint::LateUpdate_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);

    return;
}

void CPlayerPoint::Render_GameObject()
{
    if (!m_bPlanted)
        return;

    D3DXMATRIX matWorld;
    m_pTransformCom->Get_World(&matWorld);
    m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    m_pTextureCom->Set_Texture(m_iPlayerNum);


    if (FAILED(Set_Metarial()))
        return;

    m_pBufferCom->Render_Buffer();


    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CPlayerPoint::Set_PlayerNum(_uint _iID)
{
    m_iPlayerNum = _iID;
}

void CPlayerPoint::Set_Pos(_vec3 _vPos)
{
    m_pTransformCom->Set_Pos(_vPos.x, _vPos.y, _vPos.z);
}

void CPlayerPoint::Set_Plant(_bool _b)
{
    m_bPlanted = _b;
}

HRESULT CPlayerPoint::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pBufferCom = dynamic_cast<Engine::CCubeTex*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_CubeTex"));
    if (nullptr == pComponent)
        return E_FAIL;
    m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_PlayerTexture"));
    if (nullptr == pComponent)
        return E_FAIL;
    m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

    pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Transform"));
    if (nullptr == pComponent)
        return E_FAIL;
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

    return S_OK;
}

HRESULT CPlayerPoint::Set_Metarial()
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

CPlayerPoint* CPlayerPoint::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CPlayerPoint* pPlayerPoint = new CPlayerPoint(pGraphicDev);

    if (FAILED(pPlayerPoint->Ready_GameObject()))
    {
        Safe_Release(pPlayerPoint);
        MSG_BOX("pBlock Create Failed");
        return nullptr;
    }

    return pPlayerPoint;
}

void CPlayerPoint::Free()
{
    Engine::CGameObject::Free();
}
