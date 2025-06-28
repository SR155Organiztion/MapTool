#include "pch.h"
#include "CMonster.h"
#include "CProtoMgr.h"
#include "CManagement.h"

CMonster::CMonster(LPDIRECT3DDEVICE9 pGraphicDev)
    : Engine::CGameObject(pGraphicDev)
{
}

CMonster::CMonster(const CGameObject& rhs)
    : Engine::CGameObject(rhs)
{
}

CMonster::~CMonster()
{
}

HRESULT CMonster::Ready_GameObject()
{
    if (FAILED(Add_Component()))
        return E_FAIL;

    return S_OK;
}

_int CMonster::Update_GameObject(const _float& fTimeDelta)
{
    return Engine::CGameObject::Update_GameObject(fTimeDelta);
}

void CMonster::LateUpdate_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);

    Engine::CTransform* pPlayerTransformCom = dynamic_cast<Engine::CTransform*>
        (CManagement::GetInstance()->Get_Component(ID_DYNAMIC, L"Environment_Layer", L"Player", L"Com_Transform"));

    if (nullptr == pPlayerTransformCom)
        return;

    _vec3   vPlayerPos{};

    pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);

    m_pTransformCom->Chase_Target(&vPlayerPos, 3.f, fTimeDelta);
    
}

void CMonster::Render_GameObject()
{
    D3DXMATRIX matWorld;
    m_pTransformCom->Get_World(&matWorld);
    m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    m_pBufferCom->Render_Buffer();

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CMonster::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pBufferCom = dynamic_cast<Engine::CTriCol*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_TriCol"));
    if (nullptr == pComponent)
        return E_FAIL;
    m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Transform"));
    if (nullptr == pComponent)
        return E_FAIL;
    m_mapComponent[ID_STATIC].insert({ L"Com_Transform", pComponent });


    return S_OK;
}

HRESULT CMonster::Set_Metarial()
{

    return E_NOTIMPL;
}


CMonster* CMonster::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CMonster* pMonster = new CMonster(pGraphicDev);

    if (FAILED(pMonster->Ready_GameObject()))
    {
        Safe_Release(pMonster);
        MSG_BOX("pMonster Create Failed");
        return nullptr;

    }

    return pMonster;
}

void CMonster::Free()
{
    Engine::CGameObject::Free();
}
