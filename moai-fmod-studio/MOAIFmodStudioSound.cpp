// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-fmod-studio/MOAIFmodStudio.h>
#include <moai-fmod-studio/MOAIFmodStudioSound.h>
#include <fmod.h>
#include <fmod_errors.h>

#ifdef MOAI_OS_IPHONE
	#include <fmod_ios.h>
#endif

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	load
	@text	Loads the specified sound from file, or from a MOAIDataBuffer.

	@overload
	
		@in		MOAIFmodStudioSound self
		@in		string filename			The path to the sound to load from file.
		@in		boolean streaming		Whether the sound should be streamed from the data source, rather than preloaded.
		@in		boolean	async			Whether the sound file should be loaded asynchronously.
		@out	nil

	@overload

		@in		MOAIFmodStudioSound self
		@in		MOAIDataBuffer data		The MOAIDataBuffer that is storing sound data.  You must either provide a string or MOAIDataBuffer, but not both.
		@in		boolean streaming		Whether the sound should be streamed from the data source, rather than preloaded.
		@out	nil
*/
int MOAIFmodStudioSound::_load ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodStudioSound, "U" )

	bool streaming	= state.GetValue < bool >( 3, false );
	bool async		= state.GetValue < bool >( 4, false );

	if ( state.IsType( 2, LUA_TSTRING ) ) {

		cc8* filename	= state.GetValue < cc8* >( 2, "" );
		memcpy( self->mFileName, filename, strlen ( filename ));
		self->Load ( filename, streaming, async );
	}
	else {
		
		MOAIDataBuffer* data = state.GetLuaObject < MOAIDataBuffer >( 2, true );
		if ( data ) {
			self->Load( *data, streaming );
		}
	}
    self->mType = TYPE_UNSET;
	return 0;
}

//----------------------------------------------------------------//
/**	@name	loadBGM
	@text	Loads the specified BGM sound from file, or from a MOAIDataBuffer.

	@overload
	@in		MOAIFmodStudioSound self
	@in		string filename			The path to the sound to load from file.
	@out	nil
	
	@overload
	@in		MOAIFmodStudioSound self
	@in		MOAIDataBuffer data		The MOAIDataBuffer that is storing sound data.
	@out	nil
*/
int	MOAIFmodStudioSound::_loadBGM ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodStudioSound, "U" )

	
	if ( state.IsType( 2, LUA_TSTRING ) ) {

		cc8* filename	= state.GetValue < cc8* >( 2, "" );
		memcpy( self->mFileName, filename, strlen ( filename ));
		self->Load ( filename, true, false );
	}
	else {
	
		MOAIDataBuffer* data = state.GetLuaObject < MOAIDataBuffer >( 2, true );
		if ( data ) {
			self->Load( *data, true );
		}
	}
    self->mType = TYPE_BGM;
	return 0;
}

//----------------------------------------------------------------//
/**	@name	loadSFX
	@text	Loads the specified SFX sound from file, or from a MOAIDataBuffer.

	@overload
	@in		MOAIFmodStudioSound self
	@in	string filename			The path to the sound to load from file.
	@out	nil

	@overload
	@in		MOAIFmodStudioSound self
	@in		MOAIDataBuffer data		The MOAIDataBuffer that is storing sound data.
	@out	nil
*/
int	MOAIFmodStudioSound::_loadSFX ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodStudioSound, "U" )

	if ( state.IsType( 2, LUA_TSTRING ) ) {

		cc8* filename	= state.GetValue < cc8* >( 2, "" );
		memcpy( self->mFileName, filename, strlen ( filename ));
		self->Load ( filename, false, true );
	}
	else {
		MOAIDataBuffer* data = state.GetLuaObject < MOAIDataBuffer >( 2, true );
		if ( data ) {
			self->Load( *data, false );
		}
	}
    self->mType = TYPE_SFX;
	return 0;
}

//----------------------------------------------------------------//
/**	@name	release
	@text	Releases the sound data from memory.

	@in		MOAIFmodStudioSound self
	@out	nil
*/
int MOAIFmodStudioSound::_release ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodStudioSound, "U" )

	self->ReleaseSound ();

	return 0;
}

//================================================================//
// MOAIFmodStudioSound
//================================================================//

//----------------------------------------------------------------//
MOAIFmodStudioSound::MOAIFmodStudioSound () :
	mSound ( 0 ),
	mLoopCount ( 0 ),
	mType ( TYPE_UNSET ) {

	RTTI_SINGLE ( MOAILuaObject )

	memset ( mFileName, 0, 128 );
}

//----------------------------------------------------------------//
MOAIFmodStudioSound::~MOAIFmodStudioSound () {

	this->ReleaseSound ();
}

//----------------------------------------------------------------//
void MOAIFmodStudioSound::Load ( MOAIDataBuffer& data, bool streaming ) {

	if ( this->mSound ) return;
	
	FMOD_SYSTEM* soundSys = MOAIFmodStudio::Get ().GetSoundSys ();
	if ( !soundSys ) return;
	
	void* bytes;
	size_t size;
	data.Lock ( &bytes, &size );

	FMOD_MODE mode = FMOD_OPENMEMORY;
	mode |= streaming ? FMOD_CREATESTREAM : FMOD_DEFAULT;

	FMOD_CREATESOUNDEXINFO info;
	memset ( &info, 0, sizeof ( FMOD_CREATESOUNDEXINFO ));
	info.cbsize = sizeof ( FMOD_CREATESOUNDEXINFO );
	info.length = size;

	FMOD_RESULT result;
	FMOD_SOUND* sound = 0;

	result = FMOD_System_CreateSound ( soundSys, ( cc8* )bytes, mode, &info, &sound );
	
	data.Unlock ();
	
	if ( MOAIFmodCheckError ( result )) return;
	
	this->mSound = sound;
}

//----------------------------------------------------------------//
void MOAIFmodStudioSound::Load ( cc8* filename, bool streaming, bool async ) {

	async = false;
	if ( this->mSound ) return;
	
	FMOD_SYSTEM* soundSys = MOAIFmodStudio::Get ().GetSoundSys ();
	if ( !soundSys ) return;
	
	FMOD_MODE mode = 0;
//	mode = streaming ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE;
    
    // FMOD_CREATESAMPLE is faster, use this.
    mode = streaming ? FMOD_CREATESTREAM : FMOD_CREATESAMPLE;
	mode |= async ? FMOD_NONBLOCKING : 0;
	
	FMOD_RESULT result;
	FMOD_SOUND* sound = 0;
	FMOD_CREATESOUNDEXINFO info;
	memset ( &info, 0, sizeof ( FMOD_CREATESOUNDEXINFO ));
	
	info.cbsize = sizeof ( FMOD_CREATESOUNDEXINFO );
	
	#ifdef MOAI_OS_IPHONE
    // Why??
//		info.audioqueuepolicy = FMOD_AUDIOQUEUE_CODECPOLICY_SOFTWAREONLY;
	#endif

	if ( streaming ) {
		result = FMOD_System_CreateStream ( soundSys, filename, mode, &info, &sound );
	}
	else {
		result = FMOD_System_CreateSound ( soundSys, filename, mode, &info, &sound );
	}
	
	if ( !MOAIFmodCheckError ( result )) return;
    
	this->mSound = sound;
}

//----------------------------------------------------------------//
void MOAIFmodStudioSound::ReleaseSound () {

	if ( !this->mSound ) return;
	
	if ( MOAIFmodStudio::IsValid ()) {
        FMOD_RESULT result = FMOD_Sound_Release ( this->mSound );
        MOAIFmodCheckError ( result );
	}
	this->mSound = 0;
}

//----------------------------------------------------------------//
void MOAIFmodStudioSound::RegisterLuaClass ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIFmodStudioSound::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "load",			_load },
		{ "loadBGM",		_loadBGM },
		{ "loadSFX",		_loadSFX },
		{ "release",		_release },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

