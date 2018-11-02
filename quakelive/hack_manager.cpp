// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------

// ===== Platform includes =====
#include <Windows.h>
#include <tlhelp32.h>
#include <Psapi.h>
#include <vector>

// ===== Project includes =====
#include "hack_manager.h"


// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
HackManager::HackManager( )
{
	game_ = nullptr;
	detour_cg_draw_active_frame_ = nullptr;
	detour_cl_create_new_commands_ = nullptr;
	feature_aimbot_ = nullptr;
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
HackManager::~HackManager( )
{

}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
bool HackManager::OnPre_CG_DrawActiveFrame( int& serverTime, int& stereoView, int& demoPlayback )
{
	return true;
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
bool HackManager::OnPre_CL_CreateNewCommands( )
{
	return true;
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
void HackManager::OnPost_CG_DrawActiveFrame( int& serverTime, int& stereoView, int& demoPlayback )
{
	if ( feature_aimbot_ != nullptr )
	{
		feature_aimbot_->UpdateCVars( );

		feature_aimbot_->Render( );
	}

	if ( feature_esp_ != nullptr )
	{
		feature_esp_->UpdateCVars( );

		feature_esp_->Render( );
	}
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
void HackManager::OnPost_CL_CreateNewCommands( )
{
	if ( feature_aimbot_ )
	{
		feature_aimbot_->ClientCommand( );
	}
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
void HackManager::Startup( )
{
	Detour_Exception::EnableExceptionDetours( );

	HMODULE module_cgame = nullptr;

	while ( true )
	{
		HMODULE module_cgame_new = GetModuleHandle( "cgamex86.dll" );
		if ( module_cgame_new != module_cgame )
		{
			// This gives the game time to load or unload.
			//	It'd be better to check game's CG variables
			//	but just waiting a couple of seconds works as well.
			Sleep( 5000 );

			module_cgame = module_cgame_new;

			if ( module_cgame_new != nullptr )
				Install( );
			else
				Uninstall( );
		}

		Sleep( 100 );
	}
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
void HackManager::Shutdown( )
{
	Uninstall( );
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
void HackManager::Install( )
{
	SuspendMainThread( );

	game_ = new Game( GetMainThreadID( ), GetModuleHandle( "quakelive_steam.exe" ), GetModuleHandle( "cgamex86.dll" ) );

	detour_cg_draw_active_frame_ = new Detour_CG_DrawActiveFrame( dynamic_cast< Detour_CG_DrawActiveFrame_Callback* >( this ) );
	detour_cl_create_new_commands_ = new Detour_CL_CreateNewCommands( dynamic_cast< Detour_CL_CreateNewCommands_Callback* >( this ) );

	detour_cg_draw_active_frame_->Install( );
	detour_cl_create_new_commands_->Install( );

	feature_aimbot_ = new Feature_Aimbot( game_ );
	feature_esp_ = new Feature_ESP( game_ );

	feature_aimbot_->RegisterCVars( );
	feature_esp_->RegisterCVars( );

	ResumeMainThread( );
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
void HackManager::Uninstall( )
{
	SuspendMainThread( );

	if ( detour_cg_draw_active_frame_ != nullptr )
	{
		detour_cg_draw_active_frame_->Remove( );

		delete detour_cg_draw_active_frame_;

		detour_cg_draw_active_frame_ = nullptr;
	}
	
	if ( detour_cl_create_new_commands_ != nullptr )
	{
		detour_cl_create_new_commands_->Remove( );

		delete detour_cl_create_new_commands_;

		detour_cl_create_new_commands_ = nullptr;
	}

	if ( feature_aimbot_ != nullptr )
	{
		delete feature_aimbot_;

		feature_aimbot_ = nullptr;
	}

	if ( feature_esp_ != nullptr )
	{
		delete feature_esp_;

		feature_esp_ = nullptr;
	}

	if ( game_ != nullptr )
	{
		delete game_;

		game_ = nullptr;
	}

	ResumeMainThread( );
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
bool HackManager::EnumerateThreadIDs( std::vector< DWORD >& thread_ids )
{
	// 
	thread_ids.clear( );

	// Take a snapshot of all running threads  
	HANDLE thread_snapshot = CreateToolhelp32Snapshot( TH32CS_SNAPTHREAD, GetCurrentProcessId( ) ); 
	if ( thread_snapshot == INVALID_HANDLE_VALUE ) 
		return false; 

	// Fill in the size of the structure before using it. 
	THREADENTRY32 thread_entry; 
	thread_entry.dwSize = sizeof( thread_entry ); 

	// Retrieve information about the first thread,
	// and exit if unsuccessful
	if( Thread32First( thread_snapshot, &thread_entry ) ) 
	{
		//
		do
		{
			thread_ids.push_back( thread_entry.th32ThreadID );
		} while( Thread32Next( thread_snapshot, &thread_entry ) );
	}

	// 
	if ( !CloseHandle( thread_snapshot ) )
		return false;

	// 
	return true;
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
DWORD HackManager::GetThreadEntryPoint( DWORD thread_id )
{
	// 
	DWORD address_NtQueryInformationThread = ( DWORD )GetProcAddress( GetModuleHandle( "ntdll.dll" ), "NtQueryInformationThread" );
	if ( address_NtQueryInformationThread == 0 )
		return 0;

	// 
	HANDLE thread_handle = OpenThread( THREAD_QUERY_INFORMATION, FALSE, thread_id );
	if ( thread_handle == nullptr )
		return 0;

	//
	DWORD start_address = 0;


	// 
	long result = ( ( long ( __stdcall *  )( HANDLE, long, void*, DWORD, DWORD* ) )address_NtQueryInformationThread )( thread_handle, 9, &start_address, sizeof( start_address ), nullptr );
	
	// 
	CloseHandle( thread_handle );

	// 
	if ( result != 0 )
		return 0;

	// 
	return start_address;
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
DWORD HackManager::GetProcessEntryPoint( )
{
	// 
	MODULEINFO module_info;
	if ( !GetModuleInformation( GetCurrentProcess( ), GetModuleHandle( nullptr ), &module_info, sizeof( module_info ) ) )
		return 0;

	return ( DWORD )module_info.EntryPoint;
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
DWORD HackManager::GetMainThreadID( )
{
	static DWORD main_thread_id = 0;
	if ( main_thread_id != 0 )
		return main_thread_id;

	std::vector< DWORD > thread_ids;
	if ( !EnumerateThreadIDs( thread_ids ) )
		return 0;

	for ( auto it = thread_ids.begin( ); it != thread_ids.end( ); it++ )
	{
		if ( GetProcessEntryPoint( ) == GetThreadEntryPoint( *it ) )
		{
			main_thread_id = *it;

			return *it;
		}
	}

	return 0;
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
void HackManager::SuspendMainThread( )
{
	std::vector< DWORD > thread_ids;
	if ( !EnumerateThreadIDs( thread_ids ) )
		return;

	for ( auto it = thread_ids.begin( ); it != thread_ids.end( ); it++ )
	{
		if ( GetCurrentThreadId( ) == *it )
			continue;

		if ( GetMainThreadID( ) != *it )
			continue;

		HANDLE thread_handle = OpenThread( THREAD_SUSPEND_RESUME, FALSE, *it );
		if ( thread_handle == nullptr )
			continue;

		SuspendThread( thread_handle );

		CloseHandle( thread_handle );
	}
}

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
void HackManager::ResumeMainThread( )
{
	std::vector< DWORD > thread_ids;
	if ( !EnumerateThreadIDs( thread_ids ) )
		return;

	for ( auto it = thread_ids.begin( ); it != thread_ids.end( ); it++ )
	{
		if ( GetCurrentThreadId( ) == *it )
			continue;

		if ( GetMainThreadID( ) != *it )
			continue;

		HANDLE thread_handle = OpenThread( THREAD_SUSPEND_RESUME, FALSE, *it );
		if ( thread_handle == nullptr )
			continue;

		ResumeThread( thread_handle );

		CloseHandle( thread_handle );
	}
}

