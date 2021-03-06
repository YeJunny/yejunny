﻿




// Generated by PIDL compiler.
// Do not modify this file, but modify the source .pidl file.

using System;
using System.Net;	     

namespace ServerPIDL
{
	internal class Stub:Nettention.Proud.RmiStub
	{
public AfterRmiInvocationDelegate AfterRmiInvocation = delegate(Nettention.Proud.AfterRmiSummary summary) {};
public BeforeRmiInvocationDelegate BeforeRmiInvocation = delegate(Nettention.Proud.BeforeRmiSummary summary) {};

		public delegate bool RegisterRequestDelegate(Nettention.Proud.HostID remote,Nettention.Proud.RmiContext rmiContext, string id, string pw, string name);  
		public RegisterRequestDelegate RegisterRequest = delegate(Nettention.Proud.HostID remote,Nettention.Proud.RmiContext rmiContext, string id, string pw, string name)
		{ 
			return false;
		};
		public delegate bool RegisterResponseDelegate(Nettention.Proud.HostID remote,Nettention.Proud.RmiContext rmiContext, string id, bool bIsRegister);  
		public RegisterResponseDelegate RegisterResponse = delegate(Nettention.Proud.HostID remote,Nettention.Proud.RmiContext rmiContext, string id, bool bIsRegister)
		{ 
			return false;
		};
		public delegate bool LoginRequestDelegate(Nettention.Proud.HostID remote,Nettention.Proud.RmiContext rmiContext, string id, string pw);  
		public LoginRequestDelegate LoginRequest = delegate(Nettention.Proud.HostID remote,Nettention.Proud.RmiContext rmiContext, string id, string pw)
		{ 
			return false;
		};
		public delegate bool LoginResponseDelegate(Nettention.Proud.HostID remote,Nettention.Proud.RmiContext rmiContext, string id, bool bIsLogin);  
		public LoginResponseDelegate LoginResponse = delegate(Nettention.Proud.HostID remote,Nettention.Proud.RmiContext rmiContext, string id, bool bIsLogin)
		{ 
			return false;
		};
		public delegate bool CreatePlayerDelegate(Nettention.Proud.HostID remote,Nettention.Proud.RmiContext rmiContext, string id, string tag);  
		public CreatePlayerDelegate CreatePlayer = delegate(Nettention.Proud.HostID remote,Nettention.Proud.RmiContext rmiContext, string id, string tag)
		{ 
			return false;
		};
		public delegate bool GetPlayerRequestDelegate(Nettention.Proud.HostID remote,Nettention.Proud.RmiContext rmiContext, string id, string tag);  
		public GetPlayerRequestDelegate GetPlayerRequest = delegate(Nettention.Proud.HostID remote,Nettention.Proud.RmiContext rmiContext, string id, string tag)
		{ 
			return false;
		};
		public delegate bool GetPlayerResponseDelegate(Nettention.Proud.HostID remote,Nettention.Proud.RmiContext rmiContext, string id, string tag, int bullet, int hp, int life, bool bIsSucess);  
		public GetPlayerResponseDelegate GetPlayerResponse = delegate(Nettention.Proud.HostID remote,Nettention.Proud.RmiContext rmiContext, string id, string tag, int bullet, int hp, int life, bool bIsSucess)
		{ 
			return false;
		};
	public override bool ProcessReceivedMessage(Nettention.Proud.ReceivedMessage pa, Object hostTag) 
	{
		Nettention.Proud.HostID remote=pa.RemoteHostID;
		if(remote==Nettention.Proud.HostID.HostID_None)
		{
			ShowUnknownHostIDWarning(remote);
		}

		Nettention.Proud.Message __msg=pa.ReadOnlyMessage;
		int orgReadOffset = __msg.ReadOffset;
        Nettention.Proud.RmiID __rmiID = Nettention.Proud.RmiID.RmiID_None;
        if (!__msg.Read( out __rmiID))
            goto __fail;
					
		switch(__rmiID)
		{
        case Common.RegisterRequest:
            ProcessReceivedMessage_RegisterRequest(__msg, pa, hostTag, remote);
            break;
        case Common.RegisterResponse:
            ProcessReceivedMessage_RegisterResponse(__msg, pa, hostTag, remote);
            break;
        case Common.LoginRequest:
            ProcessReceivedMessage_LoginRequest(__msg, pa, hostTag, remote);
            break;
        case Common.LoginResponse:
            ProcessReceivedMessage_LoginResponse(__msg, pa, hostTag, remote);
            break;
        case Common.CreatePlayer:
            ProcessReceivedMessage_CreatePlayer(__msg, pa, hostTag, remote);
            break;
        case Common.GetPlayerRequest:
            ProcessReceivedMessage_GetPlayerRequest(__msg, pa, hostTag, remote);
            break;
        case Common.GetPlayerResponse:
            ProcessReceivedMessage_GetPlayerResponse(__msg, pa, hostTag, remote);
            break;
		default:
			 goto __fail;
		}
		return true;
__fail:
	  {
			__msg.ReadOffset = orgReadOffset;
			return false;
	  }
	}
    void ProcessReceivedMessage_RegisterRequest(Nettention.Proud.Message __msg, Nettention.Proud.ReceivedMessage pa, Object hostTag, Nettention.Proud.HostID remote)
    {
        Nettention.Proud.RmiContext ctx = new Nettention.Proud.RmiContext();
        ctx.sentFrom=pa.RemoteHostID;
        ctx.relayed=pa.IsRelayed;
        ctx.hostTag=hostTag;
        ctx.encryptMode = pa.EncryptMode;
        ctx.compressMode = pa.CompressMode;

        string id; Nettention.Proud.Marshaler.Read(__msg,out id);	
string pw; Nettention.Proud.Marshaler.Read(__msg,out pw);	
string name; Nettention.Proud.Marshaler.Read(__msg,out name);	
core.PostCheckReadMessage(__msg, RmiName_RegisterRequest);
        if(enableNotifyCallFromStub==true)
        {
        string parameterString = "";
        parameterString+=id.ToString()+",";
parameterString+=pw.ToString()+",";
parameterString+=name.ToString()+",";
        NotifyCallFromStub(Common.RegisterRequest, RmiName_RegisterRequest,parameterString);
        }

        if(enableStubProfiling)
        {
        Nettention.Proud.BeforeRmiSummary summary = new Nettention.Proud.BeforeRmiSummary();
        summary.rmiID = Common.RegisterRequest;
        summary.rmiName = RmiName_RegisterRequest;
        summary.hostID = remote;
        summary.hostTag = hostTag;
        BeforeRmiInvocation(summary);
        }

        long t0 = Nettention.Proud.PreciseCurrentTime.GetTimeMs();

        // Call this method.
        bool __ret =RegisterRequest (remote,ctx , id, pw, name );

        if(__ret==false)
        {
        // Error: RMI function that a user did not create has been called. 
        core.ShowNotImplementedRmiWarning(RmiName_RegisterRequest);
        }

        if(enableStubProfiling)
        {
        Nettention.Proud.AfterRmiSummary summary = new Nettention.Proud.AfterRmiSummary();
        summary.rmiID = Common.RegisterRequest;
        summary.rmiName = RmiName_RegisterRequest;
        summary.hostID = remote;
        summary.hostTag = hostTag;
        summary.elapsedTime = Nettention.Proud.PreciseCurrentTime.GetTimeMs()-t0;
        AfterRmiInvocation(summary);
        }
    }
    void ProcessReceivedMessage_RegisterResponse(Nettention.Proud.Message __msg, Nettention.Proud.ReceivedMessage pa, Object hostTag, Nettention.Proud.HostID remote)
    {
        Nettention.Proud.RmiContext ctx = new Nettention.Proud.RmiContext();
        ctx.sentFrom=pa.RemoteHostID;
        ctx.relayed=pa.IsRelayed;
        ctx.hostTag=hostTag;
        ctx.encryptMode = pa.EncryptMode;
        ctx.compressMode = pa.CompressMode;

        string id; Nettention.Proud.Marshaler.Read(__msg,out id);	
bool bIsRegister; Nettention.Proud.Marshaler.Read(__msg,out bIsRegister);	
core.PostCheckReadMessage(__msg, RmiName_RegisterResponse);
        if(enableNotifyCallFromStub==true)
        {
        string parameterString = "";
        parameterString+=id.ToString()+",";
parameterString+=bIsRegister.ToString()+",";
        NotifyCallFromStub(Common.RegisterResponse, RmiName_RegisterResponse,parameterString);
        }

        if(enableStubProfiling)
        {
        Nettention.Proud.BeforeRmiSummary summary = new Nettention.Proud.BeforeRmiSummary();
        summary.rmiID = Common.RegisterResponse;
        summary.rmiName = RmiName_RegisterResponse;
        summary.hostID = remote;
        summary.hostTag = hostTag;
        BeforeRmiInvocation(summary);
        }

        long t0 = Nettention.Proud.PreciseCurrentTime.GetTimeMs();

        // Call this method.
        bool __ret =RegisterResponse (remote,ctx , id, bIsRegister );

        if(__ret==false)
        {
        // Error: RMI function that a user did not create has been called. 
        core.ShowNotImplementedRmiWarning(RmiName_RegisterResponse);
        }

        if(enableStubProfiling)
        {
        Nettention.Proud.AfterRmiSummary summary = new Nettention.Proud.AfterRmiSummary();
        summary.rmiID = Common.RegisterResponse;
        summary.rmiName = RmiName_RegisterResponse;
        summary.hostID = remote;
        summary.hostTag = hostTag;
        summary.elapsedTime = Nettention.Proud.PreciseCurrentTime.GetTimeMs()-t0;
        AfterRmiInvocation(summary);
        }
    }
    void ProcessReceivedMessage_LoginRequest(Nettention.Proud.Message __msg, Nettention.Proud.ReceivedMessage pa, Object hostTag, Nettention.Proud.HostID remote)
    {
        Nettention.Proud.RmiContext ctx = new Nettention.Proud.RmiContext();
        ctx.sentFrom=pa.RemoteHostID;
        ctx.relayed=pa.IsRelayed;
        ctx.hostTag=hostTag;
        ctx.encryptMode = pa.EncryptMode;
        ctx.compressMode = pa.CompressMode;

        string id; Nettention.Proud.Marshaler.Read(__msg,out id);	
string pw; Nettention.Proud.Marshaler.Read(__msg,out pw);	
core.PostCheckReadMessage(__msg, RmiName_LoginRequest);
        if(enableNotifyCallFromStub==true)
        {
        string parameterString = "";
        parameterString+=id.ToString()+",";
parameterString+=pw.ToString()+",";
        NotifyCallFromStub(Common.LoginRequest, RmiName_LoginRequest,parameterString);
        }

        if(enableStubProfiling)
        {
        Nettention.Proud.BeforeRmiSummary summary = new Nettention.Proud.BeforeRmiSummary();
        summary.rmiID = Common.LoginRequest;
        summary.rmiName = RmiName_LoginRequest;
        summary.hostID = remote;
        summary.hostTag = hostTag;
        BeforeRmiInvocation(summary);
        }

        long t0 = Nettention.Proud.PreciseCurrentTime.GetTimeMs();

        // Call this method.
        bool __ret =LoginRequest (remote,ctx , id, pw );

        if(__ret==false)
        {
        // Error: RMI function that a user did not create has been called. 
        core.ShowNotImplementedRmiWarning(RmiName_LoginRequest);
        }

        if(enableStubProfiling)
        {
        Nettention.Proud.AfterRmiSummary summary = new Nettention.Proud.AfterRmiSummary();
        summary.rmiID = Common.LoginRequest;
        summary.rmiName = RmiName_LoginRequest;
        summary.hostID = remote;
        summary.hostTag = hostTag;
        summary.elapsedTime = Nettention.Proud.PreciseCurrentTime.GetTimeMs()-t0;
        AfterRmiInvocation(summary);
        }
    }
    void ProcessReceivedMessage_LoginResponse(Nettention.Proud.Message __msg, Nettention.Proud.ReceivedMessage pa, Object hostTag, Nettention.Proud.HostID remote)
    {
        Nettention.Proud.RmiContext ctx = new Nettention.Proud.RmiContext();
        ctx.sentFrom=pa.RemoteHostID;
        ctx.relayed=pa.IsRelayed;
        ctx.hostTag=hostTag;
        ctx.encryptMode = pa.EncryptMode;
        ctx.compressMode = pa.CompressMode;

        string id; Nettention.Proud.Marshaler.Read(__msg,out id);	
bool bIsLogin; Nettention.Proud.Marshaler.Read(__msg,out bIsLogin);	
core.PostCheckReadMessage(__msg, RmiName_LoginResponse);
        if(enableNotifyCallFromStub==true)
        {
        string parameterString = "";
        parameterString+=id.ToString()+",";
parameterString+=bIsLogin.ToString()+",";
        NotifyCallFromStub(Common.LoginResponse, RmiName_LoginResponse,parameterString);
        }

        if(enableStubProfiling)
        {
        Nettention.Proud.BeforeRmiSummary summary = new Nettention.Proud.BeforeRmiSummary();
        summary.rmiID = Common.LoginResponse;
        summary.rmiName = RmiName_LoginResponse;
        summary.hostID = remote;
        summary.hostTag = hostTag;
        BeforeRmiInvocation(summary);
        }

        long t0 = Nettention.Proud.PreciseCurrentTime.GetTimeMs();

        // Call this method.
        bool __ret =LoginResponse (remote,ctx , id, bIsLogin );

        if(__ret==false)
        {
        // Error: RMI function that a user did not create has been called. 
        core.ShowNotImplementedRmiWarning(RmiName_LoginResponse);
        }

        if(enableStubProfiling)
        {
        Nettention.Proud.AfterRmiSummary summary = new Nettention.Proud.AfterRmiSummary();
        summary.rmiID = Common.LoginResponse;
        summary.rmiName = RmiName_LoginResponse;
        summary.hostID = remote;
        summary.hostTag = hostTag;
        summary.elapsedTime = Nettention.Proud.PreciseCurrentTime.GetTimeMs()-t0;
        AfterRmiInvocation(summary);
        }
    }
    void ProcessReceivedMessage_CreatePlayer(Nettention.Proud.Message __msg, Nettention.Proud.ReceivedMessage pa, Object hostTag, Nettention.Proud.HostID remote)
    {
        Nettention.Proud.RmiContext ctx = new Nettention.Proud.RmiContext();
        ctx.sentFrom=pa.RemoteHostID;
        ctx.relayed=pa.IsRelayed;
        ctx.hostTag=hostTag;
        ctx.encryptMode = pa.EncryptMode;
        ctx.compressMode = pa.CompressMode;

        string id; Nettention.Proud.Marshaler.Read(__msg,out id);	
string tag; Nettention.Proud.Marshaler.Read(__msg,out tag);	
core.PostCheckReadMessage(__msg, RmiName_CreatePlayer);
        if(enableNotifyCallFromStub==true)
        {
        string parameterString = "";
        parameterString+=id.ToString()+",";
parameterString+=tag.ToString()+",";
        NotifyCallFromStub(Common.CreatePlayer, RmiName_CreatePlayer,parameterString);
        }

        if(enableStubProfiling)
        {
        Nettention.Proud.BeforeRmiSummary summary = new Nettention.Proud.BeforeRmiSummary();
        summary.rmiID = Common.CreatePlayer;
        summary.rmiName = RmiName_CreatePlayer;
        summary.hostID = remote;
        summary.hostTag = hostTag;
        BeforeRmiInvocation(summary);
        }

        long t0 = Nettention.Proud.PreciseCurrentTime.GetTimeMs();

        // Call this method.
        bool __ret =CreatePlayer (remote,ctx , id, tag );

        if(__ret==false)
        {
        // Error: RMI function that a user did not create has been called. 
        core.ShowNotImplementedRmiWarning(RmiName_CreatePlayer);
        }

        if(enableStubProfiling)
        {
        Nettention.Proud.AfterRmiSummary summary = new Nettention.Proud.AfterRmiSummary();
        summary.rmiID = Common.CreatePlayer;
        summary.rmiName = RmiName_CreatePlayer;
        summary.hostID = remote;
        summary.hostTag = hostTag;
        summary.elapsedTime = Nettention.Proud.PreciseCurrentTime.GetTimeMs()-t0;
        AfterRmiInvocation(summary);
        }
    }
    void ProcessReceivedMessage_GetPlayerRequest(Nettention.Proud.Message __msg, Nettention.Proud.ReceivedMessage pa, Object hostTag, Nettention.Proud.HostID remote)
    {
        Nettention.Proud.RmiContext ctx = new Nettention.Proud.RmiContext();
        ctx.sentFrom=pa.RemoteHostID;
        ctx.relayed=pa.IsRelayed;
        ctx.hostTag=hostTag;
        ctx.encryptMode = pa.EncryptMode;
        ctx.compressMode = pa.CompressMode;

        string id; Nettention.Proud.Marshaler.Read(__msg,out id);	
string tag; Nettention.Proud.Marshaler.Read(__msg,out tag);	
core.PostCheckReadMessage(__msg, RmiName_GetPlayerRequest);
        if(enableNotifyCallFromStub==true)
        {
        string parameterString = "";
        parameterString+=id.ToString()+",";
parameterString+=tag.ToString()+",";
        NotifyCallFromStub(Common.GetPlayerRequest, RmiName_GetPlayerRequest,parameterString);
        }

        if(enableStubProfiling)
        {
        Nettention.Proud.BeforeRmiSummary summary = new Nettention.Proud.BeforeRmiSummary();
        summary.rmiID = Common.GetPlayerRequest;
        summary.rmiName = RmiName_GetPlayerRequest;
        summary.hostID = remote;
        summary.hostTag = hostTag;
        BeforeRmiInvocation(summary);
        }

        long t0 = Nettention.Proud.PreciseCurrentTime.GetTimeMs();

        // Call this method.
        bool __ret =GetPlayerRequest (remote,ctx , id, tag );

        if(__ret==false)
        {
        // Error: RMI function that a user did not create has been called. 
        core.ShowNotImplementedRmiWarning(RmiName_GetPlayerRequest);
        }

        if(enableStubProfiling)
        {
        Nettention.Proud.AfterRmiSummary summary = new Nettention.Proud.AfterRmiSummary();
        summary.rmiID = Common.GetPlayerRequest;
        summary.rmiName = RmiName_GetPlayerRequest;
        summary.hostID = remote;
        summary.hostTag = hostTag;
        summary.elapsedTime = Nettention.Proud.PreciseCurrentTime.GetTimeMs()-t0;
        AfterRmiInvocation(summary);
        }
    }
    void ProcessReceivedMessage_GetPlayerResponse(Nettention.Proud.Message __msg, Nettention.Proud.ReceivedMessage pa, Object hostTag, Nettention.Proud.HostID remote)
    {
        Nettention.Proud.RmiContext ctx = new Nettention.Proud.RmiContext();
        ctx.sentFrom=pa.RemoteHostID;
        ctx.relayed=pa.IsRelayed;
        ctx.hostTag=hostTag;
        ctx.encryptMode = pa.EncryptMode;
        ctx.compressMode = pa.CompressMode;

        string id; Nettention.Proud.Marshaler.Read(__msg,out id);	
string tag; Nettention.Proud.Marshaler.Read(__msg,out tag);	
int bullet; Nettention.Proud.Marshaler.Read(__msg,out bullet);	
int hp; Nettention.Proud.Marshaler.Read(__msg,out hp);	
int life; Nettention.Proud.Marshaler.Read(__msg,out life);	
bool bIsSucess; Nettention.Proud.Marshaler.Read(__msg,out bIsSucess);	
core.PostCheckReadMessage(__msg, RmiName_GetPlayerResponse);
        if(enableNotifyCallFromStub==true)
        {
        string parameterString = "";
        parameterString+=id.ToString()+",";
parameterString+=tag.ToString()+",";
parameterString+=bullet.ToString()+",";
parameterString+=hp.ToString()+",";
parameterString+=life.ToString()+",";
parameterString+=bIsSucess.ToString()+",";
        NotifyCallFromStub(Common.GetPlayerResponse, RmiName_GetPlayerResponse,parameterString);
        }

        if(enableStubProfiling)
        {
        Nettention.Proud.BeforeRmiSummary summary = new Nettention.Proud.BeforeRmiSummary();
        summary.rmiID = Common.GetPlayerResponse;
        summary.rmiName = RmiName_GetPlayerResponse;
        summary.hostID = remote;
        summary.hostTag = hostTag;
        BeforeRmiInvocation(summary);
        }

        long t0 = Nettention.Proud.PreciseCurrentTime.GetTimeMs();

        // Call this method.
        bool __ret =GetPlayerResponse (remote,ctx , id, tag, bullet, hp, life, bIsSucess );

        if(__ret==false)
        {
        // Error: RMI function that a user did not create has been called. 
        core.ShowNotImplementedRmiWarning(RmiName_GetPlayerResponse);
        }

        if(enableStubProfiling)
        {
        Nettention.Proud.AfterRmiSummary summary = new Nettention.Proud.AfterRmiSummary();
        summary.rmiID = Common.GetPlayerResponse;
        summary.rmiName = RmiName_GetPlayerResponse;
        summary.hostID = remote;
        summary.hostTag = hostTag;
        summary.elapsedTime = Nettention.Proud.PreciseCurrentTime.GetTimeMs()-t0;
        AfterRmiInvocation(summary);
        }
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
		public override Nettention.Proud.RmiID[] GetRmiIDList { get{return Common.RmiIDList;} }
		
	}
}

