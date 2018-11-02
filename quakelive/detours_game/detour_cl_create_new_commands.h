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
class Detour_CL_CreateNewCommands_Callback
{
public:

	virtual bool		OnPre_CL_CreateNewCommands( ) = 0;
	virtual void		OnPost_CL_CreateNewCommands( ) = 0;

	virtual Game*		GetGame( ) = 0;
};

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
class Detour_CL_CreateNewCommands : public Detour_Exception_HardwareBreakpoint
{
public:

	// Constructor & Destructor
	Detour_CL_CreateNewCommands( Detour_CL_CreateNewCommands_Callback* callback );
	~Detour_CL_CreateNewCommands( );

protected:

	// This is invoked by the exception handler.
	virtual void		OnException( CONTEXT* context );

private:

	// 
	Detour_CL_CreateNewCommands_Callback*	callback_;
};

