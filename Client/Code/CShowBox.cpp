#include "pch.h"
#include "CShowBox.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CCollisionMgr.h"
#include "CMapToolMgr.h"
#include "CBlock.h"
#include "CDInputMgr.h"

CShowBox::CShowBox(LPDIRECT3DDEVICE9 pGraphicDev)
    : Engine::CGameObject(pGraphicDev)
{
}

CShowBox::CShowBox(const CGameObject& rhs)
    : Engine::CGameObject(rhs)
{
}

CShowBox::~CShowBox()
{
}

HRESULT CShowBox::Ready_GameObject()
{
    if (FAILED(Add_Component()))
        return E_FAIL;

    m_pTransformCom->m_vScale = { 0.7f, 0.7f, 0.7f };

    return S_OK;
}

_int CShowBox::Update_GameObject(const _float& fTimeDelta)
{
    _uint iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);

    _vec3 vLook = CMapToolMgr::GetInstance()->Get_DirLook();
    m_pTransformCom->Set_Look(vLook.x, vLook.y, vLook.z);

    return iExit;
}

void CShowBox::LateUpdate_GameObject(const _float& fTimeDelta)
{
    _vec3 vPos = CCollisionMgr::GetInstance()->Get_ColPos();
    _vec3 vNormal = CCollisionMgr::GetInstance()->Get_ColNormal();
    Set_Greed(vPos, vNormal);

    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);

    return;
}

void CShowBox::Render_GameObject()
{
    if (CMapToolMgr::GetInstance()->Get_NowObject() != CREATEOBJECT_ID::O_BLOCK)
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

HRESULT CShowBox::Add_Component()
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


    return S_OK;
}

HRESULT CShowBox::Set_Metarial()
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

void CShowBox::Set_Greed(_vec3 _vPos, _vec3 _vNormal)
{
    _vec3 vTmp;

    float fOffsetY = 0.f;
    if (CMapToolMgr::GetInstance()->Get_NowStation() == 0) {
        fOffsetY = 0.25f;
    }

    // 먼저 위치를 격자 단위로 스냅
    vTmp.x = floorf(_vPos.x + _vNormal.x * 0.5f) + 0.5f;
    vTmp.y = floorf(_vPos.y + _vNormal.y * 0.5f) + 0.5f;
    vTmp.z = floorf(_vPos.z + _vNormal.z * 0.5f) + 0.5f;

    m_pTransformCom->Set_Pos(vTmp.x, vTmp.y, vTmp.z);
}


CShowBox* CShowBox::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CShowBox* pShowBox = new CShowBox(pGraphicDev);

    if (FAILED(pShowBox->Ready_GameObject()))
    {
        Safe_Release(pShowBox);
        MSG_BOX("pShowBox Create Failed");
        return nullptr;
    }

    return pShowBox;
}

void CShowBox::Free()
{
    Engine::CGameObject::Free();
}
