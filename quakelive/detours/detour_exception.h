// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------

#pragma once

// ===== Platform includes =====
#include <Windows.h>
#include <vector>

// ===== Project includes =====
#include "detour.h"


// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
class Detour_Exception : public Detour
{
public:

	// Constructor & Destructor
	Detour_Exception( DWORD address, int size );
	~Detour_Exception( );

	// Install this detour.
	virtual bool		Install( )	{ return false; }

	// Remove this detour.
	virtual bool		Remove( )	{ return false; }

	// 
	static bool			EnableExceptionDetours( );

	// 
	static bool			DisableExceptionDetours( );

protected:

	// This is invoked by the exception handler.
	virtual void		OnException( CONTEXT* context )		{ }

private:

	// 
	static long __stdcall	VectoredExceptionHandler( EXCEPTION_POINTERS* exception_pointers );

	// Handle returned by AddVectoredExceptionHandler.
	static void*			vectored_exception_handler_handle;

	// 
	static std::vector< Detour_Exception* >	detour_exception_list;
};