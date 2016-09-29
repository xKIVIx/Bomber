// this dll control all game processe

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

// id window message
#define WM_WIN 1001
#define WM_LOSE 1002

class gCONTROL_API gCONTROL
{
public:
	std::vector <vOBJECT> GetObjectsForRend();
	gCONTROL(HWND h_wnd, unsigned int map_width, unsigned int map_height , char * host_name);
	~gCONTROL();
	void Command(short int player_id,char key);
	// init game 
	void InitMap(unsigned int map_width,unsigned int map_height);
	// stop game thread
	void StopGame();
	// make fire lines
	void Boom(unsigned int x, unsigned int y);
	void Boom(unsigned int x, unsigned int y, unsigned power);
	// get command from net
	void GetComSecondPlayer(short id_second_player);
	// check placed player in fire
	void CheckInFire();
private:
	// game fields
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
	// cd time for movement
	unsigned move_cd_ = 0;
	// net inteface
	gNET * g_net;
	// window handle
	HWND h_wnd_;
	// stop state  for game thred
	bool stop = 0;
	// game threa
	std::thread game_thread;
	std::mutex lock_objects_, lock_stop_stat_;
	// game map
	std::vector <CELL> objects_;
	// function list for game thread
	gCONVEER do_list_;
	// players
	gOBJECT_PERSON * players[2];
	// map parametrs
	unsigned int map_width_, map_height_;
	// translate coord into id element in vector
	unsigned int TransCoord(unsigned int x, unsigned int y);
	bool CheckStopStat();
	// doing finction from do_list_
	void GameProcess();
};

#endif // !_gCONTROL_HEADER