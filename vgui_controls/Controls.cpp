#include "metahook.h"
#include "plugins.h"

#include "Controls.h"
#include <locale.h>

vgui::IInput *g_pVGuiInput;
vgui::ISchemeManager *g_pVGuiSchemeManager;
vgui::ISurface *g_pVGuiSurface;
vgui::ISystem *g_pVGuiSystem;
vgui::IVGui *g_pVGui;
vgui::IPanel *g_pVGuiPanel;
vgui::ILocalize *g_pVGuiLocalize;

//IFileSystem *g_pFullFileSystem;
IFileSystem *g_pFullFileSystem;
IKeyValuesSystem *g_pKeyValuesSystem;

void KeyValuesSystem_InstallHook(void);

namespace vgui
{

static char g_szControlsModuleName[256];

bool VGui_InitInterfacesList(const char *moduleName, CreateInterfaceFn *factoryList, int numFactories)
{
	strncpy(g_szControlsModuleName, moduleName, sizeof(g_szControlsModuleName));
	g_szControlsModuleName[sizeof(g_szControlsModuleName) - 1] = 0;

	setlocale(LC_CTYPE, "");
	setlocale(LC_TIME, "");
	setlocale(LC_COLLATE, "");
	setlocale(LC_MONETARY, "");

	//g_pFullFileSystem = (IFileSystem *)factoryList[2](FILESYSTEM_INTERFACE_VERSION, NULL);
	g_pKeyValuesSystem = (IKeyValuesSystem *)factoryList[1](KEYVALUES_INTERFACE_VERSION, NULL);

	g_pVGuiInput = (IInput *)factoryList[1](VGUI_INPUT_INTERFACE_VERSION, NULL);
	g_pVGuiSchemeManager = (ISchemeManager *)factoryList[1](VGUI_SCHEME_INTERFACE_VERSION, NULL);
	//g_pVGuiSurface = (ISurface *)factoryList[0](VGUI_SURFACE_INTERFACE_VERSION, NULL);
	g_pVGuiSystem = (ISystem *)factoryList[1](VGUI_SYSTEM_INTERFACE_VERSION, NULL);
	g_pVGui = (IVGui *)factoryList[1](VGUI_IVGUI_INTERFACE_VERSION, NULL);
	g_pVGuiPanel = (IPanel *)factoryList[1](VGUI_PANEL_INTERFACE_VERSION, NULL);
	//g_pVGuiLocalize = (ILocalize *)factoryList[1](VGUI_LOCALIZE_INTERFACE_VERSION, NULL);

	if (!g_pFullFileSystem || !g_pKeyValuesSystem || !g_pVGuiInput || !g_pVGuiSchemeManager || 
		!g_pVGuiSurface || !g_pVGuiSystem || !g_pVGui || !g_pVGuiPanel || !g_pVGuiLocalize)
	{
		Warning("vgui_controls is missing a required interface!\n");
		return false;
	}

	KeyValuesSystem_InstallHook();

	return true;
}

const char *GetControlsModuleName(void)
{
	return g_szControlsModuleName;
}

}