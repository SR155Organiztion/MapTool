#include "CImguiMgr.h"	
#include <d3d9.h>
#include <d3dx9.h>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx9.h"
#include "CMapToolMgr.h"
#include "CCollisionMgr.h"
#include <sstream>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


IMPLEMENT_SINGLETON(CImguiMgr);

CImguiMgr::CImguiMgr() : m_iCurrent_Item(0), m_iCurrent_Food(0), m_bTerrainEnable(true), iX(0), iY(0)
{
   
}

CImguiMgr::~CImguiMgr()
{
    Free();
}

void CImguiMgr::Set_WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam);
}

HRESULT CImguiMgr::Ready_Imgui(LPDIRECT3DDEVICE9 pGraphicDev, HWND hWnd)
{
    float main_scale = ImGui_ImplWin32_GetDpiScaleForMonitor(::MonitorFromPoint(POINT{ 0, 0 }, MONITOR_DEFAULTTOPRIMARY));

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup scaling
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
    style.FontScaleDpi = main_scale;        // Set initial font scale. (using io.ConfigDpiScaleFonts=true makes this unnecessary. We leave both here for documentation purpose)

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hWnd);
    ImGui_ImplDX9_Init(pGraphicDev);

    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    
    strcpy_s(szName, sizeof(szName), CMapToolMgr::GetInstance()->Get_Name().c_str());
    fTimer = CMapToolMgr::GetInstance()->Get_Data(szName).Time;
    
    m_mapRecipes = CMapToolMgr::GetInstance()->Get_RecipeMap();

    iX = iY = 0;

    return S_OK;
}

void CImguiMgr::Update_Imgui()
{
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("MapTool");                       

    //���� �� �̸�
    ImGui::InputText("Scene", szName, sizeof(szName));
    
    //���� �� �ҷ����� �� �ʱ�ȭ
    if (ImGui::Button("Save")) {                 
        if (m_LoadCallback) {
            CMapToolMgr::GetInstance()->Set_Timer(fTimer);
            CMapToolMgr::GetInstance()->Set_MapSize(iX, iY);
            for (auto it : m_mapRecipes) {
                if(it.second == true)
                    CMapToolMgr::GetInstance()->Add_Recipe(it.first);
            }
            CMapToolMgr::GetInstance()->Save_Json();
        }
    }

    ImGui::SameLine();

    if (ImGui::Button("Load")) {   
        if (m_LoadCallback) {
            //�ҷ�����
            CMapToolMgr::GetInstance()->Load_Json();
            //������Ʈ��
            m_LoadCallback();
           
            //���̸�
            strcpy_s(szName, sizeof(szName), CMapToolMgr::GetInstance()->Get_Name().c_str());
            
            //�ʻ�����
            iX = CMapToolMgr::GetInstance()->Get_Data(szName).MapSize.iX;
            iY = CMapToolMgr::GetInstance()->Get_Data(szName).MapSize.iY;

            //Ÿ�̸�
            fTimer = CMapToolMgr::GetInstance()->Get_Data(szName).Time;
            
            //������
            m_mapRecipes = CMapToolMgr::GetInstance()->Get_RecipeMap();
            for (auto& it : m_mapRecipes) {
                it.second = false;
            }
            for (const auto Recipe : (CMapToolMgr::GetInstance()->Get_Data(szName).Recipe)) {
            
                for (auto& it : m_mapRecipes) {
                    if (Recipe == it.first) {
                        it.second = true;
                    }
                }
            }
        }
    }

    ImGui::SameLine();

    if (ImGui::Button("Clear")) {
        if (m_ClearCallback) {
            CMapToolMgr::GetInstance()->Reset();
            m_ClearCallback();
        }
    }

    /// �� ����â /////////////////////////////////////////////////////////////////
    const auto& nameVec = *CMapToolMgr::GetInstance()->Get_NameVec();

    std::vector<std::string> labeledNames;
    for (int i = 0; i < nameVec.size(); ++i) {
        labeledNames.push_back(nameVec[i] + "##" + std::to_string(i));
    }

    std::vector<const char*> comboItems;
    for (const auto& str : labeledNames)
        comboItems.push_back(str.c_str());

    static int current_item = 0;
    //���� ��� �� ������ Ȯ�� ��Ź��
    if (ImGui::Combo("SceneList", &current_item, comboItems.data(), comboItems.size())) {
        nameVec[current_item];
    }

    if (ImGui::Button("SetScene")) {
        if (m_LoadCallback) {
            CMapToolMgr::GetInstance()->Reset();
            CMapToolMgr::GetInstance()->Set_Name(nameVec[current_item]);
            CMapToolMgr::GetInstance()->Select_Map();
            //�������� �̸�
            strcpy_s(szName, nameVec[current_item].c_str());
            //�ʻ�����
            iX = CMapToolMgr::GetInstance()->Get_Data(szName).MapSize.iX;
            iY = CMapToolMgr::GetInstance()->Get_Data(szName).MapSize.iY;
            //Ÿ�̸�
            fTimer = CMapToolMgr::GetInstance()->Get_Data(szName).Time;
            //������
            m_mapRecipes = CMapToolMgr::GetInstance()->Get_RecipeMap();

            for (auto& it : m_mapRecipes) {
                it.second = false;
            }
            for (const auto Recipe : (CMapToolMgr::GetInstance()->Get_Data(szName).Recipe)) {

                for (auto& it : m_mapRecipes) {
                    if (Recipe == it.first) {
                        it.second = true;
                    }
                }
            }

            m_LoadCallback();
        }
    }

    ImGui::SameLine();

    if (ImGui::Button("Delete")) {
        CMapToolMgr::GetInstance()->Set_NoCreate();
        CMapToolMgr::GetInstance()->Delete_Map(nameVec[current_item]);
    }

    // Ÿ�̸� ���� ////////////////////////////////////////////////////////////////

    ImGui::InputFloat("Timer", &fTimer, sizeof(fTimer)); 

    // ������ ���� ////////////////////////////////////////////////////////////////

    if (ImGui::CollapsingHeader("Recipes")) {
        for (auto& it : m_mapRecipes) {
            ImGui::Checkbox(it.first.c_str(), &it.second);
        }
    }

    // ���� ���� ���� ////////////////////////////////////////////////////////////////

    if (ImGui::CollapsingHeader("Food")) {
        const char* foods[] = { "Lettuce", "Tomato", "Cucumber", "Fish", "Shrimp", "Seaweed", "Rice", "Pasta", "TomatoSoup"};
        ImGui::Combo("Foods", &m_iCurrent_Food, foods, IM_ARRAYSIZE(foods));
    }

    // �� ������ ���� ////////////////////////////////////////////////////////////////

    if (ImGui::CollapsingHeader("Item")) {
        const char* tools[] = { "None", "Plate", "Extinguisher", "Frypan", "Pot" };
        ImGui::Combo("Items", &m_iCurrent_Item, tools, IM_ARRAYSIZE(tools));
    }

    // ���� ���� ////////////////////////////////////////////////////////////////

    if (ImGui::CollapsingHeader("Terrain")) {
        ImGui::Checkbox(m_bTerrainEnable ? "Enable" : "Disable" , &m_bTerrainEnable);
        ImGui::SameLine();
        if (ImGui::Button("Change")) {
            m_bTerrainEnable ? m_bTerrainEnable = false : m_bTerrainEnable = true;
            m_TerrianEnableCallback();
        }
        
        ImGui::PushItemWidth(100);
        ImGui::InputInt("X", &iX, sizeof(int));
        ImGui::SameLine();
        ImGui::InputInt("Y", &iY, sizeof(int));
        ImGui::PopItemWidth();
    }

    // ȯ�������Ʈ ���� ////////////////////////////////////////////////////////////////

    if (ImGui::CollapsingHeader("EnvObj")) {
        static float pos[3] = { 1.0f, 1.0f, 1.0f };

        ImGui::Text("Now Obj : ");
        ImGui::SameLine();
        ImGui::Text(CMapToolMgr::GetInstance()->EnvObj_To_String().c_str());

        // Position �ؽ�Ʈ�� �巡�� ���� ����
        ImGui::Text("Position");
        ImGui::SameLine(100); // X ��ġ�� ������ ���� ���� (��: 100�ȼ����� ����)
        ImGui::DragFloat3("##Position", pos, 0.1f, 1.0f, 5.0f);
    }

    // ����׿� ////////////////////////////////////////////////////////////////
  
        static int blockCount = 0;
        static int tileCount = 0;
        static int envCount = 0;
        static int recipeCount = 0;

        CMapToolMgr::GetInstance()->Print_CurrentDataCounts(blockCount, tileCount, envCount, recipeCount);
        ImGui::Text("=====================================");
        ImGui::Text("Block: %d", blockCount);
        ImGui::Text("Tile: %d", tileCount);
        ImGui::Text("Environment: %d", envCount);
        ImGui::Text("Recipe: %d", recipeCount);

    //����Ȯ��
    string S = "Direction : " + CMapToolMgr::GetInstance()->Get_Dir();
    ImGui::Text(S.c_str());

    //�浹 ��ġ
    _vec3 colpos = CCollisionMgr::GetInstance()->Get_ColPos();
    char buf[64];
    sprintf_s(buf, sizeof(buf), "X: %.2f | Y: %.2f | Z: %.2f", colpos.x, colpos.y, colpos.z);
    ImGui::Text(buf);
        
    //���� ��ġ+++++++
    _vec3 RayPos, RayDir;
    CCollisionMgr::GetInstance()->Get_Ray(&RayPos, &RayDir);
    char buf1[64];
    char buf2[64];
    sprintf_s(buf1, sizeof(buf1), "RayPos(X:%.2f | Y:%.2f | Z:%.2f)", RayPos.x, RayPos.y, RayPos.z);
    sprintf_s(buf2, sizeof(buf2), "RayDir(X:%.2f | Y:%.2f | Z:%.2f)", RayDir.x, RayDir.y, RayDir.z);
    ImGui::Text(buf1);
    ImGui::Text(buf2);

    ImGui::End();
}

void CImguiMgr::LateUpdate_Imgui(const _float& fTimeDelta)
{
}

void CImguiMgr::Render_Imgui()
{
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

void CImguiMgr::Free()
{
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}