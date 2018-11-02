// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------

#pragma once

// ===== Project includes =====
#include "feature.h"


// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
class Feature_Aimbot : public Feature
{
public:
	
	// 
	Feature_Aimbot( Game* game );
	~Feature_Aimbot( );

	// 
	virtual void		RegisterCVars( );
	virtual void		UpdateCVars( );
	
	// 
	virtual void		Render( );
	virtual void		ClientCommand( );


private:

	struct Target
	{
		int				entity_index;
		vec3_t			position;
		float			distance;

		Target( )
		{
			entity_index = 0;
			distance = 0.0;

			VectorClear( position );
		}

		bool operator < ( const Target& rhs )
		{
			return distance < rhs.distance;
		}
	};

	// 
	usercmd_t*			CreateClientCommand( );

	// 
	void				GetMuzzlePosition( vec3_t& muzzle_position ) const;
	int					GetWeaponRange( int weapon_id ) const;

	// 
	bool				CheckTarget( int entity_index, Target* out_target = nullptr );
	bool				FindTargets( std::vector< Target >& target_list );

	// 
	void				AimAtPosition( vec3_t position, usercmd_t* client_command, bool silent = false );

	// 
	bool				is_active_;

	// 
	int					current_target_entity_index_;

	// 
	vmCvar_t			cvar_aim_enabled_;
	vmCvar_t			cvar_aim_silent_;
};

