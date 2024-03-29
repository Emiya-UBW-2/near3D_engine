#pragma once

enum class Key {
	KEY_M_LEFT,
	KEY_M_MID,
	KEY_M_RIGHT,
	KEY_NO_1,
	KEY_NO_2,
	KEY_NO_3,
	KEY_NO_4,
	KEY_NO_5,
	KEY_NO_6,
	KEY_UP,
	KEY_DOWN,
	KEY_LEFT,
	KEY_RIGHT,
	KEY_ESCAPE,
	KEY_PAUSE,
	ACTIVE,
	ON_PAD,
	NUM,
};

enum class ENUM_SOUND {
	RUN,
	WALK,
	Punch,
	Kick,
	Hit,
	DownHit,
	Fire0,
	Case0,
	MagDown0,
	MagSet0,
	Slide0,
	Trigger,
	Equip,
	WallHit0,
	WallHit1,
	Look,
	Envi,
	rolling,
};

enum class ENUM_BGM {
	Phase1,
	Phase2,
	Phase3,
};

enum class Anim_Sel {
	WALK,
	STAND,
	RUN,

	PRONEMOVE,
	PRONE,

	SQUAT,

	EQUIPSTAND_HITWALL,
	EQUIPWALK_HITWALL,

	EQUIPPRONEMOVE,
	EQUIPPRONE,

	EQUIPSTAND,
	EQUIPWALK,

	RELOADPRONE,
	RELOADSTAND,
	RELOADWALK,

	ROLLING,

	PUNCH1,
	PUNCH2,
	KICK,

	NOMALDAMAGE,
	DOWNDAMAGE,
	DOWN,

	NUM,//22
};

enum class Bone_Sel {
	LEFTHAND = 0,
	LEFTARM2 = 3,//2
	LEFTARM1 = 4,

	RIGHTARM1 = 6,
	RIGHTARM2 = 7,
	RIGHTHAND = 10,

	HEAD = 15,

	BODYTOP = 5,
	BODYMIDDLE = 16,
	BODYBOTTOM = 27,

	LEFTLEG3 = 22,
	LEFTLEG2 = 24,
	LEFTLEG1 = 26,

	RIGHTLEG1 = 28,
	HOLSTER = 17,
	RIGHTLEG2 = 30,
	RIGHTLEG3 = 32,

	NUM = 33,
};

enum class GUN_SELECT {
	SEMI,
	FULL,
};
