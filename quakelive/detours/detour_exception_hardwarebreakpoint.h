// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------

#pragma once

// ===== Platform includes =====
#include <Windows.h>

// ===== Project includes =====
#include "detour_exception.h"


// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
class Detour_Exception_HardwareBreakpoint : public Detour_Exception
{
public:

	// 
	Detour_Exception_HardwareBreakpoint( DWORD thread_id, DWORD address, int size );
	~Detour_Exception_HardwareBreakpoint( );

	// Install this detour.
	virtual bool		Install( );

	// Remove this detour.
	virtual bool		Remove( );

protected:

	// This is invoked by the exception handler.
	virtual void		OnException( CONTEXT* context )		{ }

protected:

	// Which thread this breakpoint was created on.
	DWORD				breakpoint_thread_id_;
};
