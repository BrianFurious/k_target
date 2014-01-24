/*

     ///////////////////////////////////////////////////
	//											 	 //
   //	k_target.inc  v1.0                          //
  //	Created by kadaradam on 2014. 01. 21.      //
 //		  										  //
///////////////////////////////////////////////////

Functions:
native CreateTarget(Float:Tx, Float:Ty, Float:Tz, Float:FacingAngle);
native SetTargetFacingAngle(targetid, Float:FacingAngle);
native SetTargetPos(targetid, Float:Tx, Float:Ty, Float:Tz);
native GetTargetPos(targetid, &Float:Tx, &Float:Ty, &Float:Tz);
native MoveTarget(targetid, direction, Float:distance);
native MoveTargetEx(targetid, Float:X, Float:Y, Float:Z, Float:Speed, Float:FacingAngle);
native DestroyBodyPart(targetid, bodypart); // NOTE: If you destroy the torso, all body parts will be destroyed.
native DestroyTarget(targetid);
native IsValidTarget(targetid);

CallBacks:
public OnTargetMoved(targetid)
public OnPlayerShotTarget(playerid, targetid, bodypart, Float:Tx, Float:Ty, Float:Tz)


*/

#include <a_samp>

#if !defined BULLET_HIT_TYPE_NONE
        #error "Please update your samp version to 0.3Z!"
#endif

#define MOVE_FORWARD    	0
#define MOVE_BACKWARD   	1
#define MOVE_LEFT   		2
#define MOVE_RIGHT   		3

#define TORSO      			3
#define LEFT_ARM   			5
#define RIGHT_ARM 			6
#define RIGHT_LEG   		8
#define LEFT_LEG   			7
#define HEAD   				9

forward OnTargetMoved(targetid);
forward OnPlayerShotTarget(playerid, targetid, bodypart, Float:Tx, Float:Ty, Float:Tz);

#define MAX_TARGETS 50

enum target_ENUM
{
	tID,
	main_object_id,
	TOSlot[5]
}
new Targets[MAX_TARGETS][target_ENUM];


stock CreateTarget(Float:Tx, Float:Ty, Float:Tz, Float:FacingAngle)
{
	new ID = GetFreeTargetID();
	if(ID == -1)
	{
	    printf(" < k_target.inc > : There is no empty slot to create the object. ");
	    printf(" < k_target.inc > : Please increase the \"MAX_TARGETS\" size! ");
		return 1;
	}

	Targets[ID][tID] = ID;

    Targets[ID][main_object_id] = CreateObject(2907, Tx, Ty, Tz, 												0.000000, 0.000000, 0.000000);

	Targets[ID][TOSlot][0] = CreateObject(2908,0,0,-1000,0,0,0);
	AttachObjectToObject(Targets[ID][TOSlot][0], Targets[ID][main_object_id], 0.049999, -0.539999, 0.049999,	0.000000, 102.000000, 0.500000);


	Targets[ID][TOSlot][1] = CreateObject(2906,0,0,-1000,0,0,0);
	AttachObjectToObject(Targets[ID][TOSlot][1], Targets[ID][main_object_id], 0.169999, -0.180000, 0.149999, 	55.500000, -52.500000, 0.000000);


	Targets[ID][TOSlot][2] = CreateObject(2906,0,0,-1000,0,0,0);
	AttachObjectToObject(Targets[ID][TOSlot][2], Targets[ID][main_object_id], -0.170000, -0.180000, 0.149999, 	62.000000, 200.000000, 0.000000);


	Targets[ID][TOSlot][3] = CreateObject(2905,0,0,-1000,0,0,0);
	AttachObjectToObject(Targets[ID][TOSlot][3], Targets[ID][main_object_id], -0.079999, 0.719999, 0.029999, 	0.000000, 89.000000, 0.000000);


	Targets[ID][TOSlot][4] = CreateObject(2905,0,0,-1000,0,0,0);
	AttachObjectToObject(Targets[ID][TOSlot][4], Targets[ID][main_object_id], 0.119999,0.739999, 0.059999,   	0.000000, 88.000000, 0.000000);

	SetObjectRot(Targets[ID][main_object_id], -90, 0, FacingAngle);
	return ID;
}
stock SetTargetFacingAngle(targetid, Float:FacingAngle)
{
    SetObjectRot(Targets[targetid][main_object_id], -90, 0, FacingAngle);
}
stock SetTargetPos(targetid, Float:Tx, Float:Ty, Float:Tz)
{
	SetObjectPos(Targets[targetid][main_object_id], Tx, Ty, Tz);
}
stock GetTargetPos(targetid, &Float:Tx, &Float:Ty, &Float:Tz)
{
	GetObjectPos(Targets[targetid][main_object_id], Tx, Ty, Tz);
}
stock MoveTarget(targetid, direction, Float:distance, Float:speed)
{
	if(direction < MOVE_FORWARD || direction > MOVE_RIGHT )
	{
	    print(" < k_target.inc > : MoveTarget() Invalid direction ID!");
	    return 1;
	}

	new Float:Tx, Float:Ty, Float:Tz;
	new Float:TRZ;
    GetObjectPos(Targets[targetid][main_object_id], Tx, Ty, Tz);
    GetObjectRot(Targets[targetid][main_object_id], TRZ, TRZ, TRZ);

    switch(direction)
    {
        case MOVE_FORWARD:
        {
	    	Tx += (distance * floatsin(-TRZ, degrees));
    		Ty += (distance * floatcos(-TRZ, degrees));
    	}
    	case MOVE_BACKWARD:
        {
	    	Tx -= (distance * floatsin(-TRZ, degrees));
	    	Ty -= (distance * floatcos(-TRZ, degrees));
    	}
    	case MOVE_LEFT:
        {
	    	Tx -= (distance * floatsin(-TRZ + 90, degrees));
	    	Ty -= (distance * floatcos(-TRZ + 90, degrees));
    	}
    	case MOVE_RIGHT:
        {
	    	Tx -= (distance * floatsin(-TRZ - 90, degrees));
	    	Ty -= (distance * floatcos(-TRZ - 90, degrees));
    	}
   	}

    return MoveObject(Targets[targetid][main_object_id], Tx, Ty, Tz, speed);
}
stock MoveTargetEx(targetid, Float:X, Float:Y, Float:Z, Float:Speed, Float:FacingAngle)
{
	return MoveObject(Targets[targetid][main_object_id], X, Y, Z, Speed, -90, 0, FacingAngle);
}
stock DestroyBodyPart(targetid, bodypart)
{
    if(bodypart < TORSO || bodypart > HEAD || bodypart == 4 )
	{
	    print(" < k_target.inc > : DestroyBodyPart() Invalid bodypart ID!");
	    return 1;
	}

	switch(bodypart)
	{
 		case HEAD: DestroyObject(Targets[targetid][TOSlot][0]), Targets[targetid][TOSlot][0] = 0;
 		case TORSO: DestroyTarget(targetid);
    	case RIGHT_ARM: DestroyObject(Targets[targetid][TOSlot][1]), Targets[targetid][TOSlot][1] = 0;
    	case LEFT_ARM: DestroyObject(Targets[targetid][TOSlot][2]),Targets[targetid][TOSlot][2] = 0;
    	case LEFT_LEG: DestroyObject(Targets[targetid][TOSlot][3]),Targets[targetid][TOSlot][3] = 0;
    	case RIGHT_LEG: DestroyObject(Targets[targetid][TOSlot][4]), Targets[targetid][TOSlot][4] = 0;
	}
	return 1;
}
stock DestroyTarget(targetid)
{
    DestroyObject(Targets[targetid][main_object_id]);
    DestroyObject(Targets[targetid][TOSlot][0]);
    DestroyObject(Targets[targetid][TOSlot][1]);
    DestroyObject(Targets[targetid][TOSlot][2]);
    DestroyObject(Targets[targetid][TOSlot][3]);
    DestroyObject(Targets[targetid][TOSlot][4]);

    Targets[targetid][tID] = 0;
    Targets[targetid][main_object_id] = 0;
    Targets[targetid][TOSlot][0] = 0;
    Targets[targetid][TOSlot][1] = 0;
    Targets[targetid][TOSlot][2] = 0;
    Targets[targetid][TOSlot][3] = 0;
    Targets[targetid][TOSlot][4] = 0;
}
stock IsValidTarget(targetid)
{
	if(targetid > MAX_TARGETS) return 0;
    if(Targets[targetid][main_object_id] != 0) return 1;
    return 0;
}
stock GetFreeTargetID()
{
    for(new i; i < MAX_TARGETS; i++)
    {
        if( Targets[i][main_object_id] == 0 ) return i;
    }
    return -1;
}

public OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
    if(hittype == 3)
	{
	    for(new i; i < MAX_TARGETS; i++)
	    {
			if(hitid == Targets[i][main_object_id])
	 		{
                CallLocalFunction("OnPlayerShotTarget", "iiifff", playerid, Targets[i][tID], TORSO, fX, fY, fZ);
			}
			if(hitid == Targets[i][TOSlot][0])
			{
			    CallLocalFunction("OnPlayerShotTarget", "iiifff", playerid, Targets[i][tID], HEAD, fX, fY, fZ);
			}
			if(hitid == Targets[i][TOSlot][1])
			{
			    CallLocalFunction("OnPlayerShotTarget", "iiifff", playerid, Targets[i][tID], RIGHT_ARM, fX, fY, fZ);
			}
			if(hitid == Targets[i][TOSlot][2])
			{
			    CallLocalFunction("OnPlayerShotTarget", "iiifff", playerid, Targets[i][tID], LEFT_ARM, fX, fY, fZ);
			}
			if(hitid == Targets[i][TOSlot][3])
			{
			    CallLocalFunction("OnPlayerShotTarget", "iiifff", playerid, Targets[i][tID], LEFT_LEG, fX, fY, fZ);
			}
			if(hitid == Targets[i][TOSlot][4])
			{
			    CallLocalFunction("OnPlayerShotTarget", "iiifff", playerid, Targets[i][tID], RIGHT_LEG, fX, fY, fZ);
			}
		}
	}

	#if defined k_target_OnPlayerWeaponShot
        k_target_OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ);
    #endif

	return 1;
}

#if defined _ALS_OnPlayerWeaponShot
    #undef OnPlayerWeaponShot
#else
    #define _ALS_OnPlayerWeaponShot
#endif

#define OnPlayerWeaponShot k_target_OnPlayerWeaponShot

#if defined k_target_OnPlayerWeaponShot
    forward k_target_OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ);
#endif

public OnObjectMoved(objectid)
{
    for(new i; i < MAX_TARGETS; i++)
    {
		if(objectid == Targets[i][main_object_id])
 		{
	    	CallLocalFunction("OnTargetMoved", "i", Targets[i][tID]);
 		}
	}

	#if defined k_target_OnObjectMoved
        k_target_OnObjectMoved(objectid);
    #endif

    return 1;
}

#if defined _ALS_OnObjectMoved
    #undef OnObjectMoved
#else
    #define _ALS_OnObjectMoved
#endif

#define OnObjectMoved k_target_OnObjectMoved

#if defined k_target_OnObjectMoved
    forward k_target_OnObjectMoved(objectid);
#endif
<<<<<<< HEAD
=======

>>>>>>> Added example filterscript
