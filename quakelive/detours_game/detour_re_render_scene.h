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
class Detour_RE_RenderScene_Callback
{
public:

	virtual bool		OnPre_RE_RenderScene( refdef_t*& fd ) = 0;
	virtual void		OnPost_RE_RenderScene( refdef_t*& fd ) = 0;

	virtual Game*		GetGame( ) = 0;
};

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
class Detour_RE_RenderScene : public Detour_Exception_HardwareBreakpoint
{
public:

	// Constructor & Destructor
	Detour_RE_RenderScene( Detour_RE_RenderScene_Callback* callback );
	~Detour_RE_RenderScene( );

protected:

	// This is invoked by the exception handler.
	virtual void		OnException( CONTEXT* context );

private:

	// 
	Detour_RE_RenderScene_Callback*	callback_;
};

