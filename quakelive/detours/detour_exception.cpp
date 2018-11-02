// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------

// ===== Project includes =====
#include "detour_exception.h"

// ===== Define externs / statics =====
void*								Detour_Exception::vectored_exception_handler_handle = nullptr;
std::vector< Detour_Exception* >	Detour_Exception::detour_exception_list;


// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
Detour_Exception::Detour_Exception( DWORD address, int size ) :
	Detour( address, size )
{
	Detour_Exception::detour_exception_list.push_back( this );
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
Detour_Exception::~Detour_Exception( )
{
	auto it_this = std::find( Detour_Exception::detour_exception_list.begin( ), Detour_Exception::detour_exception_list.end( ), this );

	if ( it_this != Detour_Exception::detour_exception_list.end( ) )
		Detour_Exception::detour_exception_list.erase( it_this );
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
bool Detour_Exception::EnableExceptionDetours( )
{
	if ( Detour_Exception::vectored_exception_handler_handle != nullptr )
		return false;

	Detour_Exception::vectored_exception_handler_handle = AddVectoredExceptionHandler( TRUE, Detour_Exception::VectoredExceptionHandler );

	return Detour_Exception::vectored_exception_handler_handle != nullptr;
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
bool Detour_Exception::DisableExceptionDetours( )
{
	if ( Detour_Exception::vectored_exception_handler_handle == nullptr )
		return false;

	return RemoveVectoredExceptionHandler( Detour_Exception::vectored_exception_handler_handle ) != 0;
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
long __stdcall Detour_Exception::VectoredExceptionHandler( EXCEPTION_POINTERS* exception_pointers )
{
	bool detour_found = false;
	if ( exception_pointers->ExceptionRecord->ExceptionCode == EXCEPTION_SINGLE_STEP )
	{
		for ( auto it = Detour_Exception::detour_exception_list.begin( ); it != Detour_Exception::detour_exception_list.end( ); it++ )
		{
			if ( ( *it )->is_installed_ && ( *it )->address_detour_ == ( DWORD )exception_pointers->ExceptionRecord->ExceptionAddress )
			{
				( *it )->OnException( exception_pointers->ContextRecord );

				detour_found = true;
			}
		}
	}

	if ( detour_found )
		return EXCEPTION_CONTINUE_EXECUTION;
	else
		return EXCEPTION_CONTINUE_SEARCH;
}