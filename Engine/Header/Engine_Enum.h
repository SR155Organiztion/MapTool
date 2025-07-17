#ifndef Engine_Enum_h__
#define Engine_Enum_h__

namespace Engine
{
	enum WINMODE { MODE_FULL, MODE_WIN };

	// Dynamic 컴포넌트 경우 매 프레임마다 갱신해야하는 컴포넌트 집단
	enum COMPONENTID { ID_DYNAMIC, ID_STATIC, ID_END };

	enum INFO {	INFO_RIGHT, INFO_UP, INFO_LOOK, INFO_POS, INFO_END };
	enum ROTATION { ROT_X, ROT_Y, ROT_Z, ROT_END };

	enum TEXTUREID { TEX_NORMAL, TEX_CUBE, TEX_END };

	enum RENDERID { RENDER_PRIORITY, RENDER_NONALPHA, RENDER_ALPHA, RENDER_UI, RENDER_END };

	enum MOUSEKEYSTATE { DIM_LB, DIM_RB, DIM_MB, DIM_END };
	enum MOUSEMOVESTATE { DIMS_X, DIMS_Y, DIMS_Z, DIMS_END };

	//방향			   
	enum DIRECTIONID { PZ, PX, NZ, NX , DIR_END };
	//블럭     	    투명벽    빈칸   음식상자   도마    가스    접시   물씽크대  접시싱크  쓰레기통    제출
	enum STATIONID { S_INV, S_EMPTY, S_CREATE, S_CHOP, S_GAS, S_PLATE, S_SINK_W, S_SINK_P, S_TRASH, S_SERVING, S_END };
	//음식
	enum CREATEID { C_LETTUCE, C_TOMATO, C_CUCUMBER, C_FISH, C_SHRIMP, C_SEAWEED, C_RICE, C_PASTA, C_TOMATOSOUP, C_END};
	//타일		   파33   파44  핑44  돌갈  돌베   X     끝
 	enum RCTILEID { RT_0, RT_1, RT_2, RT_3, RT_4, RT_5, RT_END};
	//오브젝트			    블럭	사각타일   육각타일   환경  시작지점
	enum CREATEOBJECT_ID { O_BLOCK, O_RCTILE, O_HEXTILE, O_ENV, O_SPAWN ,O_END };
	//아이템
	enum ITEMID { I_NONE, I_PLATE, I_EXTINGUISHER, I_FRYPAN, I_POT, I_END };
	//스테이지
	enum STAGEID { STAGE_0, STAGE_1, STAGE_2, STAGE_3, STAGE_4, STAGE_5, STAGE_6, STAGE_END }; 
	//환경				 
	enum ENVIRONMENTID { E_FLAG,
						 E_TREE_1, E_TREE_2, E_TREE_3, E_TREE_4,
						 E_PLANT_1, E_PLANT_2, 
					     E_FLOWER_1, E_FLOWER_2,
						 E_CASTLE,
						 E_END };

}
#endif // Engine_Enum_h__
