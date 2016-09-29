// mCONTROL control game state
#pragma once
#include <vector>
#include <deque>
#include <gCONTROL\gCONTROL.h>
enum MENU_STATE
{
	LOAD,
	MAIN,
	GAME,
	SEC_MENU,
	WAIT,
	END_GAME
};
class mCONTROL
{
public:
	mCONTROL(HWND h_wnd);
	~mCONTROL();
	bool CheckStopState();
	void Stop();
	void DoCommand();
	void AddCom(char key);
	void Comand(char key);
	std::vector <vOBJECT> GetRendInfo();
	void SetMain();
	void SetSecondMenu();
	void SetGame(char * host_name);
	void SetLoad();
	void SetWin();
	void SetLose();
	void Close();
private:
	class MENU_OBJECT
	{
	public:
		MENU_OBJECT();
		MENU_OBJECT(float x_pos, 
			float y_pos,
			float z_pos,
			float x_size, 
			float y_size, 
			unsigned texture_id, 
			std::function <void()> what_do);
		MENU_OBJECT(float x_pos,
			float y_pos,
			float z_pos,
			float x_size,
			float y_size,
			unsigned texture_id);
		vOBJECT GetRendInfo();
		bool Select();
		void Unselect();
		void Use();
	private:
		float x1, y1, x2, y2;
		std::function <void()> func_;
		vOBJECT rend_info_;
		MENU_STATE new_state_;
		bool usable_ = 0;
	};
	bool stop = 0;
	int now_menu_select_ = 0;
	HWND h_wnd_;
	gCONTROL * g_control = NULL;
	MENU_STATE now_state_ = LOAD;
	std::thread doing_thread_;
	std::mutex lock_objects_, lock_do_list_ , lock_stop_stat_;
	std::deque <char> do_list_;
	std::vector <MENU_OBJECT> menu_objects_;
};