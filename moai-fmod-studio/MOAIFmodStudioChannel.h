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
	FMOD_CHANNEL* mChannel;
	
	typedef enum {
		STOPPED = 0,
		PLAYING,
		PAUSED,
	} PlayState;

	float	mPitch;
	float	mPan;
	float	mVolume;
	bool	mPaused;
	bool	mLooping;
    bool    mReusable;
	PlayState mPlayState;

	//----------------------------------------------------------------//
	static int	_getPan				( lua_State* L );
	static int	_getPitch			( lua_State* L );
	static int	_getVolume			( lua_State* L );
	static int  _isPlaying			( lua_State* L );
	static int	_moveVolume			( lua_State* L );
	static int	_play				( lua_State* L );
	static int	_seekVolume			( lua_State* L );
	static int  _setLooping			( lua_State* L );
	static int	_setPan				( lua_State* L );
	static int	_setPaused			( lua_State* L );
	static int	_setPitch			( lua_State* L );
	static int  _setReusable		( lua_State* L );
	static int	_setVolume			( lua_State* L );
	static int	_stop				( lua_State* L );

public:

	friend class MOAIFmodStudio;

	DECL_LUA_FACTORY ( MOAIFmodStudioChannel )
	DECL_ATTR_HELPER ( MOAIFmodStudioChannel )

	enum {
		ATTR_VOLUME,
		ATTR_PITCH,
		ATTR_PAN,
		TOTAL_ATTR,
	};

	//----------------------------------------------------------------//
	bool		ApplyAttrOp			( u32 attrID, MOAIAttrOp& attrOp, u32 op );
	float		GetPan				();
	float		GetPitch			();
	float		GetVolume			();
    bool        IsPlaying           ();
				MOAIFmodStudioChannel	();
				~MOAIFmodStudioChannel	();
	void		Play				( MOAIFmodStudioSound* sound, int loopCount );
	void		RegisterLuaClass	( MOAILuaState& state );
	void		RegisterLuaFuncs	( MOAILuaState& state );
	void		SetPaused			( bool paused );
	void		SetPan				( float pan );
	void		SetPitch			( float pitch );	
	void		SetVolume			( float volume );
	void		Stop				();
	STLString	ToString			();
};

#endif
