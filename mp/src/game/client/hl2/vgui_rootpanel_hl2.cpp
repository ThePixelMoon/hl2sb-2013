//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//
#include "cbase.h"
#include "vgui_int.h"
#include "ienginevgui.h"
#ifdef LUA_SDK
#include "vgui/IVgui.h"
#include "vgui_rootpanel_hl2.h"
#include "clientmode_shared.h"
#endif

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#ifdef LUA_SDK
C_ScriptedBaseGameUIPanel *g_pScriptedBaseGameUIPanel = NULL;


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void VGUI_CreateGameUIRootPanel( void )
{
	// @ThePixelMoon: https://github.com/ThePixelMoon/OpenMod/commit/a19a0ba86d634402690f334b9666e18b1aa23c2b
	//				  Weird, this fix was the only thing to make this whole thing work. Maybe i was the only one
	//				  to experience that bug? Dunno.
	g_pScriptedBaseGameUIPanel = new C_ScriptedBaseGameUIPanel( enginevgui->GetPanel( PANEL_ROOT ) );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void VGUI_DestroyGameUIRootPanel( void )
{
	delete g_pScriptedBaseGameUIPanel;
	g_pScriptedBaseGameUIPanel = NULL;
}
#endif

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void VGUI_CreateClientDLLRootPanel( void )
{
	// Just using PANEL_ROOT in HL2 right now
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void VGUI_DestroyClientDLLRootPanel( void )
{
}

//-----------------------------------------------------------------------------
// Purpose: Game specific root panel
// Output : vgui::Panel
//-----------------------------------------------------------------------------
vgui::VPANEL VGui_GetClientDLLRootPanel( void )
{
	vgui::VPANEL root = enginevgui->GetPanel( PANEL_CLIENTDLL );
	return root;
}

#ifdef LUA_SDK
vgui::Panel *VGui_GetGameUIPanel( void )
{
	return g_pScriptedBaseGameUIPanel;
}

//-----------------------------------------------------------------------------
// Purpose: Game specific root panel
// Output : vgui::Panel
//-----------------------------------------------------------------------------
vgui::Panel *VGui_GetClientLuaRootPanel( void )
{
	ClientModeShared *mode = ( ClientModeShared * )GetClientModeNormal();
	vgui::Panel *pRoot = mode->m_pClientLuaPanel;
	return pRoot;
}

//-----------------------------------------------------------------------------
// C_ScriptedBaseGameUIPanel implementation.
//-----------------------------------------------------------------------------
C_ScriptedBaseGameUIPanel::C_ScriptedBaseGameUIPanel( vgui::VPANEL parent )
	: BaseClass( NULL, "ScriptedBaseGameUIPanel" )
{
	SetParent( parent );
	SetPaintEnabled( false );
	SetPaintBorderEnabled( false );
	SetPaintBackgroundEnabled( false );

	// This panel does post child painting
	SetPostChildPaintEnabled( true );

	// Make it screen sized
	SetBounds( 0, 0, ScreenWidth(), ScreenHeight() );

	// Ask for OnTick messages
	vgui::ivgui()->AddTickSignal( GetVPanel() );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
C_ScriptedBaseGameUIPanel::~C_ScriptedBaseGameUIPanel( void )
{
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void C_ScriptedBaseGameUIPanel::PostChildPaint()
{
	BaseClass::PostChildPaint();

	// Draw all panel effects
	RenderPanelEffects();
}

//-----------------------------------------------------------------------------
// Purpose: For each panel effect, check if it wants to draw and draw it on
//  this panel/surface if so
//-----------------------------------------------------------------------------
void C_ScriptedBaseGameUIPanel::RenderPanelEffects( void )
{
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void C_ScriptedBaseGameUIPanel::OnTick( void )
{
}

//-----------------------------------------------------------------------------
// Purpose: Reset effects on level load/shutdown
//-----------------------------------------------------------------------------
void C_ScriptedBaseGameUIPanel::LevelInit( void )
{
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void C_ScriptedBaseGameUIPanel::LevelShutdown( void )
{
}
#endif

