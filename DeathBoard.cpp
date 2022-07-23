#include "hud.h" 
#include "BaseUI.h"
#include "message.h"
#include "parsemsg.h"
#include "util.h"
#include "Encode.h"
 
#include "Fonts.h" 
#include "Client/WeaponManager.h" 
#include "Client/HUD/FontText.h"
#include "DeathBoard.h" 

 
#include "Client/HUD/DrawTGA.h"
#include <BTE-Metahook/DrawTargaImage.h>
#include <HUD/DrawTABPanel.h>
#include <BTE-Metahook/exportfuncs.h>

static CHudDeathBoard g_HudDeathBoard; 
std::vector<DeathBoardMSG> g_DeathBoardMSG;

CHudDeathBoard& HudDeathBoard()
{
	return g_HudDeathBoard;
}

void CHudDeathBoard::Init(void)
{
	m_iFlags |= HUD_ACTIVE; 
	isBackSpacePress = false;
	isToggle = false; 
}

void CHudDeathBoard::VidInit(void)
{
	SPECTATE_MAIN = Hud().m_TGA.FindTexture("gfx\\charSystem\\SPECTATEUI\\SPECTATE_MAIN");
	SPECTATE_UNDER = Hud().m_TGA.FindTexture("gfx\\charSystem\\SPECTATEUI\\SPECTATE_UNDER");
	SPECTATE_UNDER_USERINFO = Hud().m_TGA.FindTexture("gfx\\charSystem\\SPECTATEUI\\SPECTATE_UNDER_USERINFO");
	SPECTATE_HITNODE_10 = Hud().m_TGA.FindTexture("gfx\\charSystem\\SPECTATEUI\\SPECTATE_HITNODE-10");
	SPECTATE_HITNODE_20 = Hud().m_TGA.FindTexture("gfx\\charSystem\\SPECTATEUI\\SPECTATE_HITNODE-20");
	SPECTATE_HITNODE_30 = Hud().m_TGA.FindTexture("gfx\\charSystem\\SPECTATEUI\\SPECTATE_HITNODE-30");
	maxY = ScreenHeight - g_Texture[SPECTATE_MAIN].iHeight - 20;

	char iData[10];
	GetPrivateProfileStringA("SPECTATEUI", "KillerHint_ICON_Start", "157", iData, sizeof(iData), "cstrike/gfx/Settings.ini");
	KillerHint_ICON_Start = atoi(iData);
	GetPrivateProfileStringA("SPECTATEUI", "KillerHint_ICON_SpaceBetween", "26", iData, sizeof(iData), "cstrike/gfx/Settings.ini");
	KillerHint_ICON_SpaceBetween = atoi(iData);
	GetPrivateProfileStringA("SPECTATEUI", "KillerHint_Name_Start", "172", iData, sizeof(iData), "cstrike/gfx/Settings.ini");
	KillerHint_Name_Start = atoi(iData);
	GetPrivateProfileStringA("SPECTATEUI", "KillerHint_Name_SpaceBetween", "26", iData, sizeof(iData), "cstrike/gfx/Settings.ini");
	KillerHint_Name_SpaceBetween = atoi(iData); 
	currentY = ScreenHeight;
	autoDisappear = removePanel;
}
 

void CHudDeathBoard::Draw(float flTime)
{
	//if (g_bAlive == false) {
	//	char test[256]; 
	//	sprintf(test, "%s", g_PlayerInfoList[g_iUser2].name);

	//	g_FontOutLine.SetColor(0, 0, 0, 255);
	//	g_FontOutLine.SetWidth(15);
	//	g_FontOutLine.DrawString(UTF8ToUnicode(test), ScreenWidth / 2, ScreenHeight / 2, 1000, 1000);//26

	//	g_Font.SetColor(130, 164, 164, 255);
	//	g_Font.SetWidth(15);
	//	g_Font.DrawString(UTF8ToUnicode(test), ScreenWidth / 2, ScreenHeight / 2, 1000, 1000);//26
	//}



	if (autoDisappear != removePanel) {
		char vicName[129];
		sprintf(vicName, "Hide Round Info: BackSpace", maxY, currentY);
		GL_DrawTGA(g_Texture[SPECTATE_UNDER].iTexture, 255, 255, 255, 255, ScreenWidth - g_Texture[SPECTATE_MAIN].iWidth, ScreenHeight - 33, 1.0);

		g_FontOutLine.SetColor(0, 0, 0, 255);
		g_FontOutLine.SetWidth(12);
		g_FontOutLine.DrawString(UTF8ToUnicode(vicName), ScreenWidth - g_Texture[SPECTATE_MAIN].iWidth + 12, ScreenHeight - 15, 1000, 1000);//26

		g_Font.SetColor(130, 164, 164, 255);
		g_Font.SetWidth(12);
		g_Font.DrawString(UTF8ToUnicode(vicName), ScreenWidth - g_Texture[SPECTATE_MAIN].iWidth + 12, ScreenHeight - 15, 1000, 1000);//26
	}

	if (isBackSpacePress && autoDisappear != removePanel) {
		 
		if (autoDisappear == removePanel) isBackSpacePress = false;
		 
		if (currentY >= maxY) {
			if (currentY != ScreenHeight) currentY = currentY * (1.0 - BezierBlend(flTime - startTime, 3.0f));
			else currentY = ScreenHeight * (1.0 - BezierBlend(flTime - startTime, 3.0f));
			disappearTime = flTime + 4.0f; 
		} 

		if (flTime >= disappearTime) { 
			startTime = cl.time;
			if (autoDisappear == autoDis) {   
				autoDisappear = manual;
				isBackSpacePress = false;
			}  
		}

		char vicName[129];
		//sprintf(vicName, "Hide Round Info: BackSpace", maxY, currentY);
		 
		 
		/// <summary>
		/// Display UI and Text
		/// </summary>
		/// <param name="flTime"></param>
		GL_DrawTGA(g_Texture[SPECTATE_MAIN].iTexture, 255, 255, 255, 255, ScreenWidth - g_Texture[SPECTATE_MAIN].iWidth, currentY, 1.0);   

		//*****Displayer Killer that killed Victim
		if (strlen(szWpnNameKiller) > 2) {
			g_FontOutLine.SetColor(0, 0, 0, 255);
			g_FontOutLine.SetWidth(12);
			g_FontOutLine.DrawString(UTF8ToUnicode(g_PlayerInfoList[iKillerID].name), ScreenWidth - g_Texture[SPECTATE_MAIN].iWidth + 12, currentY + 55, 1000, 1000);
			g_FontOutLine.DrawString(GetWeaponNameFormat(szWpnNameKiller), ScreenWidth - 105 - (g_FontOutLine.GetLen(GetWeaponNameFormat(szWpnNameKiller)) / 2), currentY + 82, 1000, 1000);

			g_Font.SetColor(130, 164, 164, 255);
			g_Font.SetWidth(12);
			g_Font.DrawString(UTF8ToUnicode(g_PlayerInfoList[iKillerID].name), ScreenWidth - g_Texture[SPECTATE_MAIN].iWidth + 12, currentY + 55, 1000, 1000);
			g_Font.DrawString(GetWeaponNameFormat(szWpnNameKiller), ScreenWidth - 105 - (g_Font.GetLen(GetWeaponNameFormat(szWpnNameKiller)) / 2), currentY + 82, 1000, 1000);

			sprintf(vicName, "gfx\\vgui\\AMMOICON\\%s_line", szWpnNameKiller);
			GL_DrawTGA(g_Texture[Hud().m_TGA.FindTexture(vicName)].iTexture, 255, 255, 255, 255, ScreenWidth - g_Texture[SPECTATE_MAIN].iWidth - 55, currentY + 85, 1.0);
		} 
		// left ScreenWidth - g_Texture[SPECTATE_MAIN].iWidth + 12
		// Right ScreenWidth - (g_Texture[SPECTATE_MAIN].iWidth + 12 - 28) - (g_Font.GetLen(GetWeaponNameFormat(szWpnNameKiller)) / 2)
		// 
		// 
		// 
		//*****Show Infomation take damage from Victim 
		 
		g_FontOutLine.SetColor(0, 0, 0, 255);
		g_FontOutLine.SetWidth(12);
		for (int i = 0; i < g_DeathBoardMSG.size(); i++) {
			sprintf(vicName, "%s", g_PlayerInfoList[g_DeathBoardMSG[i].victimID].name);

			if (g_DeathBoardMSG[i].victimDamage < 0) g_Font.SetColor(243, 166, 28, 255);
			else g_Font.SetColor(255, 255, 255, 255);

			g_FontOutLine.DrawString(UTF8ToUnicode(vicName), ScreenWidth - g_Texture[SPECTATE_MAIN].iWidth + 12, currentY + 175 + (i * 26), 1000, 1000);//26
			g_Font.DrawString(UTF8ToUnicode(vicName), ScreenWidth - g_Texture[SPECTATE_MAIN].iWidth + 12, currentY + 175 + (i * 26), 1000, 1000);//26
			if (g_DeathBoardMSG[i].victimDamage < 0) { 
				GL_DrawTGA(g_Texture[(g_DeathBoardMSG[i].victimDamage == -30) ? SPECTATE_HITNODE_30 : (g_DeathBoardMSG[i].victimDamage == -20) ? SPECTATE_HITNODE_20 : SPECTATE_HITNODE_10].iTexture, 255, 255, 255, 255, ScreenWidth - 30, currentY + 157 + (i * 26), 1.0);//26
			} else {
				sprintf(vicName, "%i", g_DeathBoardMSG[i].victimDamage);
				g_FontOutLine.DrawString(UTF8ToUnicode(vicName), ScreenWidth - 28, currentY + 175 + (i * 26), 1000, 1000);//26 
				g_Font.DrawString(UTF8ToUnicode(vicName), ScreenWidth - 28, currentY + 175 + (i * 26), 1000, 1000);//26  
			}
			 
		}
	}
	else if (!isBackSpacePress && autoDisappear != removePanel) {
		if (currentY == ScreenHeight) return; 
		char vicName[128];

		currentY = currentY * (1.0 + BezierBlend(flTime - startTime, 0.8f)); 
		GL_DrawTGA(g_Texture[SPECTATE_MAIN].iTexture, 255, 255, 255, 255, ScreenWidth - g_Texture[SPECTATE_MAIN].iWidth, currentY, 1.0);
		 

		//*****Displayer Killer that killed Victim
		if (strlen(szWpnNameKiller) > 2) {
			g_FontOutLine.SetColor(0, 0, 0, 255);
			g_FontOutLine.SetWidth(12);
			g_FontOutLine.DrawString(UTF8ToUnicode(g_PlayerInfoList[iKillerID].name), ScreenWidth - g_Texture[SPECTATE_MAIN].iWidth + 12, currentY + 55, 1000, 1000);
			g_FontOutLine.DrawString(GetWeaponNameFormat(szWpnNameKiller), ScreenWidth - 105 - (g_FontOutLine.GetLen(GetWeaponNameFormat(szWpnNameKiller)) / 2), currentY + 82, 1000, 1000);
			
			g_Font.SetColor(130, 164, 164, 255);
			g_Font.SetWidth(12);
			g_Font.DrawString(UTF8ToUnicode(g_PlayerInfoList[iKillerID].name), ScreenWidth - g_Texture[SPECTATE_MAIN].iWidth + 12, currentY + 55, 1000, 1000);
			g_Font.DrawString(GetWeaponNameFormat(szWpnNameKiller), ScreenWidth - 105 - (g_Font.GetLen(GetWeaponNameFormat(szWpnNameKiller)) / 2), currentY + 82, 1000, 1000);

			sprintf(vicName, "gfx\\vgui\\AMMOICON\\%s_line", szWpnNameKiller);
			GL_DrawTGA(g_Texture[Hud().m_TGA.FindTexture(vicName)].iTexture, 255, 255, 255, 255, ScreenWidth - g_Texture[SPECTATE_MAIN].iWidth - 55, currentY + 85, 1.0);
		} 
		// left ScreenWidth - g_Texture[SPECTATE_MAIN].iWidth + 12
		// Right ScreenWidth - (g_Texture[SPECTATE_MAIN].iWidth + 12 - 28) - (g_Font.GetLen(GetWeaponNameFormat(szWpnNameKiller)) / 2)
		// 
		// 
		// 
		//*****Show Infomation take damage from Victim 
		/////////////Show Infomation Victim 
		g_Font.SetWidth(12);
		g_FontOutLine.SetWidth(12);
		for (int i = 0; i < g_DeathBoardMSG.size(); i++) {
			sprintf(vicName, "%s", g_PlayerInfoList[g_DeathBoardMSG[i].victimID].name);

			if (g_DeathBoardMSG[i].victimDamage < 0) g_Font.SetColor(243, 166, 28, 255);
			else g_Font.SetColor(255, 255, 255, 255);

			g_FontOutLine.DrawString(UTF8ToUnicode(vicName), ScreenWidth - g_Texture[SPECTATE_MAIN].iWidth + 12, currentY + 175 + (i * 26), 1000, 1000);//26
			g_Font.DrawString(UTF8ToUnicode(vicName), ScreenWidth - g_Texture[SPECTATE_MAIN].iWidth + 12, currentY + 175 + (i * 26), 1000, 1000);//26
			if (g_DeathBoardMSG[i].victimDamage < 0) {
				GL_DrawTGA(g_Texture[(g_DeathBoardMSG[i].victimDamage == -30) ? SPECTATE_HITNODE_30 : (g_DeathBoardMSG[i].victimDamage == -20) ? SPECTATE_HITNODE_20 : SPECTATE_HITNODE_10].iTexture, 255, 255, 255, 255, ScreenWidth - 30, currentY + 157 + (i * 26), 1.0);//26
			} else {
				sprintf(vicName, "%i", g_DeathBoardMSG[i].victimDamage);
				g_FontOutLine.DrawString(UTF8ToUnicode(vicName), ScreenWidth - 28, currentY + 175 + (i * 26), 1000, 1000);//26 
				g_Font.DrawString(UTF8ToUnicode(vicName), ScreenWidth - 28, currentY + 175 + (i * 26), 1000, 1000);//26 
			}

		}


		if (currentY > ScreenHeight) { 
			currentY = ScreenHeight;  
		}
	}
}

float CHudDeathBoard::BezierBlend(float t, float duration)
{
	//if (t <= 0.5) t = 2 * t * t * t;
	//else 2 * t * (1 - t) + 0.5;
	//return 1.0 - t;
	//return t * t * (3.0f - 2.0f * t);
	return t * t * (duration * t);
}
