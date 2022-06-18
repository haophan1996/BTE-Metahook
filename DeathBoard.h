#pragma once 


struct DeathBoardMSG {
	int victimID; // Player take damage 
	int victimDamage; //victimDamage, if victimDamage == -10 Headshot, if victimDamage == -20 Knife, if victimDamage == -30 Normal 
};

enum PanelController
{
	removePanel = 0,
	autoDis = 1,
	manual = 2
	 
};

extern std::vector<DeathBoardMSG> g_DeathBoardMSG;

class CHudDeathBoard : public CHudBase
{
public:
	void Init(void);
	void VidInit(void);
	void Draw(float flTime);  

public:
	int SPECTATE_HITNODE_10, SPECTATE_HITNODE_20, SPECTATE_HITNODE_30;
	char szWpnNameKiller[64];
	int iKillerID;
	int maxY;
	int autoDisappear;
	bool isBackSpacePress;
	bool isToggle;
	int currentY;
	float startTime;

private: 
	float disappearTime; //5s
	float currentTime;
	float BezierBlend(float t, float duration);
	int SPECTATE_MAIN, SPECTATE_UNDER;
	int SPECTATE_UNDER_USERINFO; 
	int KillerHint_ICON_Start, KillerHint_ICON_SpaceBetween, KillerHint_Name_Start, KillerHint_Name_SpaceBetween;

};


CHudDeathBoard& HudDeathBoard();