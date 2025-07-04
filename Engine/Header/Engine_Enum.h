#ifndef Engine_Enum_h__
#define Engine_Enum_h__

namespace Engine
{
	enum WINMODE { MODE_FULL, MODE_WIN };

	// Dynamic ������Ʈ ��� �� �����Ӹ��� �����ؾ��ϴ� ������Ʈ ����
	enum COMPONENTID { ID_DYNAMIC, ID_STATIC, ID_END };

	enum INFO {	INFO_RIGHT, INFO_UP, INFO_LOOK, INFO_POS, INFO_END };
	enum ROTATION { ROT_X, ROT_Y, ROT_Z, ROT_END };

	enum TEXTUREID { TEX_NORMAL, TEX_CUBE, TEX_END };

	enum RENDERID { RENDER_PRIORITY, RENDER_NONALPHA, RENDER_ALPHA, RENDER_UI, RENDER_END };

	enum MOUSEKEYSTATE { DIM_LB, DIM_RB, DIM_MB, DIM_END };
	enum MOUSEMOVESTATE { DIMS_X, DIMS_Y, DIMS_Z, DIMS_END };

	enum DIRECTIONID { PX, PZ, NX, NZ , DIR_END };
	enum STATIONID { S_INV, S_EMPTY, S_CREATE, S_CHOP, S_GAS, S_PLATE, S_SINK_W, S_SINK_P, S_TRASH, S_SERVING, S_END };
	//��     	    ����    ��ĭ   ���Ļ���   ����    ����    ����   ����ũ��  ���ý�ũ  ��������  ����
	enum CREATEID { C_SEAWEED, C_LETTUCE, C_TOMATO, C_CUCUMBER, C_FISH, C_SHRIMP, C_RICE, C_PASTA, C_END};
	//����		  
}
#endif // Engine_Enum_h__
