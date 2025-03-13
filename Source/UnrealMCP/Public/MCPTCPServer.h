#pragma once
#include "CoreMinimal.h"
#include "Containers/Ticker.h"
#include "Json.h"
#include "Networking.h"
#include "Common/TcpListener.h"
#include "Sockets.h"
#include "SocketSubsystem.h"

/**
 * Configuration struct for the TCP server
 * Allows for easy customization of server parameters
 */
struct FMCPTCPServerConfig
{
    /** Port to listen on */
    int32 Port = 1337;
    
    /** Client timeout in seconds */
    float ClientTimeoutSeconds = 30.0f;
    
    /** Size of the receive buffer in bytes */
    int32 ReceiveBufferSize = 8192;
    
    /** Tick interval in seconds */
    float TickIntervalSeconds = 0.1f;
    
    /** Whether to log verbose messages */
    bool bEnableVerboseLogging = false;
};

/**
 * Structure to track client connection information
 */
struct FMCPClientConnection
{
    /** Socket for this client */
    FSocket* Socket;
    
    /** Endpoint information */
    FIPv4Endpoint Endpoint;
    
    /** Time since last activity for timeout tracking */
    float TimeSinceLastActivity;
    
    /** Buffer for receiving data */
    TArray<uint8> ReceiveBuffer;

    /**
     * Constructor
     * @param InSocket - The client socket
     * @param InEndpoint - The client endpoint
     * @param BufferSize - Size of the receive buffer
     */
    FMCPClientConnection(FSocket* InSocket, const FIPv4Endpoint& InEndpoint, int32 BufferSize = 8192)
        : Socket(InSocket)
        , Endpoint(InEndpoint)
        , TimeSinceLastActivity(0.0f)
    {
        ReceiveBuffer.SetNumUninitialized(BufferSize);
    }
};

/**
 * Interface for command handlers
 * Allows for easy addition of new commands without modifying the server
 */
class IMCPCommandHandler
{
public:
    virtual ~IMCPCommandHandler() {}
    
    /**
     * Get the command name this handler responds to
     * @return The command name
     */
    virtual FString GetCommandName() const = 0;
    
    /**
     * Handle a command
     * @param Params - The command parameters
     * @param ClientSocket - The client socket to respond to
     * @return JSON response object
     */
    virtual TSharedPtr<FJsonObject> HandleCommand(const TSharedPtr<FJsonObject>& Params, FSocket* ClientSocket) = 0;
};

/**
 * TCP Server for the Unreal MCP system
 * Handles client connections and command processing
 */
class UNREALMCP_API FMCPTCPServer
{
public:
    /**
     * Constructor
     * @param Config - Server configuration
     */
    explicit FMCPTCPServer(const FMCPTCPServerConfig& Config = FMCPTCPServerConfig());
    
    /**
     * Destructor
     */
    virtual ~FMCPTCPServer();

    /**
     * Start the server
     * @return True if started successfully
     */
    bool Start();
    
    /**
     * Stop the server
     */
    void Stop();
    
    /**
     * Check if the server is running
     * @return True if running
     */
    bool IsRunning() const { return bRunning; }
    
    /**
     * Register a command handler
     * @param Handler - The command handler to register
     */
    void RegisterCommandHandler(TSharedPtr<IMCPCommandHandler> Handler);
    
    /**
     * Unregister a command handler
     * @param CommandName - The name of the command to unregister
     */
    void UnregisterCommandHandler(const FString& CommandName);
    
    /**
     * Send a response to a client
     * @param Client - The client socket
     * @param Response - The response to send
     */
    void SendResponse(FSocket* Client, const TSharedPtr<FJsonObject>& Response);

protected:
    /**
     * Tick function called by the ticker
     * @param DeltaTime - Time since last tick
     * @return True to continue ticking
     */
    bool Tick(float DeltaTime);
    
    /**
     * Process pending connections
     */
    virtual void ProcessPendingConnections();
    
    /**
     * Process client data
     */
    virtual void ProcessClientData();
    
    /**
     * Process a command
     * @param CommandJson - The command JSON
     * @param ClientSocket - The client socket
     */
    virtual void ProcessCommand(const FString& CommandJson, FSocket* ClientSocket);
    
    /**
     * Check for client timeouts
     * @param DeltaTime - Time since last tick
     */
    virtual void CheckClientTimeouts(float DeltaTime);
    
    /**
     * Clean up a client connection
     * @param ClientConnection - The client connection to clean up
     */
    virtual void CleanupClientConnection(FMCPClientConnection& ClientConnection);
    
    /**
     * Clean up a client connection by socket
     * @param ClientSocket - The client socket to clean up
     */
    virtual void CleanupClientConnection(FSocket* ClientSocket);
    
    /**
     * Clean up all client connections
     */
    virtual void CleanupAllClientConnections();
    
    /**
     * Get a safe description of a socket
     * @param Socket - The socket
     * @return A safe description string
     */
    FString GetSafeSocketDescription(FSocket* Socket);
    
    /**
     * Connection handler
     * @param InSocket - The new client socket
     * @param Endpoint - The client endpoint
     * @return True if connection accepted
     */
    virtual bool HandleConnectionAccepted(FSocket* InSocket, const FIPv4Endpoint& Endpoint);

    /** Server configuration */
    FMCPTCPServerConfig Config;
    
    /** TCP listener */
    FTcpListener* Listener;
    
    /** Client connections */
    TArray<FMCPClientConnection> ClientConnections;
    
    /** Running flag */
    bool bRunning;
    
    /** Ticker handle */
    FTSTicker::FDelegateHandle TickerHandle;
    
    /** Command handlers map */
    TMap<FString, TSharedPtr<IMCPCommandHandler>> CommandHandlers;

private:
    // Disable copy and assignment
    FMCPTCPServer(const FMCPTCPServer&) = delete;
    FMCPTCPServer& operator=(const FMCPTCPServer&) = delete;
}; 