// ----------------------------------------------------------------------------
// Class to detour any address inside the game to another one.
// ----------------------------------------------------------------------------

#pragma once

// ===== Project includes =====
#include "../detours/detours.h"
#include "../game.h"


// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
class Detour_CG_DrawActiveFrame_Callback
{
public:

	virtual bool		OnPre_CG_DrawActiveFrame( int& serverTime, int& stereoView, int& demoPlayback ) = 0;
	virtual void		OnPost_CG_DrawActiveFrame( int& serverTime, int& stereoView, int& demoPlayback ) = 0;

	virtual Game*		GetGame( ) = 0;
};

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
class Detour_CG_DrawActiveFrame : public Detour_Exception_HardwareBreakpoint
{
public:

	// Constructor & Destructor
	Detour_CG_DrawActiveFrame( Detour_CG_DrawActiveFrame_Callback* callback );
	~Detour_CG_DrawActiveFrame( );

protected:

	// This is invoked by the exception handler.
	virtual void		OnException( CONTEXT* context );

private:

	// 
	Detour_CG_DrawActiveFrame_Callback*		callback_;
};

