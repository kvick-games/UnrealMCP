"""Blueprint-related commands for UnrealMCP.

This module provides commands for creating, modifying, and querying blueprints in Unreal Engine.
"""

import sys
import os
import json
from typing import Dict, Any, Optional, List

# Add the parent directory to sys.path to import unreal_mcp_bridge
mcp_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
if mcp_dir not in sys.path:
    sys.path.insert(0, mcp_dir)

from unreal_mcp_bridge import send_command
from mcp.server.fastmcp import Context

def register_all(mcp):
    """Register all blueprint-related commands with the MCP server."""
    
    @mcp.tool()
    def create_blueprint(ctx: Context, package_path: str, name: str, properties: Dict[str, Any] = None) -> Dict[str, Any]:
        """Create a new blueprint in the Unreal project.
        
        Args:
            ctx: The context object.
            package_path: The path where the blueprint should be created (e.g., "/Game/Blueprints").
            name: The name of the blueprint.
            properties: Optional properties for the blueprint (e.g., parent_class).
            
        Returns:
            A dictionary with information about the created blueprint.
        """
        try:
            params = {
                "package_path": package_path,
                "name": name
            }
            
            if properties:
                params["properties"] = properties
                
            response = send_command("create_blueprint", params)
            
            if response["status"] == "success":
                return {
                    "status": "success",
                    "result": response["result"]
                }
            else:
                return {
                    "status": "error",
                    "message": response.get("message", "Failed to create blueprint")
                }
        except Exception as e:
            return {
                "status": "error",
                "message": f"Error creating blueprint: {str(e)}"
            }

    @mcp.tool()
    def get_blueprint_info(ctx: Context, blueprint_path: str) -> Dict[str, Any]:
        """Get information about a blueprint.
        
        Args:
            ctx: The context object.
            blueprint_path: The path to the blueprint (e.g., "/Game/Blueprints/MyBlueprint").
            
        Returns:
            A dictionary with information about the blueprint.
        """
        try:
            params = {
                "blueprint_path": blueprint_path
            }
            
            response = send_command("get_blueprint_info", params)
            
            if response["status"] == "success":
                return {
                    "status": "success",
                    "result": response["result"]
                }
            else:
                return {
                    "status": "error",
                    "message": response.get("message", "Failed to get blueprint info")
                }
        except Exception as e:
            return {
                "status": "error",
                "message": f"Error getting blueprint info: {str(e)}"
            }

    @mcp.tool()
    def modify_blueprint(ctx: Context, blueprint_path: str, properties: Dict[str, Any]) -> Dict[str, Any]:
        """Modify an existing blueprint.
        
        Args:
            ctx: The context object.
            blueprint_path: The path to the blueprint to modify.
            properties: Properties to modify (e.g., description, category, options).
            
        Returns:
            A dictionary indicating success or failure.
        """
        try:
            params = {
                "blueprint_path": blueprint_path,
                "properties": properties
            }
            
            response = send_command("modify_blueprint", params)
            
            if response["status"] == "success":
                return {
                    "status": "success"
                }
            else:
                return {
                    "status": "error",
                    "message": response.get("message", "Failed to modify blueprint")
                }
        except Exception as e:
            return {
                "status": "error",
                "message": f"Error modifying blueprint: {str(e)}"
            }

    @mcp.tool()
    def create_blueprint_event(ctx: Context, event_name: str, blueprint_path: str = None, parameters: Dict[str, Any] = None) -> Dict[str, Any]:
        """Create a new event in a blueprint.
        
        Args:
            ctx: The context object.
            event_name: The name of the event to create.
            blueprint_path: Optional path to the blueprint. If not provided, a new blueprint will be created.
            parameters: Optional parameters for the event.
            
        Returns:
            A dictionary with information about the created event.
        """
        try:
            params = {
                "event_name": event_name
            }
            
            if blueprint_path:
                params["blueprint_path"] = blueprint_path
                
            if parameters:
                params["parameters"] = parameters
                
            response = send_command("create_blueprint_event", params)
            
            if response["status"] == "success":
                return {
                    "status": "success",
                    "result": response["result"]
                }
            else:
                return {
                    "status": "error",
                    "message": response.get("message", "Failed to create blueprint event")
                }
        except Exception as e:
            return {
                "status": "error",
                "message": f"Error creating blueprint event: {str(e)}"
            } 