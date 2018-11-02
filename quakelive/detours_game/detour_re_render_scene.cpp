// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------

// ===== Project includes =====
#include "detour_re_render_scene.h"


// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
Detour_RE_RenderScene::Detour_RE_RenderScene( Detour_RE_RenderScene_Callback* callback ) : 
	Detour_Exception_HardwareBreakpoint( callback->GetGame( )->GetMainThreadID( ), callback->GetGame( )->GetAddress_RE_RenderScene( ), 1 )
{
	callback_ = callback;
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
Detour_RE_RenderScene::~Detour_RE_RenderScene( )
{

}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
void Detour_RE_RenderScene::OnException( CONTEXT* context )
{
	// Retrieve parameters.
	refdef_t* fd = *( refdef_t** )( context->Esp + 4 );

	// 
	if ( callback_->OnPre_RE_RenderScene( fd ) )
	{
		DWORD call_address = address_trampoline_;
		__asm
		{
			push	[ fd ];
			call	[ call_address ];
			add		esp, 4;
		}

		callback_->OnPost_RE_RenderScene( fd );
	}

	// Jump back to where we came from.
	context->Eip = *( int* )( context->Esp );
	context->Esp += 4;
}

