#include "CScene.h"

CScene::CScene(LPDIRECT3DDEVICE9 pGraphicDev)
    : m_pGraphicDev(pGraphicDev)
{
    m_pGraphicDev->AddRef();
}

CScene::~CScene()
{
}

CComponent* CScene::Get_Component(COMPONENTID eID, const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag)
{
    auto    iter = find_if(m_mapLayer.begin(), m_mapLayer.end(), CTag_Finder(pLayerTag));

    if (iter == m_mapLayer.end())
        return nullptr;

    return iter->second->Get_Component(eID, pObjTag, pComponentTag);
}

CLayer* CScene::Get_Layer(const _tchar* pLayerTag)
{
    for (auto it = m_mapLayer.begin(); it != m_mapLayer.end(); ++it) {
        if (0 == lstrcmpW(pLayerTag, it->first))
            return it->second;
    }

    return nullptr;
}

HRESULT CScene::Ready_Scene()
{
    return S_OK;
}

_int CScene::Update_Scene(const _float& fTimeDelta)
{
    for (auto& pLayer : m_mapLayer)
        pLayer.second->Update_Layer(fTimeDelta);

    return 0;
}

void CScene::LateUpdate_Scene(const _float& fTimeDelta)
{
    for (auto& pLayer : m_mapLayer)
        pLayer.second->LateUpdate_Layer(fTimeDelta);
}


void CScene::Free()
{
    for_each(m_mapLayer.begin(), m_mapLayer.end(), CDeleteMap());
    m_mapLayer.clear();

    Safe_Release(m_pGraphicDev);
}
