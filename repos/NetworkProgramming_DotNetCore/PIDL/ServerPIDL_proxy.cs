﻿




// Generated by PIDL compiler.
// Do not modify this file, but modify the source .pidl file.

using System;
using System.Net;

namespace ServerPIDL
{
	internal class Proxy:Nettention.Proud.RmiProxy
	{
public bool RegisterRequest(Nettention.Proud.HostID remote,Nettention.Proud.RmiContext rmiContext, string id, string pw, string name)
{
	Nettention.Proud.Message __msg=new Nettention.Proud.Message();
		__msg.SimplePacketMode = core.IsSimplePacketMode();
		Nettention.Proud.RmiID __msgid= Common.RegisterRequest;
		__msg.Write(__msgid);
		Nettention.Proud.Marshaler.Write(__msg, id);
		Nettention.Proud.Marshaler.Write(__msg, pw);
		Nettention.Proud.Marshaler.Write(__msg, name);
		
	Nettention.Proud.HostID[] __list = new Nettention.Proud.HostID[1];
	__list[0] = remote;
		
	return RmiSend(__list,rmiContext,__msg,
		RmiName_RegisterRequest, Common.RegisterRequest);
}

public bool RegisterRequest(Nettention.Proud.HostID[] remotes,Nettention.Proud.RmiContext rmiContext, string id, string pw, string name)
{
	Nettention.Proud.Message __msg=new Nettention.Proud.Message();
__msg.SimplePacketMode = core.IsSimplePacketMode();
Nettention.Proud.RmiID __msgid= Common.RegisterRequest;
__msg.Write(__msgid);
Nettention.Proud.Marshaler.Write(__msg, id);
Nettention.Proud.Marshaler.Write(__msg, pw);
Nettention.Proud.Marshaler.Write(__msg, name);
		
	return RmiSend(remotes,rmiContext,__msg,
		RmiName_RegisterRequest, Common.RegisterRequest);
}
public bool RegisterResponse(Nettention.Proud.HostID remote,Nettention.Proud.RmiContext rmiContext, string id, bool bIsRegister)
{
	Nettention.Proud.Message __msg=new Nettention.Proud.Message();
		__msg.SimplePacketMode = core.IsSimplePacketMode();
		Nettention.Proud.RmiID __msgid= Common.RegisterResponse;
		__msg.Write(__msgid);
		Nettention.Proud.Marshaler.Write(__msg, id);
		Nettention.Proud.Marshaler.Write(__msg, bIsRegister);
		
	Nettention.Proud.HostID[] __list = new Nettention.Proud.HostID[1];
	__list[0] = remote;
		
	return RmiSend(__list,rmiContext,__msg,
		RmiName_RegisterResponse, Common.RegisterResponse);
}

public bool RegisterResponse(Nettention.Proud.HostID[] remotes,Nettention.Proud.RmiContext rmiContext, string id, bool bIsRegister)
{
	Nettention.Proud.Message __msg=new Nettention.Proud.Message();
__msg.SimplePacketMode = core.IsSimplePacketMode();
Nettention.Proud.RmiID __msgid= Common.RegisterResponse;
__msg.Write(__msgid);
Nettention.Proud.Marshaler.Write(__msg, id);
Nettention.Proud.Marshaler.Write(__msg, bIsRegister);
		
	return RmiSend(remotes,rmiContext,__msg,
		RmiName_RegisterResponse, Common.RegisterResponse);
}
public bool LoginRequest(Nettention.Proud.HostID remote,Nettention.Proud.RmiContext rmiContext, string id, string pw)
{
	Nettention.Proud.Message __msg=new Nettention.Proud.Message();
		__msg.SimplePacketMode = core.IsSimplePacketMode();
		Nettention.Proud.RmiID __msgid= Common.LoginRequest;
		__msg.Write(__msgid);
		Nettention.Proud.Marshaler.Write(__msg, id);
		Nettention.Proud.Marshaler.Write(__msg, pw);
		
	Nettention.Proud.HostID[] __list = new Nettention.Proud.HostID[1];
	__list[0] = remote;
		
	return RmiSend(__list,rmiContext,__msg,
		RmiName_LoginRequest, Common.LoginRequest);
}

public bool LoginRequest(Nettention.Proud.HostID[] remotes,Nettention.Proud.RmiContext rmiContext, string id, string pw)
{
	Nettention.Proud.Message __msg=new Nettention.Proud.Message();
__msg.SimplePacketMode = core.IsSimplePacketMode();
Nettention.Proud.RmiID __msgid= Common.LoginRequest;
__msg.Write(__msgid);
Nettention.Proud.Marshaler.Write(__msg, id);
Nettention.Proud.Marshaler.Write(__msg, pw);
		
	return RmiSend(remotes,rmiContext,__msg,
		RmiName_LoginRequest, Common.LoginRequest);
}
public bool LoginResponse(Nettention.Proud.HostID remote,Nettention.Proud.RmiContext rmiContext, string id, bool bIsLogin)
{
	Nettention.Proud.Message __msg=new Nettention.Proud.Message();
		__msg.SimplePacketMode = core.IsSimplePacketMode();
		Nettention.Proud.RmiID __msgid= Common.LoginResponse;
		__msg.Write(__msgid);
		Nettention.Proud.Marshaler.Write(__msg, id);
		Nettention.Proud.Marshaler.Write(__msg, bIsLogin);
		
	Nettention.Proud.HostID[] __list = new Nettention.Proud.HostID[1];
	__list[0] = remote;
		
	return RmiSend(__list,rmiContext,__msg,
		RmiName_LoginResponse, Common.LoginResponse);
}

public bool LoginResponse(Nettention.Proud.HostID[] remotes,Nettention.Proud.RmiContext rmiContext, string id, bool bIsLogin)
{
	Nettention.Proud.Message __msg=new Nettention.Proud.Message();
__msg.SimplePacketMode = core.IsSimplePacketMode();
Nettention.Proud.RmiID __msgid= Common.LoginResponse;
__msg.Write(__msgid);
Nettention.Proud.Marshaler.Write(__msg, id);
Nettention.Proud.Marshaler.Write(__msg, bIsLogin);
		
	return RmiSend(remotes,rmiContext,__msg,
		RmiName_LoginResponse, Common.LoginResponse);
}
public bool CreatePlayer(Nettention.Proud.HostID remote,Nettention.Proud.RmiContext rmiContext, string id, string tag)
{
	Nettention.Proud.Message __msg=new Nettention.Proud.Message();
		__msg.SimplePacketMode = core.IsSimplePacketMode();
		Nettention.Proud.RmiID __msgid= Common.CreatePlayer;
		__msg.Write(__msgid);
		Nettention.Proud.Marshaler.Write(__msg, id);
		Nettention.Proud.Marshaler.Write(__msg, tag);
		
	Nettention.Proud.HostID[] __list = new Nettention.Proud.HostID[1];
	__list[0] = remote;
		
	return RmiSend(__list,rmiContext,__msg,
		RmiName_CreatePlayer, Common.CreatePlayer);
}

public bool CreatePlayer(Nettention.Proud.HostID[] remotes,Nettention.Proud.RmiContext rmiContext, string id, string tag)
{
	Nettention.Proud.Message __msg=new Nettention.Proud.Message();
__msg.SimplePacketMode = core.IsSimplePacketMode();
Nettention.Proud.RmiID __msgid= Common.CreatePlayer;
__msg.Write(__msgid);
Nettention.Proud.Marshaler.Write(__msg, id);
Nettention.Proud.Marshaler.Write(__msg, tag);
		
	return RmiSend(remotes,rmiContext,__msg,
		RmiName_CreatePlayer, Common.CreatePlayer);
}
public bool GetPlayerRequest(Nettention.Proud.HostID remote,Nettention.Proud.RmiContext rmiContext, string id, string tag)
{
	Nettention.Proud.Message __msg=new Nettention.Proud.Message();
		__msg.SimplePacketMode = core.IsSimplePacketMode();
		Nettention.Proud.RmiID __msgid= Common.GetPlayerRequest;
		__msg.Write(__msgid);
		Nettention.Proud.Marshaler.Write(__msg, id);
		Nettention.Proud.Marshaler.Write(__msg, tag);
		
	Nettention.Proud.HostID[] __list = new Nettention.Proud.HostID[1];
	__list[0] = remote;
		
	return RmiSend(__list,rmiContext,__msg,
		RmiName_GetPlayerRequest, Common.GetPlayerRequest);
}

public bool GetPlayerRequest(Nettention.Proud.HostID[] remotes,Nettention.Proud.RmiContext rmiContext, string id, string tag)
{
	Nettention.Proud.Message __msg=new Nettention.Proud.Message();
__msg.SimplePacketMode = core.IsSimplePacketMode();
Nettention.Proud.RmiID __msgid= Common.GetPlayerRequest;
__msg.Write(__msgid);
Nettention.Proud.Marshaler.Write(__msg, id);
Nettention.Proud.Marshaler.Write(__msg, tag);
		
	return RmiSend(remotes,rmiContext,__msg,
		RmiName_GetPlayerRequest, Common.GetPlayerRequest);
}
public bool GetPlayerResponse(Nettention.Proud.HostID remote,Nettention.Proud.RmiContext rmiContext, string id, string tag, int bullet, int hp, int life, bool bIsSucess)
{
	Nettention.Proud.Message __msg=new Nettention.Proud.Message();
		__msg.SimplePacketMode = core.IsSimplePacketMode();
		Nettention.Proud.RmiID __msgid= Common.GetPlayerResponse;
		__msg.Write(__msgid);
		Nettention.Proud.Marshaler.Write(__msg, id);
		Nettention.Proud.Marshaler.Write(__msg, tag);
		Nettention.Proud.Marshaler.Write(__msg, bullet);
		Nettention.Proud.Marshaler.Write(__msg, hp);
		Nettention.Proud.Marshaler.Write(__msg, life);
		Nettention.Proud.Marshaler.Write(__msg, bIsSucess);
		
	Nettention.Proud.HostID[] __list = new Nettention.Proud.HostID[1];
	__list[0] = remote;
		
	return RmiSend(__list,rmiContext,__msg,
		RmiName_GetPlayerResponse, Common.GetPlayerResponse);
}

public bool GetPlayerResponse(Nettention.Proud.HostID[] remotes,Nettention.Proud.RmiContext rmiContext, string id, string tag, int bullet, int hp, int life, bool bIsSucess)
{
	Nettention.Proud.Message __msg=new Nettention.Proud.Message();
__msg.SimplePacketMode = core.IsSimplePacketMode();
Nettention.Proud.RmiID __msgid= Common.GetPlayerResponse;
__msg.Write(__msgid);
Nettention.Proud.Marshaler.Write(__msg, id);
Nettention.Proud.Marshaler.Write(__msg, tag);
Nettention.Proud.Marshaler.Write(__msg, bullet);
Nettention.Proud.Marshaler.Write(__msg, hp);
Nettention.Proud.Marshaler.Write(__msg, life);
Nettention.Proud.Marshaler.Write(__msg, bIsSucess);
		
	return RmiSend(remotes,rmiContext,__msg,
		RmiName_GetPlayerResponse, Common.GetPlayerResponse);
}
#if USE_RMI_NAME_STRING
// RMI name declaration.
// It is the unique pointer that indicates RMI name such as RMI profiler.
public const string RmiName_RegisterRequest="RegisterRequest";
public const string RmiName_RegisterResponse="RegisterResponse";
public const string RmiName_LoginRequest="LoginRequest";
public const string RmiName_LoginResponse="LoginResponse";
public const string RmiName_CreatePlayer="CreatePlayer";
public const string RmiName_GetPlayerRequest="GetPlayerRequest";
public const string RmiName_GetPlayerResponse="GetPlayerResponse";
       
public const string RmiName_First = RmiName_RegisterRequest;
#else
// RMI name declaration.
// It is the unique pointer that indicates RMI name such as RMI profiler.
public const string RmiName_RegisterRequest="";
public const string RmiName_RegisterResponse="";
public const string RmiName_LoginRequest="";
public const string RmiName_LoginResponse="";
public const string RmiName_CreatePlayer="";
public const string RmiName_GetPlayerRequest="";
public const string RmiName_GetPlayerResponse="";
       
public const string RmiName_First = "";
#endif
		public override Nettention.Proud.RmiID[] GetRmiIDList() { return Common.RmiIDList; } 
	}
}

