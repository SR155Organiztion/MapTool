#include "pch.h"
#include "CShowEnvObject.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CCollisionMgr.h"
#include "CMapToolMgr.h"
#include "CBlock.h"
#include "CDInputMgr.h"

CShowEnvObject::CShowEnvObject(LPDIRECT3DDEVICE9 pGraphicDev)
    : Engine::CGameObject(pGraphicDev)
{
}

CShowEnvObject::CShowEnvObject(const CGameObject& rhs)
    : Engine::CGameObject(rhs)
{
}

CShowEnvObject::~CShowEnvObject()
{
}

HRESULT CShowEnvObject::Ready_GameObject()
{
    if (FAILED(Add_Component()))
        return E_FAIL;

    m_pTransformCom->m_vScale = { 0.25f, 0.25f, 0.25f };

    return S_OK;
}

_int CShowEnvObject::Update_GameObject(const _float& fTimeDelta)
{
    m_pTransformCom->m_vAngle.y = CMapToolMgr::GetInstance()->Get_NowAngle();

    _uint iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);


    return iExit;
}

void CShowEnvObject::LateUpdate_GameObject(const _float& fTimeDelta)
{
    _vec3 vPos = CCollisionMgr::GetInstance()->Get_ColPos();
    m_pTransformCom->Set_Pos(vPos.x, vPos.y + 0.125f, vPos.z);
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);

    return;
}

void CShowEnvObject::Render_GameObject()
{
    if (CMapToolMgr::GetInstance()->Get_NowObject() != CREATEOBJECT_ID::O_ENV)
        return;

    D3DXMATRIX matWorld;
    m_pTransformCom->Get_World(&matWorld);
    m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
    m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

    m_pTextureCom->Set_Texture(CMapToolMgr::GetInstance()->Get_NowStation());

    if (FAILED(Set_Metarial()))
        return;

    m_pBufferCom->Render_Buffer();

    m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CShowEnvObject::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTileTex*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_RcTileTex"));
    if (nullptr == pComponent)
        return E_FAIL;
    m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_ShowEnvObjectTexture"));
    if (nullptr == pComponent)
        return E_FAIL;
    m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

    pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Transform"));
    if (nullptr == pComponent)
        return E_FAIL;
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });


    return S_OK;
}

HRESULT CShowEnvObject::Set_Metarial()
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

CShowEnvObject* CShowEnvObject::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CShowEnvObject* pShowEnvObject = new CShowEnvObject(pGraphicDev);

    if (FAILED(pShowEnvObject->Ready_GameObject()))
    {
        Safe_Release(pShowEnvObject);
        MSG_BOX("pShowEnvObject Create Failed");
        return nullptr;
    }

    return pShowEnvObject;
}

void CShowEnvObject::Free()
{
    Engine::CGameObject::Free();
}
