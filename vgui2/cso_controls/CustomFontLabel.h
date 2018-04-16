#pragma once

#include "vgui_controls/Label.h"
#include "KeyValues.h"
#include "VGUI/IScheme.h"

using namespace vgui;

class CustomFontLabel : public vgui::Label
{
	using BaseClass = vgui::Label;
public:
	CustomFontLabel(Panel *parent, const char *panelName, const char *text, const char *font) : BaseClass(parent, panelName, text)
	{
		m_fontOverrideName = NULL;
		SetCustomFont(font);
	}
	CustomFontLabel(Panel *parent, const char *panelName, const wchar_t *text, const char *font) : BaseClass(parent, panelName, text)
	{
		m_fontOverrideName = NULL;
		SetCustomFont(font);
	}
	~CustomFontLabel()
	{
		delete[] m_fontOverrideName;
	}

	virtual void ApplySchemeSettings(vgui::IScheme *pScheme) override
	{
		BaseClass::ApplySchemeSettings(pScheme);
		SetFont(pScheme->GetFont(m_fontOverrideName, IsProportional()));
	}

	char *m_fontOverrideName;
	void SetCustomFont(const char *font)
	{
		delete[] m_fontOverrideName;
		m_fontOverrideName = new char[strlen(font) + 1];
		strcpy(m_fontOverrideName, font);
		ApplySchemeSettings(vgui::scheme()->GetIScheme(GetScheme()));
	}
};