#include "metahook.h"
#include "bte_const.h"
#include "exportfuncs.h"
#include "hud.h"
#include "display.h"
#include "parsemsg.h"
#include "configs.h"
#include "util.h"

#include "weapons.h"

#include "health.h"

#include "Encode.h"
#include "Fonts.h"
#include "PlayerClassManager.h" 
#include "MVPBoard.h"
#include <HUD/DrawTga.h>
#include "DrawTargaImage.h"

#include <metahook.h> 
#include <gl/gl.h>
#include "common.h"

#include<string>
#include<stdio.h>
#include<stdlib.h>

static CHudHealth g_HudHealth;

CHudHealth& HudHealth()
{
	return g_HudHealth;
}

//DECLARE_MESSAGE(m_Health, HealthExtra);

void CHudHealth::Init(void)
{
	m_iFlags |= HUD_ACTIVE; 

	//HOOK_MESSAGE(HealthExtra);
	gEngfuncs.pfnHookUserMsg("HealthExtra", [](const char* pszName, int iSize, void* pbuf) { return HudHealth().MsgFunc_HealthExtra(pszName, iSize, pbuf); });
}

void CHudHealth::VidInit(void)
{
	m_iHealth = 100;
	m_flHealthFade = 0;
	m_flArmorFade = 0;
	//m_iArmorFlags = DHN_3DIGITS | DHN_DRAWZERO;

	//m_iHealthIcon = Hud().GetSpriteIndex("cross");
	//m_iHealthExtraIcon = Hud().GetSpriteIndex("crosstime");
	//m_iArmorIcon[0] = Hud().GetSpriteIndex("suit_empty");
	//m_iArmorFullIcon[0] = Hud().GetSpriteIndex("suit_full");
	//m_iArmorIcon[1] = Hud().GetSpriteIndex("suithelmet_empty");
	//m_iArmorFullIcon[1] = Hud().GetSpriteIndex("suithelmet_full");
}

void CHudHealth::Draw(float time)
{
	if (g_iMod == MOD_ZE || g_iMod == MOD_DR || !g_bAlive)
		return;

	DrawHealth(time);
	//DrawHealthExtra(time);
	//DrawArmor(time);
}

void CHudHealth::Think(void)
{
	if (gConfigs.bEnableNewHud)
	{
		if (m_iFlags & HUD_ACTIVE)
			m_iFlags &= ~HUD_ACTIVE;
	}
	else
	{
		if (!(m_iFlags & HUD_ACTIVE))
			m_iFlags |= HUD_ACTIVE;
	}
}

void CHudHealth::DrawHealth(float time)
{ 
	g_Font.SetColor(251, 201, 96, 255);
	g_Font.SetWidth(20);
	 
	//Display HP & AC tga image
	int HPAC[2] = { Hud().m_TGA.FindTexture("gfx\\charSystem\\SPECTATE_HP_MAIN"),Hud().m_TGA.FindTexture("gfx\\charSystem\\SPECTATE_AC_MAIN") };
	GL_DrawTGA(g_Texture[HPAC[1]].iTexture, 255, 255, 255, 255, ScreenWidth * 0.12, ScreenHeight - 63, 1.3);
	GL_DrawTGA(g_Texture[HPAC[0]].iTexture, 255, 255, 255, 255, ScreenWidth * 0.12, ScreenHeight - 33, 1.3);

	//Display Character Image
	char iF[5], iT[5], iName[MAXCHAR], cFline[40], cFbg[40], cEffect[40];
	GetPrivateProfileStringA(PlayerClassManager()[gEngfuncs.GetLocalPlayer()->index].model, "CoorFrom", "0", iF, sizeof(iF), "cstrike/gfx/char.ini");
	GetPrivateProfileStringA(PlayerClassManager()[gEngfuncs.GetLocalPlayer()->index].model, "CoorTo", "0", iT, sizeof(iT), "cstrike/gfx/char.ini");
	GetPrivateProfileStringA(PlayerClassManager()[gEngfuncs.GetLocalPlayer()->index].model, "Name", "0", iName, sizeof(iName), "cstrike/gfx/char.ini");
	 
	sprintf(cFbg, "gfx\\charSystem\\HP_FBG_%s", iName);
	sprintf(cEffect, "gfx\\charSystem\\HP_FBGEFFECT_%s", iName);
	sprintf(cFline, "gfx\\charSystem\\HP_FLINE_%s", iName);

	int r, g, b;
	if (HudHealth().m_iHealth >= 51) {
		r = 64, g = 140, b = 85; // green
	}
	else if (HudHealth().m_iHealth <= 50 && HudHealth().m_iHealth >= 21) {
		r = 205, g = 117, b = 32; //orgrange
	}
	else {
		r = 147, g = 33, b = 29; //red
	}
	GL_DrawTGA(g_Texture[Hud().m_TGA.FindTexture(cFbg)].iTexture, 69, 154, 98, 255, 5, ScreenHeight - 90, 1.0);
	GL_DrawTGACustom(g_Texture[Hud().m_TGA.FindTexture(cEffect)].iTexture, 5, ScreenHeight - 90, 128, 128, (atoi(iT) / 100.0f) - (float(HudHealth().m_iHealth) / 100) * ((atoi(iT) / 100.0f) - (atoi(iF) / 100.0f)), r, g, b);
	GL_DrawTGA(g_Texture[Hud().m_TGA.FindTexture(cFline)].iTexture, 255, 255, 255, 255, 5, ScreenHeight - 90, 1.0);

	//Display HP & AC
	char hp[10], ac[10];
	sprintf(hp, "%i", HudHealth().m_iHealth);
	sprintf(ac, "%i", HudHealth().m_iArmor);
	g_Font.DrawString(UTF8ToUnicode(ac), ScreenWidth / 6.5, ScreenHeight - 44, 1000, 1000);
	g_Font.DrawString(UTF8ToUnicode(hp), ScreenWidth / 6.5, ScreenHeight - 14, 1000, 1000);

}

void CHudHealth::DrawHealthExtra(float time)
{
	int r, g, b;
	int a = 0, x, y;

	if ((Hud().m_iHideHUDDisplay & HIDEWEAPON_HEALTH) || gEngfuncs.IsSpectateOnly())
		return;
	if (m_iHealthExtra <= 0)
		return;

	a = 255;

	UnpackRGB(r, g, b, RGB_LIGHTBLUE);

	ScaleColors(r, g, b, a);

	if (Hud().m_iWeaponBits & (1 << (WEAPON_VEST)))
	{
		int iCrossWidth = Hud().GetSpriteRect(m_iHealthExtraIcon).right - Hud().GetSpriteRect(m_iHealthExtraIcon).left;

		x = iCrossWidth / 2;
		y = ScreenHeight - Hud().m_iFontHeight * 3 + 7;

		gEngfuncs.pfnSPR_Set(Hud().GetSprite(m_iHealthExtraIcon), r, g, b);
		gEngfuncs.pfnSPR_DrawAdditive(0, x, y, &Hud().GetSpriteRect(m_iHealthExtraIcon));

		x = iCrossWidth + Hud().m_iFontWidth / 2;
		Hud().DrawHudNumber(x, y, m_iHealthFlags, m_iHealthExtra, r, g, b);
	}
}

void CHudHealth::DrawArmor(float time)
{
	/*if ((Hud().m_iHideHUDDisplay & (HIDEWEAPON_HEALTH | HIDEWEAPON_ALL)) || gEngfuncs.IsSpectateOnly())
		return;

	if (!(Hud().m_iWeaponBits & (1 << (WEAPON_VEST))))
		return;

	if (m_iArmorType >= sizeof m_iArmorIcon)
		return;

	int x, y;
	int r, g, b, a;

	if (m_flArmorFade)
	{
		m_flArmorFade -= (Hud().m_flTimeDelta * 20);

		if (m_flArmorFade <= 0)
		{
			a = MIN_ALPHA;
			m_flArmorFade = 0;
		}
		a = MIN_ALPHA + (m_flArmorFade / FADE_TIME) * 128;
	}
	else
		a = MIN_ALPHA;
	UnpackRGB(r, g, b, RGB_YELLOWISH);
	ScaleColors(r, g, b, a);
	int iCrossWidth = Hud().GetSpriteRect(m_iHealthIcon).right - Hud().GetSpriteRect(m_iHealthIcon).left;
	x = ScreenWidth / 5;
	y = ScreenHeight - Hud().m_iFontHeight - Hud().m_iFontHeight / 2;
	gEngfuncs.pfnSPR_Set(Hud().GetSprite(m_iArmorIcon[m_iArmorType]), r, g, b);
	gEngfuncs.pfnSPR_DrawAdditive(0, x, y, &Hud().GetSpriteRect(m_iArmorIcon[m_iArmorType]));
	wrect_t rcFullArmo = *&Hud().GetSpriteRect(m_iArmorFullIcon[m_iArmorType]);
	rcFullArmo.top += (rcFullArmo.bottom - rcFullArmo.top) * ((float)(100 - (min(100, m_iArmor))) * 0.01);
	if (rcFullArmo.bottom > rcFullArmo.top)
	{
		gEngfuncs.pfnSPR_Set(Hud().GetSprite(m_iArmorFullIcon[m_iArmorType]), r, g, b);
		gEngfuncs.pfnSPR_DrawAdditive(0, x, y + rcFullArmo.top - Hud().GetSpriteRect(m_iArmorFullIcon[m_iArmorType]).top, &rcFullArmo);
	}
	x += iCrossWidth;
	Hud().DrawHudNumber(x, y, m_iArmorFlags, m_iArmor, r, g, b);*/
}

int CHudHealth::MsgFunc_HealthExtra(const char* pszName, int iSize, void* pbuf)
{
	BEGIN_READ(pbuf, iSize);
	m_iHealthExtra = READ_SHORT();

	return 1;
}