
#include "hud.h"
#include "DrawTargaImage.h"
#include "respawnbar.h"

#include "Client/HUD/DrawTGA.h"

static CHudRespawnBar g_HudRespawnBar;
CHudRespawnBar &HudRespawnBar()
{
	return g_HudRespawnBar;
}

void CHudRespawnBar::Init(void)
{
	m_iFlags |= HUD_ACTIVE;
}

void CHudRespawnBar::VidInit(void)
{
	m_bOn = FALSE;
	//m_iBG = Hud().m_TGA.FindTexture("resource\\hud\\other\\respawn_bg");
	m_iBG = Hud().m_TGA.FindTexture("gfx\\charSystem\\GAUGE\\RESPAWNBOX");
	m_BAR = Hud().m_TGA.FindTexture("gfx\\charSystem\\GAUGE\\respawn_BAR");
	m_GAUGEEFFECT = Hud().m_TGA.FindTexture("gfx\\charSystem\\GAUGE\\respawn_GAUGEEFFECT"); 
}

void CHudRespawnBar::Draw(float time)
{
	if(!m_bOn) return;
	if(m_flShowTime<=time) return;

	//Calculate Position Wide=432,High=72
	int iStartX,iStartY; 
	iStartX = (int)(ScreenWidth / 2) - g_Texture[m_iBG].iWidth / 2 + 64;
	iStartY = (int)ScreenHeight * 0.7;

	GL_DrawTGA(g_Texture[m_iBG].iTexture,255,255,255,255,iStartX,iStartY,1.0);
	
	const int ibarW = 370;
	GL_DrawTGACustom(g_Texture[m_BAR].iTexture, iStartX+4, iStartY+4,ibarW * (time - m_flStartTime) / (m_flShowTime - m_flStartTime),14,0.0,255,255,255,255);
	GL_DrawTGA(g_Texture[m_GAUGEEFFECT].iTexture, 255, 255, 255, 255, (iStartX - g_Texture[m_GAUGEEFFECT].iWidth + 8) + (ibarW * (time - m_flStartTime) / (m_flShowTime - m_flStartTime)), iStartY, 1.0);
	 
}
