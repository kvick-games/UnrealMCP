# UnrealMCP Plugin

# VERY WIP REPO
I'm working on adding more tools now and cleaning up the codebase, 
I plan to allow for easy tool extension outside the main plugin

This is very much a work in progress, and I need to clean up a lot of stuff!!!!!

Also, I only use windows, so I don't know how this would be setup for mac/unix

## Overview
UnrealMCP is an Unofficial Unreal Engine plugin designed to control Unreal Engine with AI tools. It implements a Model Context Protocol (MCP) https://modelcontextprotocol.io/introduction within Unreal Engine, allowing external AI systems to interact with and manipulate the Unreal environment programmatically.

I only just learned about MCP a few days ago, so I'm not that familiar with it, I'm still learning so things might be initially pretty rough.
I've implemented this using https://github.com/ahujasid/blender-mcp as a reference, which relies on claude for desktop. It may or may not work with other models, if you experiment with any, please let me know!

## ⚠️ DISCLAIMER
This plugin allows AI agents to directly modify your Unreal Engine project. While it can be a powerful tool, it also comes with risks:

- AI agents may make unexpected changes to your project
- Files could be accidentally deleted or modified
- Project settings could be altered
- Assets could be overwritten

**IMPORTANT SAFETY MEASURES:**
1. Always use source control (like Git or Perforce) with your project
2. Make regular backups of your project
3. Test the plugin in a separate project first
4. Review changes before committing them

By using this plugin, you acknowledge that:
- You are solely responsible for any changes made to your project
- The plugin author is not responsible for any damage, data loss, or issues caused by AI agents
- You use this plugin at your own risk

## Features
- TCP server implementation for remote control of Unreal Engine
- JSON-based command protocol for AI tools integration
- Editor UI integration for easy access to MCP functionality
- Comprehensive scene manipulation capabilities
- Python companion scripts for client-side interaction

## Roadmap
These are what I have in mind for development as of 3/14/2025
I'm not sure what's possible yet, in theory anything, but it depends on how
good the integrated LLM is at utilizing these tools.
- [X] Basic operations working
- [X] Python working
- [X] Materials
- [ ] User Extensions (in progress)
- [ ] Asset tools
- [ ] Blueprints
- [ ] Niagara VFX
- [ ] Metasound
- [ ] Landscape (I might hold off on this because Epic has mentioned they are going to be updating the landscape tools)
- [ ] Modeling Tools
- [ ] PCG

## Requirements
- Unreal Engine 5.5 (I have only tested on this version, may work with earlier, but no official support)
- C++ development environment configured for Unreal Engine
- Python 3.7+ for client-side scripting
- Model to run the commands, in testing I've been using Claude for Desktop https://claude.ai/download

## Installation
1. Clone this repository into your Unreal project's `Plugins` directory:
   ```
   git clone https://github.com/kvick-games/UnrealMCP.git Plugins/UnrealMCP
   ```
   The project path should match this pattern like so:
...\UNREAL_PROJECT\Plugins\UnrealMCP\

3. Regenerate your project files (right-click your .uproject file and select "Generate Visual Studio project files")
4. Build the project in whatever IDE you use, I use Rider, Visual Studio works (working on releases now)
5. Open your project and enable the plugin in Edit > Plugins > UnrealMCP
6. Enable Python plugins in Unreal
7. Run setup_unreal_mcp.bat (I probably need to make some fixes to this file as more people try it out)
8. Currently I've only tested with Claude for Desktop so follow the instructions below to continue

## With Claude for Desktop
You will need to find your installation directory for claude for desktop. Find claude_desktop_config.json and add an entry and make it look like so:
```json
{
    "mcpServers": {
        "unreal": {
            "command": "C:\\UnrealMCP_Project\\Plugins\\UnrealMCP\\MCP\\run_unreal_mcp.bat",
            "args": []
        }
    }
}
```
IN THE COMMAND FIELD PUT YOUR PATH TO YOUR PLUGIN DIRECTORY POINTED TO THE SCRIPT: "run_unreal_mcp.bat"
This script is located within ../plugin_root_directory/MCP/run_unreal_mcp.bat

You can refer to this link for more info:
https://modelcontextprotocol.io/quickstart/user

To find the path to your claude for desktop install you can go into settings and click 'Edit Config'
On my Windows PC the path is:
C:\Users\USERNAME\AppData\Roaming\Claude

## Usage
### In Unreal Editor
Once the plugin is enabled, you'll find MCP controls in the editor toolbar button. 
![image](https://github.com/user-attachments/assets/68338e7a-090d-4fd9-acc9-37c0c1b63227)

![image](https://github.com/user-attachments/assets/34f734ee-65a4-448a-a6db-9e941a588e93)

The TCP server can be started/stopped from here.
Check the output log under log filter LogMCP for extra information.

Once the server is confirmed up and running from the editor.
Open Claude for Desktop, ensure that the tools have successfully enabled, ask Claude to work in unreal.

Currently only basic operations are supported, creating objects, modfiying their transforms, getting scene info, and running python scripts.
Claude makes a lot of errors with unreal python as I believe there aren't a ton of examples for it, but let it run and it will usually figure things out.
I would really like to improve this aspect of how it works but it's low hanging fruit for adding functionality into unreal.

### Client-Side Integration
Use the provided Python scripts in the `MCP` directory to connect to and control your Unreal Engine instance:

```python
from unreal_mcp_client import UnrealMCPClient

# Connect to the Unreal MCP server
client = UnrealMCPClient("localhost", 13377)

# Example: Create a cube in the scene
client.create_object(
    class_name="StaticMeshActor",
    asset_path="/Engine/BasicShapes/Cube.Cube",
    location=(0, 0, 100),
    rotation=(0, 0, 0),
    scale=(1, 1, 1),
    name="MCP_Cube"
)
```

## Command Reference
The plugin supports various commands for scene manipulation:
- `get_scene_info`: Retrieve information about the current scene
- `create_object`: Spawn a new object in the scene
- `delete_object`: Remove an object from the scene
- `modify_object`: Change properties of an existing object
- `execute_python`: Run Python commands in Unreal's Python environment
- And more to come...

Refer to the documentation in the `Docs` directory for a complete command reference.

## Security Considerations
- The MCP server accepts connections from any client by default
- Limit server exposure to localhost for development
- Validate all incoming commands to prevent injection attacks

## Troubleshooting
- Ensure Unreal Engine is running with the MCP plugin.
- Check logs in Claude for Desktop for stderr output.
- Reach out on the discord, I just made it, but I will check it periodically
  Discord (Dreamatron Studios): https://discord.gg/abRftdSe
  
### Project Structure
- `Source/UnrealMCP/`: Core plugin implementation
  - `Private/`: Internal implementation files
  - `Public/`: Public header files
- `Content/`: Plugin assets
- `MCP/`: Python client scripts and examples
- `Resources/`: Icons and other resources

## License
MIT License

Copyright (c) 2025 kvick

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

## Credits
- Created by: kvick
- X: [@kvickart](https://x.com/kvickart)
- Discord: https://discord.gg/abRftdSe
  
### Thank you to testers!!!
- https://github.com/TheMurphinatur
  
- [@sidahuj](https://x.com/sidahuj) for the inspriation



## Contributing
Contributions are welcome, but I will need some time to wrap my head around things and cleanup first, lol
