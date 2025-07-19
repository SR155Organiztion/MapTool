#include "../Header/json.hpp"
#include "CMapToolMgr.h"
#include "CManagement.h"
#include <fstream>
#include "CImguiMgr.h"

using json = nlohmann::json;

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(_vec3, x, y, z)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(S_BLOCK, Block_Type, vPos, Direction, Item)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(S_GAMEOBJECT, Block)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(S_TILE, Tile_Type, vPos, Direction)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(S_ENVOBJECT, Env_Type, vPos, fAngle, vScale)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(S_ENVIRONMENT, Tile, EnvObject)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(S_MAPSIZE, iX, iY)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(S_PLAYER, P1, P2)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(S_EVENT, bEvent, fEventTime)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(S_STAGE, MapSize, Player, Time, Event, Recipe, GameObject, Environment);

IMPLEMENT_SINGLETON(CMapToolMgr)

CMapToolMgr::CMapToolMgr()
    : m_fAngle(0.f), m_iPlayer(0), m_sName("None"), m_iSelectName(0), m_iDir(DIRECTIONID::NX), m_iObject(CREATEOBJECT_ID::O_BLOCK), m_iRcTile(0), m_fTimer(0.f), m_bCreate(true)
{
    m_tBlockVec.clear();
    m_tTileVec.clear();
    m_tEnvObjVec.clear();
    m_sRecipeVec.clear();
    m_mapJson.clear();
    m_tEvent.bEvent = false;
    m_tEvent.fEventTime = 0.f;
    {
        m_mapRecipes["salad_lettuce"] = false;
        m_mapRecipes["salad_lettuce_tomato"] = false;
        m_mapRecipes["salad_cucumber_lettuce_tomato"] = false;
        m_mapRecipes["sashimi_fish"] = false;
        m_mapRecipes["sashimi_shrimp"] = false;
        m_mapRecipes["sushi_fish"] = false;
        m_mapRecipes["sushi_cucumber"] = false;
        m_mapRecipes["pasta_tomato"] = false;
    }
}

CMapToolMgr::~CMapToolMgr()
{
}

void CMapToolMgr::Plant_Block(_vec3 _vPos)
{
    S_BLOCK tBlock = { Block_To_String() , _vPos, Dir_To_String(), Item_To_String() };
    m_tBlockVec.push_back(tBlock);
}

void CMapToolMgr::Plant_Block(string _sType, _vec3 _vPos, string _sDir, string _sItem)
{
    S_BLOCK tBlock = { _sType , _vPos, _sDir, _sItem };
    m_tBlockVec.push_back(tBlock);
}

void CMapToolMgr::Break_Block(_vec3 _vPos)
{
    for (vector<S_BLOCK>::iterator it = m_tBlockVec.begin(); it != m_tBlockVec.end(); ) {
        if ((*it).vPos == _vPos) {
            it = m_tBlockVec.erase(it);
        }
        else {
            it++;
        }
    }
}

void CMapToolMgr::Plant_Tile(_vec3 _vPos)
{
    S_TILE tTile = { Tile_To_String(), _vPos, Dir_To_String() };
    m_tTileVec.push_back(tTile);
}

void CMapToolMgr::Plant_HexTile(_vec3 _vPos)
{
    S_TILE tTile = { "TileHex", _vPos, Dir_To_String()};
    m_tTileVec.push_back(tTile);
}


void CMapToolMgr::Plant_Tile(string _sType, _vec3 _vPos, string _sDir)
{
    S_TILE tTile = { _sType, _vPos, _sDir };
    m_tTileVec.push_back(tTile);
}

void CMapToolMgr::Break_Tile(_vec3 _vPos)
{
    for (vector<S_TILE>::iterator it = m_tTileVec.begin(); it != m_tTileVec.end(); ) {
        if ((*it).vPos == _vPos) {
            it = m_tTileVec.erase(it);
        }
        else {
            it++;
        }
    }
}

void CMapToolMgr::Plant_Environment(_vec3 _vPos)
{
    S_ENVOBJECT tEnvObj = { EnvObj_To_String() , _vPos, CImguiMgr::GetInstance()->Get_Angle(), CImguiMgr::GetInstance()->Get_NowScale()};
    m_tEnvObjVec.push_back(tEnvObj);
}

void CMapToolMgr::Plant_Environment(string _sType, _vec3 _vPos, float _fAngle, _vec3 vScale)
{
    S_ENVOBJECT tEnvObj = { _sType , _vPos, _fAngle, vScale };
    m_tEnvObjVec.push_back(tEnvObj);
}

void CMapToolMgr::Break_Environment(_vec3 _vPos)
{
    for (vector<S_ENVOBJECT>::iterator it = m_tEnvObjVec.begin(); it != m_tEnvObjVec.end(); ) {
        if ((*it).vPos == _vPos) {
            it = m_tEnvObjVec.erase(it);
        }
        else {
            it++;
        }
    }
}

void CMapToolMgr::Plant_Camera(_vec3 _vEye, _vec3 _vAt)
{
}

void CMapToolMgr::Plant_Player(_int _iPlayer, _vec3 _vPos)
{
    //저장할때 플레이어 위치조절
    _vPos.y += 0.5f;

    if (_iPlayer == 0) {
        m_tPlayer.P1 = _vPos;
    }
    else {
        m_tPlayer.P2 = _vPos;
    }
}

HRESULT CMapToolMgr::Save_Json()
{
    m_sName = CImguiMgr::GetInstance()->Get_Name();
    //데이터 종합
    S_GAMEOBJECT GameObject = { m_tBlockVec };
    S_ENVIRONMENT Environmnet = { m_tTileVec , m_tEnvObjVec };
    S_STAGE stage = { m_tMapSize, m_tPlayer , m_fTimer, m_tEvent, m_sRecipeVec, GameObject, Environmnet };
    //저장하기 전 json에 있는 모든 맵의 키값을 스테이지 이름 벡터와 비교찾는다
    bool bFound = false;
    for (auto it = m_mapJson.begin(); it != m_mapJson.end(); ++it) {
        //만약 겹치는 키 값이 있다면 그 키의 second를 비우고 현재 스테이지 데이터를 넣음
        if ((*it).first == m_sName) {
            (*it).second = stage;
            bFound = true;
            break;
        }
    }
    //없다면 추가함
    if (!bFound && m_bCreate) {
        m_mapJson.insert(pair<string, S_STAGE>(m_sName, stage));
    }
    json j = m_mapJson;
    /////////////////////////////////////
    ///데이터 저장시점
    {
        std::ofstream file("../Bin/Data/SaveData.json");
        if (!file.is_open()) {
            MSG_BOX("저장 오류");
            return E_FAIL;
        }
        file << j.dump(2);
        file.close();
    }

    //한번 불러와서 확인
    Load_Json();
    m_bCreate = true;
    return S_OK;
}

HRESULT CMapToolMgr::Load_Json()
{
    Reset();
    m_mapJson.clear();
    m_sNameVec.clear();
    std::ifstream file("../Bin/Data/SaveData.json");

    if (!file.is_open()) {
        MSG_BOX("파일 열기 실패");
        return E_FAIL;
    }

    {
        json j;
        file >> j;

        bool firstKey = false;
        // 순회하며 각 요소를 map<string, S_STAGE>로 변환
        for (auto it = j.begin(); it != j.end(); ++it)
        {
            const std::string& stageName = it.key();
            const json& stageJson = it.value();

            if (m_sNameVec.empty())
                m_sName = stageName;

            m_sNameVec.push_back(stageName);
            S_STAGE stage = stageJson.get<S_STAGE>();
            m_mapJson[stageName] = stage;
        }

        Select_Map();

        file.close();
    }

    return S_OK;
}

void CMapToolMgr::Select_Map()
{
    m_tMapSize = m_mapJson[m_sName].MapSize;
    m_tPlayer = m_mapJson[m_sName].Player;
    m_fTimer = m_mapJson[m_sName].Time;
    m_sRecipeVec = m_mapJson[m_sName].Recipe;
    m_tBlockVec = m_mapJson[m_sName].GameObject.Block;
    m_tTileVec = m_mapJson[m_sName].Environment.Tile;
    m_tEnvObjVec = m_mapJson[m_sName].Environment.EnvObject;

    for (const auto& recipeName : m_sRecipeVec) {
        auto it = m_mapRecipes.find(recipeName);
        if (it != m_mapRecipes.end()) {
            it->second = true;  // 키가 같으면 bool 값을 true로 설정
        }
    }
}

void CMapToolMgr::Delete_Map(string _s)
{
    auto it = m_mapJson.find(_s);

    if (it != m_mapJson.end()) {
        Reset();
        m_mapJson.erase(it);

        if (m_sName == _s && !m_mapJson.empty()) {
            m_sName = m_mapJson.begin()->first;
        }

        Save_Json();
    }
}

void CMapToolMgr::Reset()
{
    ZeroMemory(&m_tMapSize, sizeof(S_MAPSIZE));
    ZeroMemory(&m_tPlayer, sizeof(S_PLAYER));
    m_fTimer = 0.f;
    m_sRecipeVec.clear();
    m_tBlockVec.clear();
    m_tTileVec.clear();
    m_tEnvObjVec.clear();
    {
        m_mapRecipes["salad_lettuce"] = false;
        m_mapRecipes["salad_lettuce_tomato"] = false;
        m_mapRecipes["salad_cucumber_lettuce_tomato"] = false;
        m_mapRecipes["sashimi_fish"] = false;
        m_mapRecipes["sashimi_shrimp"] = false;
        m_mapRecipes["sushi_fish"] = false;
        m_mapRecipes["sushi_cucumber"] = false;
        m_mapRecipes["pasta_tomato"] = false;
    }
}

void CMapToolMgr::Set_NoCreate()
{
    m_bCreate = false;
}

S_STAGE CMapToolMgr::Get_Data(string s)
{
    auto it = m_mapJson.find(s);
    if (it != m_mapJson.end())
        return it->second;

    MSG_BOX("없는 스테이지");
    return S_STAGE{};
}

string CMapToolMgr::Get_Name()
{
    return m_sName;
}

void CMapToolMgr::Set_Name(string _s)
{
    for (auto a : m_sNameVec) {
        if (a == _s) {
            m_sName = _s;
            return;
        }
    }

    MSG_BOX("SetName Error");
}

vector<string>* CMapToolMgr::Get_NameVec()
{
    return &m_sNameVec;
}


void CMapToolMgr::Set_MapSize(int _iX, int _iY)
{
    m_tMapSize.iX = _iX;
    m_tMapSize.iY = _iY;
}

void CMapToolMgr::NextObject()
{
    ++m_iObject;
    if (m_iObject >= static_cast<_uint>(CREATEOBJECT_ID::O_END)) {
        m_iObject = 0;
    }
}

_uint CMapToolMgr::Get_NowObject()
{
    return m_iObject;
}
void CMapToolMgr::NextRotate()
{
    ++m_iDir;
    if (m_iDir == static_cast<_uint>(DIRECTIONID::DIR_END)) {
        m_iDir = 0;
    }
}

void CMapToolMgr::PrevRotate()
{
    --m_iDir;
    if (m_iDir < 0) {
        m_iDir = static_cast<_uint>(DIRECTIONID::DIR_END) - 1;
    }
}

_vec3 CMapToolMgr::Get_DirLook()
{
    switch (m_iDir)
    {
    case Engine::PX:
        return _vec3(0.f, D3DXToRadian(0.f), 0.f);
    case Engine::PZ:
        return _vec3(0.f, D3DXToRadian(90.f), 0.f);
    case Engine::NX:
        return _vec3(0.f, D3DXToRadian(180.f), 0.f);
    case Engine::NZ:
        return _vec3(0.f, D3DXToRadian(270.f), 0.f);
    default:
        return _vec3(0.f, 0.f, 0.f);
    }
}

//void CMapToolMgr::TurnRight(_float _fTimeDelta)
//{
//    m_fAngle -= 1.f * _fTimeDelta;
//}
//
//void CMapToolMgr::TurnLeft(_float _fTimeDelta)
//{
//    m_fAngle += 1.f * _fTimeDelta;
//}
//
//_float CMapToolMgr::Get_NowAngle()
//{
//    return m_fAngle;
//}

void CMapToolMgr::NextStation()
{
    ++m_iStation;
    if (m_iStation >= static_cast<_uint>(STATIONID::S_END)) {
        m_iStation = 0;
    }
}

void CMapToolMgr::PrevStation()
{
    --m_iStation;
    if (m_iStation < 0) {
        m_iStation = static_cast<_uint>(STATIONID::S_END) - 1;
    }
}

_uint CMapToolMgr::Get_NowStation()
{
    return m_iStation;
}


void CMapToolMgr::NextRcTile()
{
    ++m_iRcTile;
    if (m_iRcTile >= static_cast<_uint>(RCTILEID::RT_END)) {
        m_iRcTile = 0;
    }
}

void CMapToolMgr::PrevRcTile()
{
    --m_iRcTile;
    if (m_iRcTile < 0) {
        m_iRcTile = static_cast<_uint>(RCTILEID::RT_END) - 1;
    }
}

_uint CMapToolMgr::Get_NowRcTile()
{
    return m_iRcTile;
}

void CMapToolMgr::NextEnvObject()
{
    ++m_iEnvObject;
    if (m_iEnvObject >= static_cast<_uint>(ENVIRONMENTID::E_END)) {
        m_iEnvObject = 0;
    }
}

void CMapToolMgr::PrevEnvObject()
{
    --m_iEnvObject;
    if (m_iEnvObject < 0) {
        m_iEnvObject = static_cast<_uint>(ENVIRONMENTID::E_END) - 1;
    }
}

_uint CMapToolMgr::Get_NowEnvObject()
{
    return m_iEnvObject;
}

void CMapToolMgr::ChangePlayer()
{
    if (m_iPlayer == 0) {
        m_iPlayer = 1;
    }
    else {
        m_iPlayer = 0;
    }
}

_uint CMapToolMgr::Get_NowPlayer()
{
    return m_iPlayer;
}

string CMapToolMgr::Get_Dir()
{
    switch (m_iDir)
    {
    case Engine::DIRECTIONID::PX:
        return "PX";
    case Engine::DIRECTIONID::NX:
        return "NX";
    case Engine::DIRECTIONID::PZ:
        return "PZ";
    case Engine::DIRECTIONID::NZ:
        return "NZ";
    default:
        return "???";
    }
}

string CMapToolMgr::Dir_To_String()
{
    switch (m_iDir)
    {
    case Engine::DIRECTIONID::PX:
        return "PX";
    case Engine::DIRECTIONID::NX:
        return "NX";
    case Engine::DIRECTIONID::PZ:
        return "PZ";
    case Engine::DIRECTIONID::NZ:
        return "NZ";
    default:
        return "???";
    }
}
_vec3 CMapToolMgr::String_To_Dir(string& _s)
{
    if (_s == "PX") return _vec3(0.f, 0.f, 0.f);
    if (_s == "NX") return _vec3(0.f, D3DXToRadian(180.f), 0.f);
    if (_s == "PZ") return _vec3(0.f, D3DXToRadian(90.f), 0.f);
    if (_s == "NZ") return _vec3(0.f, D3DXToRadian(270.f), 0.f);

    // 잘못된 문자열일 경우
    return _vec3(0.f, 0.f, 0.f);
}

string CMapToolMgr::Block_To_String()
{
    switch (m_iStation)
    {
    case Engine::STATIONID::S_INV:
        return "InvWall";
        break;
    case Engine::STATIONID::S_EMPTY:
        return "Empty";
        break;
    case Engine::STATIONID::S_CREATE:
        return ("Create_" + Food_To_String());
        break;
    case Engine::STATIONID::S_CHOP:
        return "Chop";
        break;
    case Engine::STATIONID::S_GAS:
        return "Gas";
        break;
    case Engine::STATIONID::S_PLATE:
        return "Plate";
        break;
    case Engine::STATIONID::S_SINK_W:
        return "Sink_Wash";
        break;
    case Engine::STATIONID::S_SINK_P:
        return "Sink_Plate";
        break;
    case Engine::STATIONID::S_TRASH:
        return "Trash";
        break;
    case Engine::STATIONID::S_SERVING:
        return "Serving";
        break;
    case Engine::STATIONID::S_END:
        return "???";
        break;
    default:
        break;
    }

    return "???";
}
_uint CMapToolMgr::String_To_Block(string& _s)
{
    if (_s.find("Create_") == 0) {
        return Engine::STATIONID::S_CREATE;
    }

    if (_s == "InvWall")
        return Engine::STATIONID::S_INV;
    else if (_s == "Empty")
        return Engine::STATIONID::S_EMPTY;
    else if (_s == "Chop")
        return Engine::STATIONID::S_CHOP;
    else if (_s == "Gas")
        return Engine::STATIONID::S_GAS;
    else if (_s == "Plate")
        return Engine::STATIONID::S_PLATE;
    else if (_s == "Sink_Wash")
        return Engine::STATIONID::S_SINK_W;
    else if (_s == "Sink_Plate")
        return Engine::STATIONID::S_SINK_P;
    else if (_s == "Trash")
        return Engine::STATIONID::S_TRASH;
    else if (_s == "Serving")
        return Engine::STATIONID::S_SERVING;
    else if (_s == "???")
        return Engine::STATIONID::S_END;

    //에러
    return -1;
}

string CMapToolMgr::Tile_To_String()
{
    switch (m_iRcTile)
    {
    case Engine::RT_0:
        return "Tile_Blue33";
    case Engine::RT_1:
        return "Tile_Blue44";
    case Engine::RT_2:
        return "Tile_Pink44";
    case Engine::RT_3:
        return "Tile_StoneBrown";
    case Engine::RT_4:
        return "Tile_StoneBeige";
    case Engine::RT_END:
        break;
    default:
        break;
    }
    return "???";
}
_uint CMapToolMgr::String_To_Tile(string& _s)
{
    if (_s == "Tile_Blue33")
        return Engine::RCTILEID::RT_0;
    else if (_s == "Tile_Blue44")
        return Engine::RCTILEID::RT_1;
    else if (_s == "Tile_Pink44")
        return Engine::RCTILEID::RT_2;
    else if (_s == "Tile_StoneBrown")
        return Engine::RCTILEID::RT_3;
    else if (_s == "Tile_StoneBeige")
        return Engine::RCTILEID::RT_4;
    else if (_s == "???")
        return Engine::RCTILEID::RT_END;

    //에러처리
    return -1;
}

string CMapToolMgr::Item_To_String()
{
    switch (CImguiMgr::GetInstance()->Get_CurItem())
    {
    case Engine::I_NONE:
        return "";
    case Engine::I_PLATE:
        return "Plate";
    case Engine::I_EXTINGUISHER:
        return "Extinguisher";
    case Engine::I_FRYPAN:
        return "Frypan";
    case Engine::I_POT:
        return "Pot";
    case Engine::I_END:
    default:
        break;
    }
    return "";
}
_uint CMapToolMgr::String_To_Item(string& _s)
{
    if (_s == "Plate")
        return Engine::I_PLATE;
    else if (_s == "Extinguisher")
        return Engine::I_EXTINGUISHER;
    else if (_s == "Frypan")
        return Engine::I_FRYPAN;
    else if (_s == "Pot")
        return Engine::I_POT;
    else
        return Engine::I_NONE; // 혹은 예외 처리
}

string CMapToolMgr::Food_To_String()
{
    switch (CImguiMgr::GetInstance()->Get_CurFood())
    {
    case Engine::C_LETTUCE:
        return "Lettuce";
    case Engine::C_TOMATO:
        return "Tomato";
    case Engine::C_CUCUMBER:
        return "Cucumber";
    case Engine::C_FISH:
        return "Fish";
    case Engine::C_SHRIMP:
        return "Shrimp";
    case Engine::C_SEAWEED:
        return "Seaweed";
    case Engine::C_RICE:
        return "Rice";
    case Engine::C_PASTA:
        return "Pasta";
    case Engine::C_TOMATOSOUP:
        return "Tomatosoup";
    case Engine::C_END:
    default:
        break;
    }
    return "";
}
_uint CMapToolMgr::String_To_Food(string& _s)
{
    static const std::unordered_map<std::string, int> foodMap = {
        {"Seaweed", Engine::CREATEID::C_SEAWEED},
        {"Lettuce", Engine::CREATEID::C_LETTUCE},
        {"Tomato", Engine::CREATEID::C_TOMATO},
        {"Cucumber", Engine::CREATEID::C_CUCUMBER},
        {"Fish", Engine::CREATEID::C_FISH},
        {"Shrimp", Engine::CREATEID::C_SHRIMP},
        {"Rice", Engine::CREATEID::C_RICE},
        {"Pasta", Engine::CREATEID::C_PASTA},
        {"Tomatosoup", Engine::CREATEID::C_TOMATOSOUP}
    };

    auto it = foodMap.find(_s);
    if (it != foodMap.end())
        return it->second;

    return Engine::CREATEID::C_END; // 또는 -1 등 적절한 예외 처리
    return _uint();
}

string CMapToolMgr::EnvObj_To_String()
{
    //case Engine::ENVIRONMENTID:
    switch (m_iEnvObject)
    {
    case Engine::ENVIRONMENTID::E_FLAG:
        return ("Flag" + Stage_To_String());
    case Engine::ENVIRONMENTID::E_TREE_1:
        return "Tree_1";
    case Engine::ENVIRONMENTID::E_TREE_2:
        return "Tree_2";
    case Engine::ENVIRONMENTID::E_TREE_3:
        return "Tree_3";
    case Engine::ENVIRONMENTID::E_TREE_4:
        return "Tree_4";
    case Engine::ENVIRONMENTID::E_PLANT_1:
        return "Plant_1";
    case Engine::ENVIRONMENTID::E_PLANT_2:
        return "Plant_2";
    case Engine::ENVIRONMENTID::E_FLOWER_1:
        return "Flower_1";
    case Engine::ENVIRONMENTID::E_FLOWER_2:
        return "Flower_2";
    case Engine::ENVIRONMENTID::E_CASTLE:
        return "Castle";
    case Engine::ENVIRONMENTID::E_STONEWALL:
        return "Stonewall";
    case Engine::ENVIRONMENTID::E_WOODWALL:
        return "Woodwall";
    case Engine::E_END:
    default:
        break;
    }
    return "";
}
_uint CMapToolMgr::String_To_EnvObj(string& _s)
{
    if (_s.find("Flag") == 0) {
        return Engine::ENVIRONMENTID::E_FLAG;
    }

    if (_s == "Tree_1")
        return Engine::ENVIRONMENTID::E_TREE_1;
    else if (_s == "Tree_2")
        return Engine::ENVIRONMENTID::E_TREE_2;
    else if (_s == "Tree_3")
        return Engine::ENVIRONMENTID::E_TREE_3;
    else if (_s == "Tree_4")
        return Engine::ENVIRONMENTID::E_TREE_4;
    else if (_s == "Plant_1")
        return Engine::ENVIRONMENTID::E_PLANT_1;
    else if (_s == "Plant_2")
        return Engine::ENVIRONMENTID::E_PLANT_2;
    else if (_s == "Flower_1")
        return Engine::ENVIRONMENTID::E_FLOWER_1;
    else if (_s == "Flower_2")
        return Engine::ENVIRONMENTID::E_FLOWER_2;
    else if (_s == "Castle")
        return Engine::ENVIRONMENTID::E_CASTLE;
    else if (_s == "Stonewall")
        return Engine::ENVIRONMENTID::E_STONEWALL;
    else if (_s == "Woodwall")
        return Engine::ENVIRONMENTID::E_WOODWALL;
    else
        return Engine::E_END; // 혹은 예외 처리
}

string CMapToolMgr::Stage_To_String()
{
    switch (CImguiMgr::GetInstance()->Get_Stage())
    {
    case Engine::STAGE_0:
        return "_S0";
    case Engine::STAGE_1:
        return "_S1";
    case Engine::STAGE_2:
        return "_S2";
    case Engine::STAGE_3:
        return "_S3";
    case Engine::STAGE_4:
        return "_S4";
    case Engine::STAGE_5:
        return "_S5";
    case Engine::STAGE_6:
        return "_S6";
    case Engine::STAGE_END:
    default:
        return "_??";
    }

    return "_??";
}
_uint CMapToolMgr::String_To_Stage()
{
    return _uint();
}

void CMapToolMgr::Free()
{
    m_tBlockVec.clear();
    m_tTileVec.clear();
    m_tEnvObjVec.clear();
    m_sRecipeVec.clear();
    m_mapJson.clear();
}