// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------

// ===== Project includes =====
#include "detour.h"


// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
Detour::Detour( DWORD address, int size )
{
	address_detour_ = address;
	address_trampoline_ = CreateTrampoline( address, size );
	detour_size_ = size;
	is_installed_ = false;
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
Detour::~Detour( )
{
	Remove( );
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
DWORD Detour::CreateTrampoline( DWORD address, int size )
{
	// Allocate space for the trampoline.
	BYTE* codecave = ( BYTE* )VirtualAlloc( nullptr, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE );

	// Make sure space allocation succeeded.
	if ( codecave == nullptr )
		return 0;

	// Calculate jump distance
	DWORD opcode_distance = ( DWORD )address - ( DWORD )codecave - 5;

	// Copy original codecave into codecave.
	memcpy( codecave, ( void* )address, size );

	// Write the jump opcode.
	codecave[ size ] = 0xE9;

	// Write the jump distance.
	memcpy( &codecave[ size + 1 ], &opcode_distance, 4 );

	// Return result-
	return ( DWORD )codecave;
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
bool Detour::WriteData( DWORD address, BYTE* data, int size )
{
	// Needed by VirtualProtect.
	DWORD old_protection = 0;

	// Make sure writing memory is permitted.
	if ( VirtualProtect( ( void* )address, size, PAGE_READWRITE, &old_protection ) == 0 )
		return false;

	// Write Byte-After-Byte.
	for ( int i = 0; i < size; i++ )
		*( BYTE* )( address + i ) = data[ i ];

	// Restore old protection.
	if ( VirtualProtect( ( void* )address, size, old_protection, &old_protection ) == 0 )
		return false;

	// 
	return true;
}