#include <a_samp>
#include <zcmd>
#include <sscanf2>

#include <k_target>

new test;

public OnFilterScriptInit()
{
	print(" < k_target.inc > by kadaradam: Example filterscript loaded");

	test = CreateTarget(0, 10.0, 5.0, 90);
	
	#define SOLDIERS 			30
	#define ROW_PER_SOLDIER 	5
	#define FACING_ANGLE        0
	#define T_POS_X             5.0
	#define T_POS_Y             5.0
	#define T_POS_Z             3.2
	#define DISTANCE            2.0
	
	new
		count = 0,
		row_count = 0,
		Float:t_pos_x = T_POS_X,
		Float:t_pos_y = T_POS_Y
	;

	
	for(new i; i < SOLDIERS - 1; i++)
    {
        count++;
   		
        if(count != ROW_PER_SOLDIER)
        {
            t_pos_x -= (DISTANCE * floatsin(FACING_ANGLE + 90, degrees));
	   		t_pos_y -= (DISTANCE * floatcos(FACING_ANGLE + 90, degrees));
	   		CreateTarget(t_pos_x, t_pos_y, T_POS_Z, FACING_ANGLE);
        }
        else
        {
            if(row_count == 0)
            {
                CreateTarget(T_POS_X, T_POS_Y, T_POS_Z, FACING_ANGLE);
            }
            
	   		count = 0;
			row_count ++;
        
			t_pos_x = T_POS_X;
            t_pos_y = T_POS_Y;

            t_pos_x -= ( (row_count * DISTANCE) * floatsin(FACING_ANGLE, degrees));
	   		t_pos_y -= ( (row_count * DISTANCE) * floatcos(FACING_ANGLE, degrees));
	   		CreateTarget(t_pos_x, t_pos_y, T_POS_Z, FACING_ANGLE);
		}
    }
	return 1;
}

public OnFilterScriptExit()
{
    DestroyTarget(test);
	return 1;
}

public OnPlayerShotTarget(playerid, targetid, bodypart, Float:Tx, Float:Ty, Float:Tz)
{
	printf("playerid: %i, targetid: %i, bodypart: %i, Float:Tx: %f, Float:Ty: %f, Float:Tz: %f", playerid, targetid, bodypart, Float:Tx, Float:Ty, Float:Tz);

	switch(bodypart)
	{
    	case HEAD:      SendClientMessage(playerid, -1, "HEAD");
		case TORSO:     SendClientMessage(playerid, -1, "TORSO");
		case LEFT_ARM:  SendClientMessage(playerid, -1, "LEFT_ARM");
		case RIGHT_ARM: SendClientMessage(playerid, -1, "RIGHT_ARM");
		case RIGHT_LEG: SendClientMessage(playerid, -1, "RIGHT_LEG");
		case LEFT_LEG: SendClientMessage(playerid, -1, "LEFT_LEG");
	}

	DestroyBodyPart(targetid, bodypart);
	return 1;
}
COMMAND:kcmds(playerid, params[])
{
    SendClientMessage(playerid, -1, " < k_target.inc > Commands: /createt | /setangle | /destroy | /destroybody | /move");
	return 1;
}
COMMAND:createt(playerid, params[])
{
	new Float:pos[3], Float:Angle;
	GetPlayerPos(playerid, pos[0], pos[1], pos[2]);
	GetPlayerFacingAngle(playerid, Angle);

	new ID = CreateTarget(pos[0], pos[1], pos[2], Angle);

	new str[39];
	format(str, 39, "Target created! ID: %i", ID);
	SendClientMessage(playerid, -1, str);
	return 1;
}
COMMAND:setangle(playerid, params[])
{
	new target, Float: rot;
	if(sscanf(params, "if", target, rot)) return SendClientMessage(playerid, -1, "Usage: /setrot [targetid] [Float:facingangle]");
	if(!IsValidTarget(target)) return SendClientMessage(playerid, -1, "Invalid target id!");
	
	SetTargetFacingAngle(target, rot );
	SendClientMessage(playerid, -1, "");
	return 1;
}
COMMAND:destroy(playerid, params[])
{
	if(isnull(params)) return SendClientMessage(playerid, -1, "Usage: /destroy [targetid]");
	new target = strval(params);
    if(!IsValidTarget(target)) return SendClientMessage(playerid, -1, "Invalid target id!");
	DestroyTarget( target );
	SendClientMessage(playerid, -1, "Target destroyed!");
	return 1;
}
COMMAND:destroybody(playerid, params[])
{
    new target, bodypart;
    if(sscanf(params, "ii", target, bodypart)) return SendClientMessage(playerid, -1, "Usage: /destroybody [targetid] [bodypart: TORSO = 3, LEFT_ARM = 5, RIGHT_ARM = 6, RIGHT_LEG = 8, LEFT_LEG = 7, HEAD = 9]");
    if(!IsValidTarget(target)) return SendClientMessage(playerid, -1, "Invalid target id!");
    
	DestroyBodyPart(target, bodypart);
	SendClientMessage(playerid, -1, "Bodypart destroyed");
	return 1;
}
CMD:move(playerid, params[])
{
	new target, direction, Float:distance, Float:speed;
	if(sscanf(params, "iiff", target, direction, distance, speed)) return SendClientMessage(playerid, -1, "Usage: /move [targetid] [direction: MOVE_FORWARD = 0, MOVE_BACKWARD = 1, MOVE_LEFT = 2, MOVE_RIGHT = 3] [Float:distance] [Float:speed]");
  	if(!IsValidTarget(target)) return SendClientMessage(playerid, -1, "Invalid target id!");

	SendClientMessage(playerid, -1, "Object moved!");
    MoveTarget(target, direction, distance, speed);
    return 1;
}
