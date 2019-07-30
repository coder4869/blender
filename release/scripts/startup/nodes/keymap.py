import bpy

def register():
    wm = bpy.context.window_manager

    km = wm.keyconfigs.addon.keymaps.new(
        name="Node Editor",
        space_type='NODE_EDITOR')

    km.keymap_items.new(
        "fn.node_search",
        type='A',
        value='PRESS',
        ctrl=True)