// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-fmod-studio/MOAIFmodStudio.h>
#include <fmod.h>
#include <fmod_errors.h>

bool MOAIFmodCheckError ( FMOD_RESULT result ) {
    
    if ( result != FMOD_OK ) {
        ZLLog::Print ( "FMOD Error: %s", FMOD_ErrorString ( result ));
        return false;
    }
    return true;
}

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	getMemoryStats
	@text	Get memory usage.

	@opt	boolean blocking		Default value is 'false.'
	@out	number currentAlloc
	@out	number maxAlloc
*/
int	MOAIFmodStudio::_getMemoryStats( lua_State* L ) {
	MOAILuaState state ( L );
	
	bool blocking = state.GetValue < bool >( 1, false );
	
	int currentAlloc;
	int maxAlloc;
	
	FMOD_Memory_GetStats ( &currentAlloc, &maxAlloc, blocking );
	
	lua_pushnumber ( state, currentAlloc );
	lua_pushnumber ( state, maxAlloc );
	
	return 2;
}

//----------------------------------------------------------------//
int MOAIFmodStudio::_getVolume ( lua_State* L ) {
    
	MOAILuaState state ( L );
	
    MOAIFmodStudio& system = MOAIFmodStudio::Get();
	state.Push ( system.GetVolume ( system.mMainChannelGroup ));
	return 1;
}

//----------------------------------------------------------------//
int MOAIFmodStudio::_getBGMVolume ( lua_State* L ) {
    
	MOAILuaState state ( L );
	
    MOAIFmodStudio& system = MOAIFmodStudio::Get();
	state.Push ( system.GetVolume ( system.mBGMChannelGroup ));
	return 1;
}

//----------------------------------------------------------------//
int MOAIFmodStudio::_getSFXVolume ( lua_State* L ) {
    
	MOAILuaState state ( L );
	
    MOAIFmodStudio& system = MOAIFmodStudio::Get();
	state.Push ( system.GetVolume ( system.mSFXChannelGroup ));
	return 1;
}

//----------------------------------------------------------------//
/**	@name	init
	@text	Initializes the sound system.
    
    @in     number  numChannels     maximum number of active channels. These are managed by fmod
                                    and there is no hardware limit. Default is 100
	@out	nil
*/
int MOAIFmodStudio::_init ( lua_State* L ) {

	MOAILuaState state ( L );
    
    u32 channels = state.GetValue < u32 >( 1, 100 );
	MOAIFmodStudio::Get ().OpenSoundSystem ( channels );

	return 0;
}

//----------------------------------------------------------------//
int MOAIFmodStudio::_mute ( lua_State* L ) {

	MOAILuaState state ( L );
	
	bool mute = state.GetValue < bool >( 1, false );
    
    MOAIFmodStudio& system = MOAIFmodStudio::Get();
	system.MuteChannels ( mute, system.mMainChannelGroup );
	return 0;
}

//----------------------------------------------------------------//
int MOAIFmodStudio::_muteBGM ( lua_State* L ) {
    
	MOAILuaState state ( L );
	
	bool mute = state.GetValue < bool >( 1, false );
    
    MOAIFmodStudio& system = MOAIFmodStudio::Get();
	system.MuteChannels ( mute, system.mBGMChannelGroup );
	return 0;
}

//----------------------------------------------------------------//
int MOAIFmodStudio::_muteSFX ( lua_State* L ) {
    
	MOAILuaState state ( L );
	
	bool mute = state.GetValue < bool >( 1, false );
    
    MOAIFmodStudio& system = MOAIFmodStudio::Get();
	system.MuteChannels ( mute, system.mSFXChannelGroup );
	return 0;
}

//----------------------------------------------------------------//
int MOAIFmodStudio::_setVolume ( lua_State* L ) {
    
	MOAILuaState state ( L );
	
	float volume = state.GetValue < float >( 1, 1.0f );
    
    MOAIFmodStudio& system = MOAIFmodStudio::Get();
	system.SetVolume ( volume, system.mMainChannelGroup );
	return 0;
}

//----------------------------------------------------------------//
int MOAIFmodStudio::_setBGMVolume ( lua_State* L ) {
    
	MOAILuaState state ( L );
	
	float volume = state.GetValue < float >( 1, 1.0f );
    
    MOAIFmodStudio& system = MOAIFmodStudio::Get();
	system.SetVolume ( volume, system.mBGMChannelGroup );
	return 0;
}

//----------------------------------------------------------------//
int MOAIFmodStudio::_setSFXVolume ( lua_State* L ) {
    
	MOAILuaState state ( L );
	
	float volume = state.GetValue < float >( 1, 1.0f );
    
    MOAIFmodStudio& system = MOAIFmodStudio::Get();
	system.SetVolume ( volume, system.mSFXChannelGroup );
	return 0;
}

//================================================================//
// MOAIFmodStudio
//================================================================//

//----------------------------------------------------------------//
void MOAIFmodStudio::CloseSoundSystem () {

	if ( !this->mSoundSys ) return;
	
    FMOD_System_Close ( this->mSoundSys );
	FMOD_System_Release ( this->mSoundSys );
	this->mSoundSys = 0;
}

//----------------------------------------------------------------//
float MOAIFmodStudio::GetVolume ( FMOD_CHANNELGROUP* group ) {
    
    if ( group ) {
        float volume;
        FMOD_RESULT result = FMOD_ChannelGroup_GetVolume ( group, &volume );
        MOAIFmodCheckError ( result );
        return volume;
    }
    return 0;
}

//----------------------------------------------------------------//
MOAIFmodStudio::MOAIFmodStudio () :
	mSoundSys ( 0 ),
    mMainChannelGroup ( 0 ),
    mBGMChannelGroup ( 0 ),
    mSFXChannelGroup ( 0 ) {

}

//----------------------------------------------------------------//
MOAIFmodStudio::~MOAIFmodStudio () {

	this->CloseSoundSystem ();
}

//----------------------------------------------------------------//
void MOAIFmodStudio::MuteChannels ( bool mute, FMOD_CHANNELGROUP* group ) {

	if ( group ) {
		FMOD_RESULT result = FMOD_ChannelGroup_SetMute ( group, mute );
        MOAIFmodCheckError ( result );
	}
}

//----------------------------------------------------------------//
void MOAIFmodStudio::OpenSoundSystem ( u32 channels ) {

	FMOD_RESULT result;

	//FMOD::Debug_SetLevel(FMOD_DEBUG_ALL);
	//FMOD::Debug_SetLevel(FMOD_DEBUG_LEVEL_ERROR);

	result = FMOD_System_Create ( &this->mSoundSys ); // Create the main system object.
	if ( !MOAIFmodCheckError ( result ) ) return;

	result = FMOD_System_Init ( this->mSoundSys, channels, FMOD_INIT_NORMAL, 0 );
	if ( !MOAIFmodCheckError ( result ) ) return;
	
	result =  FMOD_System_GetMasterChannelGroup ( this->mSoundSys, &this->mMainChannelGroup );
	if ( !MOAIFmodCheckError ( result ) ) return;

    result = FMOD_System_CreateChannelGroup ( this->mSoundSys, "effects", &this->mSFXChannelGroup );
    MOAIFmodCheckError ( result );
    
    result = FMOD_System_CreateChannelGroup ( this->mSoundSys, "music", &this->mBGMChannelGroup );
    MOAIFmodCheckError ( result );
}

//----------------------------------------------------------------//
void MOAIFmodStudio::RegisterLuaClass ( MOAILuaState& state ) {
	
	luaL_Reg regTable [] = {
		{ "getMemoryStats",		_getMemoryStats },
		{ "getVolume",			_getVolume },
		{ "getBGMVolume",		_getBGMVolume },
		{ "getSFXVolume",		_getSFXVolume },
		{ "init",				_init },
		{ "mute",				_mute },
		{ "muteBGM",			_muteBGM },
		{ "muteSFX",			_muteSFX },
		{ "setVolume",			_setVolume },
		{ "setBGMVolume",		_setBGMVolume },
		{ "setSFXVolume",		_setSFXVolume },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIFmodStudio::RegisterLuaFuncs ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIFmodStudio::Resume () {
    
    if ( this->mSoundSys ) {
        FMOD_RESULT result = FMOD_System_MixerResume ( this->mSoundSys );
        MOAIFmodCheckError ( result );
    }
}

//----------------------------------------------------------------//
void MOAIFmodStudio::SetVolume ( float volume, FMOD_CHANNELGROUP* group ) {
    
    if ( group ) {
        FMOD_RESULT result = FMOD_ChannelGroup_SetVolume ( group, volume );
        MOAIFmodCheckError ( result );
    }
}

//----------------------------------------------------------------//
void MOAIFmodStudio::Suspend () {
    
    if ( this->mSoundSys ) {
        FMOD_RESULT result = FMOD_System_MixerSuspend ( this->mSoundSys );
        MOAIFmodCheckError ( result );
    }
}

//----------------------------------------------------------------//
void MOAIFmodStudio::Update () {

	if ( this->mSoundSys ) {
		FMOD_RESULT result = FMOD_System_Update ( this->mSoundSys );
        MOAIFmodCheckError ( result );
	}
}

