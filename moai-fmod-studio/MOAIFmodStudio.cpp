// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-fmod-studio/MOAIFmodStudio.h>
#include <fmod.hpp>


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
/**	@name	init
	@text	Initializes the sound system.

	@out	nil
*/
int MOAIFmodStudio::_init ( lua_State* L ) {

	MOAILuaState state ( L );
	MOAIFmodStudio::Get ().OpenSoundSystem ();

	return 0;
}

int MOAIFmodStudio::_mute ( lua_State* L ) {
	MOAILuaState state ( L );
	
	bool mute = state.GetValue < bool >( 1, false );

	MOAIFmodStudio::Get().MuteChannels ( mute );
	return 0;
}

//================================================================//
// MOAIFmodStudio
//================================================================//

//----------------------------------------------------------------//
void MOAIFmodStudio::CloseSoundSystem () {

	if ( !this->mSoundSys ) return;
	
	this->mSoundSys->close ();
	this->mSoundSys->release ();
	this->mSoundSys = 0;
}

//----------------------------------------------------------------//
MOAIFmodStudio::MOAIFmodStudio () :
	mSoundSys ( 0 ) {
}

//----------------------------------------------------------------//
MOAIFmodStudio::~MOAIFmodStudio () {

	this->CloseSoundSystem ();
}

//----------------------------------------------------------------//
void MOAIFmodStudio::MuteChannels ( bool mute ) {

	this->mMainChannelGroup->setMute ( mute );
}

//----------------------------------------------------------------//
void MOAIFmodStudio::OpenSoundSystem () {

	FMOD_RESULT result;

	//FMOD::Debug_SetLevel(FMOD_DEBUG_ALL);
	//FMOD::Debug_SetLevel(FMOD_DEBUG_LEVEL_ERROR);

	result = FMOD::System_Create ( &this->mSoundSys ); // Create the main system object.
	if ( result != FMOD_OK ) return;

	result = this->mSoundSys->init ( 100, FMOD_INIT_NORMAL, 0 );

	if ( result != FMOD_OK ) return;
	
	result = this->mSoundSys->getMasterChannelGroup ( &this->mMainChannelGroup );
	if ( result != FMOD_OK ) return;
}

//----------------------------------------------------------------//
void MOAIFmodStudio::RegisterLuaClass ( MOAILuaState& state ) {
	
	luaL_Reg regTable [] = {
		{ "getMemoryStats",		_getMemoryStats },
		{ "init",				_init },
		{ "mute",				_mute },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIFmodStudio::RegisterLuaFuncs ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIFmodStudio::Update () {

	if ( this->mSoundSys ) {
		this->mSoundSys->update ();
	}
}

