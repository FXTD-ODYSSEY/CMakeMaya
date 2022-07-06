# -*- coding: utf-8 -*-
"""
Run maya standlone mode test the mll load.
"""

from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

__author__ = "timmyliang"
__email__ = "820472580@qq.com"
__date__ = "2022-07-05 15:03:23"


import os
import sys
import maya.standalone

repo = (lambda f: lambda p=__file__: f(f, p))(
    lambda f, p: p
    if [
        d
        for d in os.listdir(p if os.path.isdir(p) else os.path.dirname(p))
        if d == ".git"
    ]
    else None
    if os.path.dirname(p) == p
    else f(f, os.path.dirname(p))
)()
MODULE = os.path.join(repo, "plug-ins", "Release")
sys.path.insert(0, MODULE) if MODULE not in sys.path else None

if __name__ == "__main__":
    maya.standalone.initialize()
    import maya.cmds as cmds

    target_dir = os.path.join(MODULE, "maya{0}".format(cmds.about(q=1, v=1)), "pyd")
    plugin_path = os.path.join(target_dir, "pyMayaPlugins.mll")
    cmds.loadPlugin(plugin_path)

    import mll_py

    mll_py.hello_world_maya("my string")

    cmds.unloadPlugin("pyMayaPlugins.mll")
    cmds.loadPlugin(plugin_path)

    import mll_py

    mll_py.hello_world_maya("my string2")

    maya.standalone.uninitialize()
