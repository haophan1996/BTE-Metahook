#ifndef BASEPANEL_H
#define BASEPANEL_H

#ifdef _WIN32
#pragma once
#endif

#include "vgui_controls/Panel.h"
#include "vgui_controls/PHandle.h"
#include "vgui_controls/MenuItem.h"
#include "vgui_controls/messagedialog.h"
#include "KeyValues.h"
#include "UtlVector.h"
//#include "tier1/CommandBuffer.h"

#include "GL_BinkTexture.h"

class CGameMenu;
class CBackgroundMenuButton;
class CBinkPanel;
class CBackGroundPanel;
class CToolBar;

class CCSBTEEscPanel;
class CCSBTEWelcomeDialog;

class CFooterPanel : public vgui::EditablePanel
{
	DECLARE_CLASS_SIMPLE(CFooterPanel, vgui::EditablePanel);

public:
	CFooterPanel(Panel *parent, const char *panelName);
	virtual ~CFooterPanel(void);

public:
	virtual void ApplySchemeSettings(vgui::IScheme *pScheme);
	virtual void ApplySettings(KeyValues *pResourceData);
	virtual void Paint(void);
	virtual void PaintBackground(void);

public:
	void SetHelpNameAndReset(const char *pName);
	const char *GetHelpName(void);
	void AddButtonsFromMap(vgui::Frame *pMenu);
	void SetStandardDialogButtons(void);
	void AddNewButtonLabel(const char *text, const char *icon);
	void ShowButtonLabel(const char *name, bool show = true);
	void SetButtonText(const char *buttonName, const char *text);
	void ClearButtons(void);
	void SetButtonGap(int nButtonGap) { m_nButtonGap = nButtonGap; }
	void UseDefaultButtonGap(void) { m_nButtonGap = m_nButtonGapDefault; }

private:
	struct ButtonLabel_t
	{
		bool bVisible;
		char name[MAX_PATH];
		wchar_t text[MAX_PATH];
		wchar_t icon[2];
	};

private:
	CUtlVector<ButtonLabel_t *> m_ButtonLabels;
	vgui::Label *m_pSizingLabel;
	bool m_bPaintBackground;
	bool m_bCenterHorizontal;
	int m_ButtonPinRight;
	int m_nButtonGap;
	int m_nButtonGapDefault;
	int m_FooterTall;
	int m_ButtonOffsetFromTop;
	int m_ButtonSeparator;
	int m_TextAdjust;
	char m_szTextFont[64];
	char m_szButtonFont[64];
	char m_szFGColor[64];
	char m_szBGColor[64];
	vgui::HFont m_hButtonFont;
	vgui::HFont m_hTextFont;
	char *m_pHelpName;
};

class CMainMenuGameLogo : public vgui::EditablePanel
{
	DECLARE_CLASS_SIMPLE(CMainMenuGameLogo, vgui::EditablePanel);

public:
	CMainMenuGameLogo(vgui::Panel *parent, const char *name);

public:
	virtual void ApplySettings(KeyValues *inResourceData);
	virtual void ApplySchemeSettings(vgui::IScheme *pScheme);

public:
	int GetOffsetX(void) { return m_nOffsetX; }
	int GetOffsetY(void) { return m_nOffsetY; }

private:
	int m_nOffsetX;
	int m_nOffsetY;
};

class CCSOLLuckyItemPopupDialog;

class CGameMenuItem : public vgui::MenuItem
{
	DECLARE_CLASS_SIMPLE(CGameMenuItem, vgui::MenuItem);

public:
	CGameMenuItem(vgui::Menu *parent, const char *name);

public:
	virtual void ApplySchemeSettings(vgui::IScheme *pScheme);
	virtual Color GetButtonBgColor();
	virtual Color GetButtonFgColor();
	virtual void PaintBackground(void);
	virtual void OnCursorEntered();
	virtual void OnCursorExited();

public:
	void SetRightAlignedText(bool state);

private:
	bool m_bRightAligned;
};

class CBasePanel : public vgui::Panel
{
	DECLARE_CLASS_SIMPLE(CBasePanel, vgui::Panel);

public:
	CBasePanel(void);
	~CBasePanel(void);

public:
	void OnLevelLoadingStarted(const char *levelName);
	void OnLevelLoadingFinished(void);
	void RunFrame(void);
	void SetMenuItemBlinkingState(const char *itemName, bool state);

public:
	void OnWelcomeNewbie(void);
	void OnGameUIActivated(void);
	void OnOpenServerBrowser(void);
	void OnOpenCreateMultiplayerGameDialog(void);
	void OnOpenQuitConfirmationDialog(void);
	void OnOpenOptionsDialog(void);

	/*void OnOpenCSBTEAboutDialog(void);
	void OnOpenCSBTEUpdateDialog(void);
	void OnOpenCSBTEModeSelection(void);
	void OnOpenCSBTEMyWpnEditor(void);
	void OnOpenCSBTEWpnDataEditor(void);
	void OnOpenCSBTEZombieDNA(void);*/
	template<class T>
	T *CreateSubDialog(const char *name)
	{
		return new T(this, name);
	}
	template<class T>
	T *CreateSubDialog()
	{
		return new T(this, typeid(T).name() + 7); // "class CMiao" -> "Miao"
	}
	template<class T>
	vgui::DHANDLE<T> &GetSubPanelHandle()
	{
		static vgui::DHANDLE<T> x;
		return x;
	}
	template<class T, class...Args>
	void OnOpenSubDialog(const Args &...args)
	{
		auto &handle = GetSubPanelHandle<T>();
		if (!handle.Get())
		{
			handle = CreateSubDialog<T>(args...);
			PositionDialog(handle);
		}
		handle->SetVisible(true);
		handle->Activate();
	}


	void OnSizeChanged(int newWide, int newTall);
	void OnGameUIHidden(void);

public:
	KeyValues *GetConsoleControlSettings(void);
	void RunCloseAnimation(const char *animName);
	void RunAnimationWithCallback(vgui::Panel *parent, const char *animName, KeyValues *msgFunc);
	void PositionDialog(vgui::PHandle dlg);
	void PositionDialog(Panel *pdlg);
	void CloseBaseDialogs(void);
	bool IsMenuFading(void);
	bool IsInitialLoading(void);

protected:
	virtual void PaintBackground(void);
	virtual void ApplySchemeSettings(vgui::IScheme *pScheme);

private:
	enum EBackgroundState
	{
		BACKGROUND_INITIAL,
		BACKGROUND_LOADING,
		BACKGROUND_MAINMENU,
		BACKGROUND_LEVEL,
		BACKGROUND_DISCONNECTED,
		BACKGROUND_EXITING,
	};

	struct coord
	{
		int x;
		int y;
	};

	EBackgroundState m_eBackgroundState;

	struct bimage_t
	{
		int imageID;
		int x, y;
		int width, height;
		bool scaled;
	};
	CUtlVector<bimage_t> m_ImageID;
	int m_iBaseResX, m_iBaseResY;

public:
	void SetVisible(bool state);
	void SetBackgroundRenderState(EBackgroundState state);
	void UpdateBackgroundState(void);
	void SetMenuAlpha(int alpha);
	void CreateGameMenu(void);
	void CreateGameLogo(void);
	void CreateBinkPanel(void);
	void CreateBackGround(void);
	void CreateToolbar(void);
	void UpdateGameMenus(void);
	CGameMenu *RecursiveLoadGameMenu(KeyValues *datafile);
	void DrawBackgroundImage(void);

private:
	virtual void OnCommand(const char *command);
	virtual void PerformLayout(void);

private:
	MESSAGE_FUNC_INT(OnActivateModule, "ActivateModule", moduleIndex);

private:
	CMainMenuGameLogo *m_pGameLogo;
	CUtlVector<CBackgroundMenuButton *> m_pGameMenuButtons;
	CGameMenu *m_pGameMenu;
	int m_iGameMenuInset;
	CUtlVector<coord> m_iGameTitlePos;
	coord m_iGameMenuPos;

	vgui::DHANDLE<vgui::PropertyDialog> m_hOptionsDialog;
	vgui::DHANDLE<vgui::Frame> m_hCreateMultiplayerGameDialog;
	vgui::DHANDLE<vgui::QueryBox> m_hQuitQueryBox;
	vgui::DHANDLE<CCSBTEEscPanel>m_hCSBTEEscPanel;
	vgui::DHANDLE<CCSBTEWelcomeDialog> m_hCSBTEWelcomeDialog;
	/*vgui::DHANDLE<CCSBTEAboutDialog> m_hCSBTEAboutDialog;
	vgui::DHANDLE<CCSBTEUpdateDialog> m_hCSBTEUpdateDialog;
	vgui::DHANDLE<CCSBTEMyWpnEditor> m_hCSBTEMyWpnEditor;
	vgui::DHANDLE<CCSBTEWpnDataEditor> m_hCSBTEWpnDataEditor;
	vgui::DHANDLE<CCSBTEZombieDNA> m_hCSBTEZombieDNA;
	
	vgui::DHANDLE<CCSOLLuckyItemPopupDialog> m_hCSOLLuckyItemPopupDialog;*/
	
	vgui::AnimationController *m_pConsoleAnimationController;
	KeyValues *m_pConsoleControlSettings;
	int m_iLoadingImageID;
	bool m_bLevelLoading;
	bool m_bEverActivated;
	bool m_bFadingInMenus;
	bool m_bInitialLoading;
	float m_flFadeMenuStartTime;
	float m_flFadeMenuEndTime;
	bool m_bRenderingBackgroundTransition;
	float m_flTransitionStartTime;
	float m_flTransitionEndTime;
	bool m_bHaveDarkenedBackground;
	bool m_bHaveDarkenedTitleText;
	bool m_bForceTitleTextUpdate;
	float m_flFrameFadeInTime;
	Color m_BackdropColor;
	vgui::VPANEL m_pFocusPanel;
	vgui::VPANEL m_pFocusParent;
	int m_iToolBarSize;
	CToolBar *m_pToolBar;

public:
	//CGL_BinkTexture *m_pBinkTexture;


public:
	//CCSBTEGameMenu *m_pGameMenu;

private:
	CPanelAnimationVar(float, m_flBackgroundFillAlpha, "m_flBackgroundFillAlpha", "0");

private:
	MESSAGE_FUNC_CHARPTR(RunEngineCommand, "RunEngineCommand", command);
	MESSAGE_FUNC_CHARPTR(RunMenuCommand, "RunMenuCommand", command);
	MESSAGE_FUNC(FinishDialogClose, "FinishDialogClose");

public:
	friend class CCSBTEEscPanel;
};

extern CBasePanel *BasePanel(void);
#endif
