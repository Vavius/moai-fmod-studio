// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFMODSTUDIOCHANNEL_H
#define	MOAIFMODSTUDIOCHANNEL_H

#include <moai-sim/headers.h>

class MOAIFmodStudioSound;

//================================================================//
// MOAIFmodStudioChannel
//================================================================//
/**	@name	MOAIFmodStudioChannel
	@text	FMOD singleton.
*/
class MOAIFmodStudioChannel :
	public virtual MOAINode {
private:

	MOAISharedPtr < MOAIFmodStudioSound > mSound;
	FMOD::Channel* mChannel;
	
	typedef enum {
		STOPPED = 0,
		PLAYING,
		PAUSED,
	} PlayState;

	float	mVolume;
	bool	mPaused;
	bool	mLooping;
	PlayState mPlayState;

	//----------------------------------------------------------------//'
	static int	_getVolume			( lua_State* L );
	static int  _isPlaying			( lua_State* L );
	static int	_moveVolume			( lua_State* L );
	static int	_play				( lua_State* L );
	static int	_seekVolume			( lua_State* L );
	static int	_setPaused			( lua_State* L );
	static int	_setVolume			( lua_State* L );
	static int  _setLooping			( lua_State* L );
	static int	_stop				( lua_State* L );

public:

	friend class MOAIFmodStudio;

	DECL_LUA_FACTORY ( MOAIFmodStudioChannel )
	DECL_ATTR_HELPER ( MOAIFmodStudioChannel )

	enum {
		ATTR_VOLUME,
		TOTAL_ATTR,
	};

	//----------------------------------------------------------------//
	bool		ApplyAttrOp			( u32 attrID, MOAIAttrOp& attrOp, u32 op );
	float		GetVolume			();
				MOAIFmodStudioChannel	();
				~MOAIFmodStudioChannel	();
	void		Play				( MOAIFmodStudioSound* sound, int loopCount );
	void		RegisterLuaClass	( MOAILuaState& state );
	void		RegisterLuaFuncs	( MOAILuaState& state );
	void		SetPaused			( bool paused );
	void		SetVolume			( float volume );
	void		Stop				();
	STLString	ToString			();
};

#endif
