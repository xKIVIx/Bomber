#pragma once
#ifndef _gCONTROL_HEADER
#define _gCONTROL_HEADER
#include <thread>
#include <vector>

#include <gNET\gNET.h>

#include "gCONVEER.h"
#include "gOBJECTS.h"

#ifdef gCONTROL_DLL
#define gCONTROL_API _declspec(dllexport)
#else
#define gCONTROL_API _declspec(dllimport)
#endif

#define WM_WIN 1001
#define WM_LOSE 1002

class gCONTROL_API gCONTROL
{
public:
	std::vector <vOBJECT> GetObjectsForRend();
	gCONTROL(HWND h_wnd, unsigned int map_width, unsigned int map_height , char * host_name);
	~gCONTROL();
	void Command(short int player_id,char key);
	void InitMap(unsigned int map_width,unsigned int map_height);
	void StopGame();
	void Boom(unsigned int x, unsigned int y);
	void Boom(unsigned int x, unsigned int y, unsigned power);
	void GetComSecondPlayer(short id_second_player);
	void CheckInFire();
private:
	class CELL
	{
	public:
		CELL();
		CELL(gOBJECT * new_object);
		CELL(CELL & copy);
		~CELL();
		bool IsFire();
		bool GetRendInfo(vOBJECT * out);
		void Delete();
		void SetNewObject(gOBJECT * new_object);
		bool Damage(int * power_bomb);
		bool Colis();
		unsigned int GetBombPower();
	private:
		std::mutex lock_;
		gOBJECT * object_;
	};
	unsigned move_cd_ = 0;
	unsigned time_to_next_airbomb_ = 30000;
	gNET * g_net_one;
	HWND h_wnd_;
	bool stop = 0;
	std::thread game_thread;
	std::mutex lock_objects_, lock_stop_stat_;
	std::vector <CELL> objects_;
	gCONVEER do_list_;
	gOBJECT_PERSON * players[2];
	unsigned int map_width_, map_height_;
	unsigned int TransCoord(unsigned int x, unsigned int y);
	bool CheckStopStat();
	void GameProcess();
};

#endif // !_gCONTROL_HEADER