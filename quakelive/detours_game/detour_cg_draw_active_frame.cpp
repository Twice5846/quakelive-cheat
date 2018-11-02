// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------

// ===== Project includes =====
#include "detour_cg_draw_active_frame.h"


// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
Detour_CG_DrawActiveFrame::Detour_CG_DrawActiveFrame( Detour_CG_DrawActiveFrame_Callback* callback ) : 
	Detour_Exception_HardwareBreakpoint( callback->GetGame( )->GetMainThreadID( ), callback->GetGame( )->GetAddress_CG_DrawActiveFrame( ), 1 )
{
	callback_ = callback;
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
Detour_CG_DrawActiveFrame::~Detour_CG_DrawActiveFrame( )
{

}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
void Detour_CG_DrawActiveFrame::OnException( CONTEXT* context )
{
	// Retrieve parameters.
	int serverTime = *( int* )( context->Esp + 4 );
	int stereoView = *( int* )( context->Esp + 8 );
	int demoPlayback = *( int* )( context->Esp + 12 );

	// 
	if ( callback_->OnPre_CG_DrawActiveFrame( serverTime, stereoView, demoPlayback ) )
	{
		DWORD call_address = address_trampoline_;
		__asm
		{
			push	[ demoPlayback ];
			push	[ stereoView ];
			push	[ serverTime ];
			call	[ call_address ];
			add		esp, 12;
		}

		callback_->OnPost_CG_DrawActiveFrame( serverTime, stereoView, demoPlayback );
	}

	// Jump back to where we came from.
	context->Eip = *( int* )( context->Esp );
	context->Esp += 4;
}

