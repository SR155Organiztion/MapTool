#include "pch.h"
#include "CEnvObject.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CMapToolMgr.h"

CEnvObject::CEnvObject(LPDIRECT3DDEVICE9 pGraphicDev)
    : Engine::CGameObject(pGraphicDev)
{
}

CEnvObject::CEnvObject(const CGameObject& rhs)
    : Engine::CGameObject(rhs)
{
}

CEnvObject::~CEnvObject()
{
}

HRESULT CEnvObject::Ready_GameObject()
{
    if (FAILED(Add_Component()))
        return E_FAIL;

    return S_OK;
}

_int CEnvObject::Update_GameObject(const _float& fTimeDelta)
{

    _uint iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

    _matrix matWorld, matView, matBill;

    m_pTransformCom->Get_World(&matWorld);
    m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
    D3DXMatrixIdentity(&matBill);
    D3DXMatrixInverse(&matView, 0, &matView);

    _vec3	vViewScale, vViewTrans;
    D3DXQUATERNION qViewRot;
    D3DXMatrixDecompose(&vViewScale, &qViewRot, &vViewTrans, &matView);
    _matrix matViewRot;  D3DXMatrixRotationQuaternion(&matViewRot, &qViewRot);

    matWorld = matViewRot * matWorld;

    m_pTransformCom->m_matWorld = matWorld;

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    return iExit;
}

void CEnvObject::LateUpdate_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);

    return;
}

void CEnvObject::Render_GameObject()
{
    D3DXMATRIX matWorld;
    m_pTransformCom->Get_World(&matWorld);
    m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
    m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
    m_pTextureCom->Set_Texture(m_iTextureNum);

    if (FAILED(Set_Metarial()))
        return;

    m_pBufferCom->Render_Buffer();
    m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
    // m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CEnvObject::Set_TextureNum(_uint _iID)
{
    m_iTextureNum = _iID;
}

void CEnvObject::Set_Angle(_float _fAngle)
{
    m_pTransformCom->m_vAngle.y = _fAngle;
}

void CEnvObject::Set_Scale(_vec3 _vScale)
{
    m_pTransformCom->m_vScale = _vScale;
}

HRESULT CEnvObject::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_RcTex"));
    if (nullptr == pComponent)
        return E_FAIL;
    m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

    pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_EnvObjectTexture"));
    if (nullptr == pComponent)
        return E_FAIL;
    m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

    pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Transform"));
    if (nullptr == pComponent)
        return E_FAIL;
    m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });


    return S_OK;
}

HRESULT CEnvObject::Set_Metarial()
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

CEnvObject* CEnvObject::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CEnvObject* pEnvObject = new CEnvObject(pGraphicDev);

    if (FAILED(pEnvObject->Ready_GameObject()))
    {
        Safe_Release(pEnvObject);
        MSG_BOX("pEnvObject Create Failed");
        return nullptr;
    }

    return pEnvObject;
}

void CEnvObject::Free()
{
    Engine::CGameObject::Free();
}
