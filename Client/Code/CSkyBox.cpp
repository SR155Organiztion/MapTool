#include "pch.h"
#include "CSkyBox.h"
#include "CProtoMgr.h"
#include "CRenderer.h"

CSkyBox::CSkyBox(LPDIRECT3DDEVICE9 pGraphicDev)
    : Engine::CGameObject(pGraphicDev)
{
}

CSkyBox::CSkyBox(const CGameObject& rhs)
    : Engine::CGameObject(rhs)
{
}

CSkyBox::~CSkyBox()
{
}

HRESULT CSkyBox::Ready_GameObject()
{
    if (FAILED(Add_Component()))
        return E_FAIL;

    m_pTransformCom->m_vScale = { 40.f, 40.f, 40.f };


    return S_OK;
}

_int CSkyBox::Update_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::Update_GameObject(fTimeDelta);

   

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_PRIORITY, this);

    return 0;
}

void CSkyBox::LateUpdate_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
    
    _matrix matView;

    m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
    D3DXMatrixInverse(&matView, 0, &matView);

    m_pTransformCom->Set_Pos(matView._41, matView._42 + 3.f, matView._43);
  
}

void CSkyBox::Render_GameObject()
{
    D3DXMATRIX matWorld;
    m_pTransformCom->Get_World(&matWorld);
    m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
    m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

    m_pTextureCom->Set_Texture(3);

    m_pBufferCom->Render_Buffer();

    m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
    m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CSkyBox::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pBufferCom = dynamic_cast<Engine::CCubeTex*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_CubeTex"));
    if (nullptr == pComponent)
        return E_FAIL;
    m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_SkyBoxTexture"));
    if (nullptr == pComponent)
        return E_FAIL;
    m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

    pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Transform"));
    if (nullptr == pComponent)
        return E_FAIL;
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

    return S_OK;
}


CSkyBox* CSkyBox::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CSkyBox* pBackGround = new CSkyBox(pGraphicDev);

    if (FAILED(pBackGround->Ready_GameObject()))
    {
        Safe_Release(pBackGround);
        MSG_BOX("pBackGround Create Failed");
        return nullptr;

    }

    return pBackGround;
}

void CSkyBox::Free()
{
    Engine::CGameObject::Free();
}
