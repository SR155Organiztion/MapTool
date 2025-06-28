#include "pch.h"
#include "CPlayer.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
    : Engine::CGameObject(pGraphicDev)
{
}

CPlayer::CPlayer(const CGameObject& rhs)
    : Engine::CGameObject(rhs)
{
}

CPlayer::~CPlayer()
{
}

HRESULT CPlayer::Ready_GameObject()
{
    if (FAILED(Add_Component()))
        return E_FAIL;

    return S_OK;
}

_int CPlayer::Update_GameObject(const _float& fTimeDelta)
{
     _uint iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);
    
    return iExit;
}

void CPlayer::LateUpdate_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);
    CTerrainTex* pTerrain = dynamic_cast<CTerrainTex*>(
        CManagement::GetInstance()->Get_Component(ID_STATIC, L"Environment_Layer", L"Terrain", L"Com_Buffer"));

    if (pTerrain == nullptr) {
        MSG_BOX("³Î");
        return;
    }
    _vec3 vPos;
    
    Key_Input(fTimeDelta);

    m_pTransformCom->Get_Info(INFO_POS, &vPos);

    if (vPos.x < 0) {
        vPos.x = 0;
        m_pTransformCom->Set_Pos(vPos.x, vPos.y, vPos.z);
    }

    if (vPos.z < 0) {
        vPos.z = 0;
        m_pTransformCom->Set_Pos(vPos.x, vPos.y, vPos.z);
    }
    return;
}

void CPlayer::Render_GameObject()
{
    D3DXMATRIX matWorld;
    m_pTransformCom->Get_World(&matWorld);
    m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

    m_pTextureCom->Set_Texture(0);

    if (FAILED(Set_Metarial()))
        return;

    m_pBufferCom->Render_Buffer();

   // m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CPlayer::Add_Component()
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

HRESULT CPlayer::Set_Metarial() 
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

void CPlayer::Key_Input(const _float& fTimeDelta)
{
    _vec3       vLook, vUp, vRight;
    m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);
    m_pTransformCom->Get_Info(INFO_UP, &vUp);
    D3DXVec3Normalize(&vLook, D3DXVec3Cross(&vLook, &vRight, &vUp));

    if (GetAsyncKeyState(VK_UP))
    {
        m_pTransformCom->Move_Pos(&vLook, 10.f, fTimeDelta);
    }
    if (GetAsyncKeyState(VK_DOWN))
    {
        m_pTransformCom->Move_Pos(&vLook, -10.f, fTimeDelta);
    }

      if (GetAsyncKeyState(VK_LEFT))
    {
        m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(180.f * -fTimeDelta));
    }

    if (GetAsyncKeyState(VK_RIGHT))
    {
        m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(180.f * fTimeDelta));
    }

}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CPlayer* pPlayer = new CPlayer(pGraphicDev);

    if (FAILED(pPlayer->Ready_GameObject()))
    {
        Safe_Release(pPlayer);
        MSG_BOX("pPlayer Create Failed");
        return nullptr;
    }

    return pPlayer;
}

void CPlayer::Free()
{
    Engine::CGameObject::Free();
}
