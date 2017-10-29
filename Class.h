typedef unsigned int        uint;
typedef char				int8;
typedef unsigned char		uint8;
typedef short int			int16;
typedef unsigned short int	uint16;
typedef int					int32;
typedef unsigned int		uint32;
typedef unsigned int		LTBOOL;
typedef float				LTFLOAT;
typedef double				LTDOUBLE;
typedef uint32				LTRESULT;
typedef void*				HOBJECT;
typedef void*				HLOCALOBJ;
typedef uint32 HMODELPIECE;
#define INVALID_MODEL_PIECE		((HMODELPIECE)-1)
typedef uint32 HMODELSOCKET;
#define INVALID_MODEL_SOCKET	((HMODELSOCKET)-1)
typedef uint32 HMODELNODE;
#define INVALID_MODEL_NODE		((HMODELNODE)-1)
typedef uint32 HMODELWEIGHTSET;
#define INVALID_MODEL_WEIGHTSET ((HMODELWEIGHTSET)-1)
typedef uint32 HMODELANIM;
#define INVALID_MODEL_ANIM		((HMODELANIM)-1)

class LTRotation
{
public:
	float x, y, z, w;
};
class LTObject
{
public:
	char _0x0000[4];
	D3DXVECTOR3 BoxMin; //0x0004 
	D3DXVECTOR3 BoxMax; //0x0010 
	char _0x001C[128];
	D3DXVECTOR3 Scale; //0x009C 
	char _0x00A8[4];
	unsigned int Type; //0x00AC 
	char _0x00B0[8];
	D3DXVECTOR3 Dimentions; //0x00B8 
	char _0x00C4[12];
	D3DXVECTOR3 ModelCullingPosition; //0x00D0 
	char _0x00DC[4];
	D3DXVECTOR3 Position; //0x00E0 
	LTRotation Rotation; //0x00EC 
	D3DXVECTOR3 Velocity; //0x00FC 
	D3DXVECTOR3 GlobalForceOverride; //0x0108 
	char _0x0114[8];
	D3DXVECTOR3 Acceleration; //0x011C 

};//Size=0x0128

class UnkWeapon
{
public:
	char Spacer[9];
	char WeaponName[15];
};
struct UnkStruct
{
	char spacer[215000];
	UnkWeapon* Weapon;
	char unk[8];
	int weaponid;
};

struct Transform
{
	D3DXVECTOR3 Pos;
	char Spacer[0x100];
};

class cObject
{
public:
	char Spacer[4];
	D3DXVECTOR3 Max;
	D3DXVECTOR3 Min;
	char spacer01[196];
	D3DXVECTOR3 Position;
public:
	FORCEINLINE D3DXVECTOR3 GetMin(void)
	{
		return (this != nullptr) ? Min : D3DXVECTOR3(0, 0, 0);
	}
	FORCEINLINE D3DXVECTOR3 GetMax(void)
	{
		return (this != nullptr) ? Max : D3DXVECTOR3(0, 0, 0);
	}
	FORCEINLINE D3DXVECTOR3 GetPos(void)
	{
		return (this != nullptr) ? Position : D3DXVECTOR3(0, 0, 0);
	}
};

struct CPlayer
{
	char __unknown[8];
	cObject* Object;
	BYTE index; //0x000C 
	BYTE Team; //0x000D 
	char Name[14]; //0x000E
	UnkStruct *unk;
	int PlayerSlotTeam;
	int unk1;
	bool Has_C4;
	int State;
	int Rank;
	unsigned long int QQNum;
	int unk2;
	short Health;
	short Kills;
	__int32 Deaths;
	__int32 HeadShots;
	__int32 TeamID;
	__int32 Ping;

	FORCEINLINE bool IsValid(void)
	{
		return (this != nullptr && Object != nullptr) ? true : false;
	}
};

struct HPOLY
{
	HPOLY() : m_nWorldIndex(0xFFFFFFFF), m_nPolyIndex(0xFFFFFFFF) {}
	HPOLY(__int32 nWorldIndex, __int32 nPolyIndex) : m_nWorldIndex(nWorldIndex), m_nPolyIndex(nPolyIndex) {}
	HPOLY(const HPOLY& hPoly) : m_nWorldIndex(hPoly.m_nWorldIndex), m_nPolyIndex(hPoly.m_nPolyIndex) {}

	HPOLY &operator=(const HPOLY &hOther)
	{
		m_nWorldIndex = hOther.m_nWorldIndex;
		m_nPolyIndex = hOther.m_nPolyIndex;
		return *this;
	}

	bool operator==(const HPOLY &hOther) const
	{
		return (m_nWorldIndex == hOther.m_nWorldIndex) && (m_nPolyIndex == hOther.m_nPolyIndex);
	}
	bool operator!=(const HPOLY &hOther) const
	{
		return (m_nWorldIndex != hOther.m_nWorldIndex) || (m_nPolyIndex != hOther.m_nPolyIndex);
	}
	__int32 m_nPolyIndex, m_nWorldIndex;
};

enum
{
	INTERSECT_OBJECTS = (1 << 0),
	IGNORE_NONSOLID = (1 << 1),
	INTERSECT_HPOLY = (1 << 2)
};

typedef bool(*ObjectFilterFn)(cObject* hObj, void *pUserData);
typedef bool(*PolyFilterFn)(HPOLY hPoly, void *pUserData, const D3DXVECTOR3& vIntersectPoint);

struct IntersectInfo
{
	D3DXVECTOR3 vImpactPos;		//0x0000
	char spacer00[16];
	//	D3DXVECTOR3 ImpactPlane;	//0x0012
	cObject* hObjImpact;		//0x001C
	DWORD nSomething;			//0x0020
	DWORD nSomething2;			//0x0024
	DWORD nSomething3;			//0x0028
};

struct IntersectQuery
{
	D3DXVECTOR3 m_From;
	D3DXVECTOR3 m_To;
	char unknown1[12]; //0x0018
	unsigned int Flags; //0x0024 
	ObjectFilterFn FilterFn; //0x0028 
	void* FilterIntersectParam; //0x002C 
	ObjectFilterFn FilterIntersectFn; //0x0030 
	void* FilterParam; //0x0034 
	void* PolyFilterParam; //0x0038 
};

class cLTModel
{
public:
	virtual void Spacer00(); // 0x0
	virtual void Spacer01(); // 0x4
	virtual void Spacer02(); // 0x8
	virtual void Spacer03(); // 0xC
	virtual void Spacer04(); // 0x10
	virtual void Spacer05(); // 0x14
	virtual void Spacer06(); // 0x18
	virtual void Spacer07(); // 0x1C
	virtual void Spacer08(); // 0x20
	virtual void Spacer09(); // 0x24
	virtual void Spacer10(); // 0x28
	virtual void Spacer11(); // 0x2C
	virtual void Spacer12(); // 0x30
	virtual void Spacer13(); // 0x34
	virtual UINT32 GetNodeName(cObject *hObj, UINT32 hNode, char *name, UINT32 maxlen); // 0x38
	virtual UINT32 GetNodeTransform(cObject *hObj, UINT32 hNode, Transform* transform, bool bWorldSpace); // 0x3C
};

class ILTClient
{
public:
	char _0x0000[0x68];
	bool(*IntersectSegment)(IntersectQuery &pQuery, IntersectInfo *pInfo); //0x0068
	char _0x006C[0x19C];
	void(*PushToConsole)(const char *Command); //0x0208
};

class CPlayerPos
{
public:
	DWORD unknown;
	D3DXVECTOR3 Pos1;
	D3DXVECTOR3 Pos2;
};

class CLTPlayerClient
{
public:
	char Spacer1[532];; //0x0000 
	float Spread000; //0x0214 
	DWORD unknown0; //0x0218 
	DWORD MoveState1; //0x021C 
	DWORD MoveState2; //0x0220 
	DWORD Recoillvl; //0x0224 
	DWORD Recoillv2; //0x0228 
	DWORD Recoillv3; //0x022C 
	DWORD Recoillv4; //0x0230 
	char Spacer2[288];
	CPlayerPos* pPlayerPos1;
	CPlayerPos* pPlayerPos2;
	DWORD unknown; //0x0358 
	float Pitch; //0x035C 
	float Yaw; //0x0360 
	float Roll; //0x0364 
	float Recoil001; //0x0368 
	float Recoil002; //0x036C 
	float UnknownF001; //0x0370 
	float Recoil003; //0x0374 
	float Recoil004; //0x0378 
	float UnknownF002; //0x037C 
	float Recoil005; //0x0380 
	float UnknownF003; //0x0384 
	float CameraYaw2; //0x0388 
	float UnknownF004; //0x038C 
	float CameraPitch; //0x0390 
	float CameraYaw; //0x0394 
	float CameraRoll; //0x0398 
	float RotationSpeed; //0x039C 
	float UnknownF005;	//0x03A0
	float UnknownF006;	//0x03A4
	D3DXVECTOR3 Positon; //0x03A8 
	D3DXVECTOR3 Velocity; //0x03B4 
	float Pitch2; //0x03C0 
	float Yaw2; //0x03C4 
	float Roll2; //0x03C8 
	char Spacer3[64]; //0x03CC 
	float Width; //0x040C 
	float Height; //0x0410 
	float Depth; //0x0414 
	DWORD Health; //0x0418 
	char Spacer4[20]; //0x041C 
	DWORD HealthMaybe; //0x0430 
	char Spacer5[20]; //0x0434 
	DWORD WeaponClass; //0x0448 
	DWORD Ammo; //0x044C 
	DWORD BAmmo; //0x0450 
	DWORD MaxAmmo; //0x0454 
	DWORD Unknown; //0x0458 
	DWORD BulletCount1; //0x045C 
	DWORD BulletCount2; //0x0460 
	DWORD BulletCount3; //0x0464 
	char Spacer6[20]; //0x0468 
	float Spread001; //0x047C 
	float Spread002; //0x0480 
	float Spread003; //0x0484 
};//Size=0x0488

static char *BoneList[] = {
	"Scene Root",		//[0]
	"M-bone",			//[1]
	"M-bone Pelvis",	//[2]
	"M-bone Spine",		//[3]
	"M-bone Spine1",	//[4]
	"M-bone Neck",		//[5]
	"M-bone Head",		//[6]
	"M-bone L Clavicle",//[7]
	"M-bone L UpperArm",//[8]
	"M-bone L Forearm",	//[9]
	"M-bone L Hand",	//[10]
	"M-bone L Finger0",	//[11]
	"M-bone L Finger1",	//[12]
	"M-bone L Finger2",	//[13]
	"M-bone R Clavicle",//[14]
	"M-bone R UpperArm",//[15]
	"M-bone R Forearm",	//[16]
	"M-bone R Hand",	//[17]
	"M-bone R Finger0",	//[18]
	"M-bone R Finger1",	//[19]
	"M-bone R Finger2",	//[20]
	"M-bone L Thigh",	//[21]
	"M-bone L Calf",	//[22]
	"M-bone L Foot",	//[23]
	"M-bone L Toe0",	//[24]
	"M-bone R Thigh",	//[25]
	"M-bone R Calf",	//[26]
	"M-bone R Foot",	//[27]
	"M-bone R Toe0"		//[28]
};

static char* ranks[] =
{
	"列兵1",
	"列兵2",
	"三等兵",
	"二等兵",
	"一等兵",
	"上等兵1",
	"上等兵2",
	"上等兵3",
	"上等兵4",
	"下士1",
	"下士2",
	"下士3",
	"下士4",
	"下士5",
	"下士6",
	"中士1",
	"中士2",
	"中士3",
	"中士4",
	"中士5",
	"中士6",
	"上士1",
	"上士2",
	"上士3",
	"上士4",
	"上士5",
	"上士6",
	"少尉1",
	"少尉2",
	"少尉3",
	"少尉4",
	"少尉5",
	"少尉6",
	"少尉7",
	"少尉8",
	"中尉1",
	"中尉2",
	"中尉3",
	"中尉4",
	"中尉5",
	"中尉6",
	"中尉7",
	"中尉8",
	"上尉1",
	"上尉2",
	"上尉3",
	"上尉4",
	"上尉5",
	"上尉6",
	"上尉7",
	"上尉8",
	"少校1",
	"少校2",
	"少校3",
	"少校4",
	"少校5",
	"少校6",
	"少校7",
	"少校8",
	"中校1",
	"中校2",
	"中校3",
	"中校4",
	"中校5",
	"中校6",
	"中校7",
	"中校8",
	"上校1",
	"上校2",
	"上校3",
	"上校4",
	"上校5",
	"上校6",
	"上校7",
	"上校8",
	"大校1",
	"大校2",
	"大校3",
	"大校4",
	"大校5",
	"大校6",
	"少将1",
	"少将2",
	"少将3",
	"少将4",
	"少将5",
	"少将6",
	"中将1",
	"中将2",
	"中将3",
	"中将4",
	"中将5",
	"中将6",
	"上将1",
	"上将2",
	"上将3",
	"上将4",
	"上将5",
	"上将6",
	"元帅"
};
