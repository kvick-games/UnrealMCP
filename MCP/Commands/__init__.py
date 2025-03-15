"""Command modules for the UnrealMCP bridge.""" 

from . import commands_scene, commands_materials, commands_python, commands_blueprint

def register_all(mcp):
    commands_scene.register_all(mcp)
    commands_materials.register_all(mcp)
    commands_python.register_all(mcp)
    commands_blueprint.register_all(mcp) 