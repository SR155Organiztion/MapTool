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
    : Engine::CGameObject(pGraphicDev), m_bClicked(false)
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

    return S_OK;
}

_int CShowBox::Update_GameObject(const _float& fTimeDelta)
{
    _uint iExit = Engine::CGameObject::Update_GameObject(fTimeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);

    return iExit;
}

void CShowBox::LateUpdate_GameObject(const _float& fTimeDelta)
{
    _vec3 vPos = CCollisionMgr::GetInstance()->Get_ColPos();
    
    Set_Greed(vPos);

    //임시
    if (CDInputMgr::GetInstance()->Get_DIMouseState(DIM_LB) & 0x80) {
        if (!m_bClicked) {
            Create_Block();
            m_bClicked = true;
        }
    }
    else {
        m_bClicked = false;
    }

    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);

    return;
}

void CShowBox::Render_GameObject()
{
    D3DXMATRIX matWorld;
    m_pTransformCom->Get_World(&matWorld);
    m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
    m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

    m_pTextureCom->Set_Texture(0);

  
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

    pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_ShowBoxTexture"));
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

void CShowBox::Set_Greed(_vec3 _v)
{
    _vec3 vTmp;

    vTmp.x = floor(_v.x) + 0.5f;
    vTmp.y = floor(_v.y) + 0.5f;
    vTmp.z = floor(_v.z) + 0.5f;
    
    m_pTransformCom->Set_Pos(vTmp.x, vTmp.y, vTmp.z);
}

HRESULT CShowBox::Create_Block()
{
    static int s_BlockIndex = 0;

    CScene* pScene = CManagement::GetInstance()->Get_Scene();
    CLayer* pLayer = pScene->Get_Layer(L"Block_Layer");

    if (nullptr == pLayer)
        return E_FAIL;

    Engine::CGameObject* pGameObject = nullptr;

    pGameObject = CBlock::Create(m_pGraphicDev);

    if (nullptr == pGameObject)
        return E_FAIL;

    CTransform* pObjectTransformCom = dynamic_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"));

    _vec3 vTmp;
    m_pTransformCom->Get_Info(INFO_POS, &vTmp);
    pObjectTransformCom->Set_Pos(vTmp.x, vTmp.y, vTmp.z);

    _tchar szTag[64] = {};

    while (true) {
        _stprintf_s(szTag, 64, L"Block_%d", s_BlockIndex);
        _tchar* pTag = new _tchar[lstrlen(szTag) + 1];
        lstrcpy(pTag, szTag);

        if (SUCCEEDED(pLayer->Add_GameObject(pTag, pGameObject))) {
            break; // 성공 시 탈출
        }
        else {
            Safe_Delete(pTag); // 실패 시 메모리 해제 후 시도 계속
            ++s_BlockIndex;
        }
    }

    //지금 오류가 계속 뜨는이유? << const TChar*로 되어있어서 지금 주소가 같아서 오류가 나는것 << 이거어떻게해야함?
    //임시방편으로 new를 이용해서 만들고 있지만 수정할 방안필요. 

    CMapToolMgr::GetInstance()->Plant_Block("NORMAL", vTmp, "DOWN");
    s_BlockIndex++;
    return S_OK;
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
