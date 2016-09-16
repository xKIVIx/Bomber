#pragma once
#ifndef _gCONTROL_HEADER
#define _gCONTROL_HEADER
#include <thread>
#include <vector>
#include <Windows.h>

#include "gCONVEER.h"
#include "gOBJECTS.h"

#ifdef gCONTROL_DLL
#define gCONTROL_API _declspec(dllexport)
#else
#define gCONTROL_API _declspec(dllimport)
#endif

class gCONTROL_API gCONTROL
{
public:
	std::vector <vOBJECT> GetObjectsForRend();
	gCONTROL(HWND h_wnd, unsigned int map_width, unsigned int map_height);
	~gCONTROL();
	void Command(char key);
	void InitMap(unsigned int map_width,unsigned int map_height);
	void StopGame();
private:
	class CELL
	{
	public:
		CELL();
		CELL(gOBJECT * new_object);
		CELL(CELL & copy);
		~CELL();
		bool GetRendInfo(vOBJECT * out);
		void Delete();
		void SetNewObject(gOBJECT * new_object);
		bool Damage(int * power_bomb);
		int Colis();
	private:
		std::mutex lock_;
		gOBJECT * object_;
	};
	HWND h_wnd_;
	bool stop = 0;
	std::thread game_thread;
	std::mutex lock_objects_, lock_stop_stat_;
	std::vector <CELL> objects_;
	gCONVEER do_list_;
	gOBJECT_PERSON * player_one_, *player_two_;
	unsigned int map_width_, map_height_;
	unsigned int TransCoord(unsigned int x, unsigned int y);
	bool CheckStopStat();
	void GameProcess();
	void Boom(unsigned int x, unsigned int y);
};

#endif // !_gCONTROL_HEADER