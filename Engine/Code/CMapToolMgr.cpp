#include "CMapToolMgr.h"

CMapToolMgr::CMapToolMgr()
{
}

CMapToolMgr::~CMapToolMgr()
{
}

void CMapToolMgr::Plant_Block()
{
}

void CMapToolMgr::Plant_Tile()
{
}

void CMapToolMgr::Plant_Environment()
{
}

void CMapToolMgr::Plant_Camera()
{
}

void CMapToolMgr::Plant_Player()
{
}

HRESULT CMapToolMgr::Save_Json()
{
    return E_NOTIMPL;
}

HRESULT CMapToolMgr::Load_Json()
{
    return E_NOTIMPL;
}

void CMapToolMgr::Reset()
{
}

void CMapToolMgr::Key_Input()
{
}

void CMapToolMgr::Free()
{
    for (auto pBlock : m_pBlockData) {
        Safe_Delete(pBlock);
    }
    for (auto pTile : m_pTileData) {
        Safe_Delete(pTile);
    }
    for (auto pEnv : m_pEnvData) {
        Safe_Delete(pEnv);
    }
}