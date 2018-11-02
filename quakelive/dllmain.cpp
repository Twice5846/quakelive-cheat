// ----------------------------------------------------------------------------
// Define module entry point and basic injection/ejection logic.
// ----------------------------------------------------------------------------

// ===== Platform includes =====
#include <Windows.h>

// ===== Project includes =====
#include "hack_manager.h"

HackManager hack_manager;


// ----------------------------------------------------------------------------
// Called when DLL is loaded.
// ----------------------------------------------------------------------------
void OnInject( )
{
	hack_manager.Startup( );
}

// ----------------------------------------------------------------------------
// Called when DLL is unloaded.
// ----------------------------------------------------------------------------
void OnEject( )
{
	hack_manager.Shutdown( );
}

// ----------------------------------------------------------------------------
// Module entry point.
// ----------------------------------------------------------------------------
BOOL __stdcall DllMain( HMODULE module, DWORD reason, void* reserved )
{
	switch ( reason )
	{
		case DLL_PROCESS_ATTACH:
		{
			CreateThread( nullptr, 0, ( LPTHREAD_START_ROUTINE )OnInject, nullptr, 0, nullptr );

			break;
		}
		case DLL_PROCESS_DETACH:
		{
			OnEject( );

			break;
		}
	}

	return TRUE;
}

