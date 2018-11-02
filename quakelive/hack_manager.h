// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------

#pragma once

// ===== Project includes =====
#include "detours_game\detours_game.h"
#include "features\features.h"
#include "game.h"


// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
class HackManager : Detour_CG_DrawActiveFrame_Callback, Detour_CL_CreateNewCommands_Callback
{
public:

	HackManager( );
	~HackManager( );

	void							Startup( );
	void							Shutdown( );

	// Callbacks
	virtual bool					OnPre_CG_DrawActiveFrame( int& serverTime, int& stereoView, int& demoPlayback );
	virtual bool					OnPre_CL_CreateNewCommands( );
	virtual void					OnPost_CG_DrawActiveFrame( int& serverTime, int& stereoView, int& demoPlayback );
	virtual void					OnPost_CL_CreateNewCommands( );

	// Getter
	virtual Game*					GetGame( )	{ return game_; }

private:

	void							Install( );
	void							Uninstall( );

	bool							EnumerateThreadIDs( std::vector< DWORD >& thread_ids );
	DWORD							GetThreadEntryPoint( DWORD thread_id );
	DWORD							GetProcessEntryPoint( );
	DWORD							GetMainThreadID( );
	void							SuspendMainThread( );
	void							ResumeMainThread( );

	Game*							game_;
	Detour_CG_DrawActiveFrame*		detour_cg_draw_active_frame_;
	Detour_CL_CreateNewCommands*	detour_cl_create_new_commands_;

	Feature_Aimbot*					feature_aimbot_;
	Feature_ESP*					feature_esp_;
};

