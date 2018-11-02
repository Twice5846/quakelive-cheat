// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------

// ===== Project includes =====
#include "detour_exception_hardwarebreakpoint.h"


// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
Detour_Exception_HardwareBreakpoint::Detour_Exception_HardwareBreakpoint( DWORD thread_id, DWORD address, int size ) :
	Detour_Exception( address, size )
{
	breakpoint_thread_id_ = thread_id;
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
Detour_Exception_HardwareBreakpoint::~Detour_Exception_HardwareBreakpoint( )
{

}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
bool Detour_Exception_HardwareBreakpoint::Install( )
{
	// Don't install twice.
	if ( is_installed_ )
		return false;

	// Try to open thread. Its ID was originally passed into the constructor.
	HANDLE thread = OpenThread( THREAD_ALL_ACCESS, FALSE, breakpoint_thread_id_ );
	if ( thread == nullptr )
		return false;

	// Try to grab current context of thread.
	CONTEXT context = { CONTEXT_DEBUG_REGISTERS };
	if ( GetThreadContext( thread, &context ) == 0 )
		return false;

	// This will try to enable the next free hardware breakpoint.
	if ( !( context.Dr7 & 0x00000001 ) )
	{
		context.Dr0 = address_detour_;
		context.Dr7 |= 0x00000001;
	}
	else if ( !( context.Dr7 & 0x00000004 ) )
	{
		context.Dr1 = address_detour_;
		context.Dr7 |= 0x00000004;
	}
	else if ( !( context.Dr7 & 0x00000010 ) )
	{
		context.Dr2 = address_detour_;
		context.Dr7 |= 0x00000010;
	}
	else if ( !( context.Dr7 & 0x00000040 ) )
	{
		context.Dr3 = address_detour_;
		context.Dr7 |= 0x00000040;
	}
	else
	{
		return false;
	}
	
	// Try to set the modified thread context.
	if ( !SetThreadContext( thread, &context ) )
		return false;

	// Close thread.
	if ( !CloseHandle( thread ) )
		return false;

	// Mark this detour as uninstalled.
	is_installed_ = true;

	// Everything's okay.
	return true;
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
bool Detour_Exception_HardwareBreakpoint::Remove( )
{
	// Only installed detours can be removed.
	if ( !is_installed_ )
		return false;

	// Try to open thread. Its ID was originally passed into the constructor.
	HANDLE thread = OpenThread( THREAD_ALL_ACCESS, FALSE, breakpoint_thread_id_ );
	if ( thread == nullptr )
		return false;

	// Try to grab current context of thread.
	CONTEXT context = { CONTEXT_DEBUG_REGISTERS };
	if ( GetThreadContext( thread, &context ) == 0 )
		return false;

	// This will try to enable the next free hardware breakpoint.
	if ( context.Dr7 & 0x00000001 && context.Dr0 == address_detour_ )
	{
		context.Dr0 = 0;
		context.Dr7 &= ~0x00000001;
	}
	else if ( context.Dr7 & 0x00000004 && context.Dr1 == address_detour_ )
	{
		context.Dr1 = 0;
		context.Dr7 &= ~0x00000004;
	}
	else if ( context.Dr7 & 0x00000010 && context.Dr2 == address_detour_ )
	{
		context.Dr2 = 0;
		context.Dr7 &= ~0x00000010;
	}
	else if ( context.Dr7 & 0x00000040 && context.Dr3 == address_detour_ )
	{
		context.Dr3 = 0;
		context.Dr7 &= ~0x00000040;
	}
	else
	{
		return false;
	}

	// Try to set the modified thread context.
	if ( !SetThreadContext( thread, &context ) )
		return false;

	// Close thread.
	if ( !CloseHandle( thread ) )
		return false;

	// Mark this detour as uninstalled.
	is_installed_ = false;

	// Everything's okay.
	return true;
}
