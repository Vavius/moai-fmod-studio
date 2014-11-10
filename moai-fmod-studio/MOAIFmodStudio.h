// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFMODSTUDIO_H
#define	MOAIFMODSTUDIO_H

#include <moai-core/headers.h>
#include <fmod.h>

bool MOAIFmodCheckError ( FMOD_RESULT result );


//================================================================//
// MOAIFmodStudio
//================================================================//
/**	@name	MOAIFmodStudio
	@text	FMOD singleton.
*/
class MOAIFmodStudio :
	public MOAIGlobalClass < MOAIFmodStudio, MOAILuaObject > {
private:

	FMOD_SYSTEM* 		mSoundSys;
	FMOD_CHANNELGROUP* 	mMainChannelGroup;
	FMOD_CHANNELGROUP* 	mBGMChannelGroup;
	FMOD_CHANNELGROUP* 	mSFXChannelGroup;

	//----------------------------------------------------------------//
	static int	_getMemoryStats			( lua_State* L );
	static int  _getVolume				( lua_State* L );
	static int  _getBGMVolume			( lua_State* L );
	static int  _getSFXVolume			( lua_State* L );
	static int	_init					( lua_State* L );
	static int  _mute					( lua_State* L );
	static int  _muteBGM				( lua_State* L );
	static int  _muteSFX				( lua_State* L );
	static int  _setVolume				( lua_State* L );
	static int  _setBGMVolume			( lua_State* L );
    static int  _setLogEnabled          ( lua_State* L );
    static int  _setSFXVolume			( lua_State* L );

public:

	DECL_LUA_SINGLETON ( MOAIFmodStudio )

	GET ( FMOD_SYSTEM*, SoundSys, mSoundSys );
	GET ( FMOD_CHANNELGROUP*, MainChannelGroup, mMainChannelGroup);
	GET ( FMOD_CHANNELGROUP*, BGMChannelGroup, mBGMChannelGroup);
	GET ( FMOD_CHANNELGROUP*, SFXChannelGroup, mSFXChannelGroup);

	//----------------------------------------------------------------//
	void			CloseSoundSystem	();
	float			GetVolume			( FMOD_CHANNELGROUP* group );
					MOAIFmodStudio		();
					~MOAIFmodStudio		();
	void			MuteChannels		( bool mute, FMOD_CHANNELGROUP* group );
	void			OpenSoundSystem		( u32 channels );
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
	void			Resume				();
	void			SetVolume			( float volume, FMOD_CHANNELGROUP* group );
	void			Suspend				();
	void			Update				();
	STLString		ToString			();
};

#endif