// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------

// ===== Project includes =====
#include "detour_cl_create_new_commands.h"


// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
Detour_CL_CreateNewCommands::Detour_CL_CreateNewCommands( Detour_CL_CreateNewCommands_Callback* callback ) : 
	Detour_Exception_HardwareBreakpoint( callback->GetGame( )->GetMainThreadID( ), callback->GetGame( )->GetAddress_CL_CreateNewCommands( ), 1 )
{
	callback_ = callback;
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
Detour_CL_CreateNewCommands::~Detour_CL_CreateNewCommands( )
{

}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
void Detour_CL_CreateNewCommands::OnException( CONTEXT* context )
{
	// 
	if ( callback_->OnPre_CL_CreateNewCommands( ) )
	{
		DWORD call_address = address_trampoline_;
		__asm
		{
			call	[ call_address ];
		}

		callback_->OnPost_CL_CreateNewCommands( );
	}

	// Jump back to where we came from.
	context->Eip = *( int* )( context->Esp );
	context->Esp += 4;
}

