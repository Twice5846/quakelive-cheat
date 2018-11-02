// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------

#pragma once

// ===== Platform includes =====
#include <Windows.h>


// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
class Detour
{
public:

	// Constructor & Destructor
	Detour( DWORD address, int size );
	~Detour( );

	// Install this detour.
	virtual bool		Install( )	{ return false; };

	// Remove this detour.
	virtual bool		Remove( )	{ return false; };

protected:

	// Takes [size] bytes from address and creates a codecave with it.
	DWORD				CreateTrampoline( DWORD address, int size );

	// Write [data] into [address].
	bool				WriteData( DWORD address, BYTE* data, int size );

	// Address on which this detour was created.
	DWORD				address_detour_;

	// Address that holds original code to continue program.
	DWORD				address_trampoline_;

	// How many opcodes where cut from address_detour_origin.
	int					detour_size_;

	// Is this detour already installed?
	bool				is_installed_;
};

