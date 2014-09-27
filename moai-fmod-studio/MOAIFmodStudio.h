// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFMODSTUDIO_H
#define	MOAIFMODSTUDIO_H

#include <moai-core/headers.h>

namespace FMOD {
	class System;
	class Sound;
	class Channel;
	class ChannelGroup;
};

//================================================================//
// MOAIFmodStudio
//================================================================//
/**	@name	MOAIFmodStudio
	@text	FMOD singleton.
*/
class MOAIFmodStudio :
	public MOAIGlobalClass < MOAIFmodStudio, MOAILuaObject > {
private:

	FMOD::System* mSoundSys;
	FMOD::ChannelGroup* mMainChannelGroup;

	//----------------------------------------------------------------//
	static int	_getMemoryStats		( lua_State* L );
	static int	_init				( lua_State* L );
	static int _mute				( lua_State* L );

public:

	DECL_LUA_SINGLETON ( MOAIFmodStudio )

	GET ( FMOD::System*, SoundSys, mSoundSys );
	GET ( FMOD::ChannelGroup*, MainChannelGroup, mMainChannelGroup);

	//----------------------------------------------------------------//
	void			CloseSoundSystem	();
					MOAIFmodStudio		();
					~MOAIFmodStudio		();
	void			MuteChannels		( bool mute );
	void			OpenSoundSystem		();
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
	void			Update				();
	STLString		ToString			();
};

#endif