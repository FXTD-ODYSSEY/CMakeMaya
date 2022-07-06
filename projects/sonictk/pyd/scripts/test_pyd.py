# -*- coding: utf-8 -*-
"""
Run maya standlone mode test the pyd import.
"""

from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

__author__ = "timmyliang"
__email__ = "820472580@qq.com"
__date__ = "2022-07-05 11:29:19"


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


if __name__ == "__main__":
    maya.standalone.initialize()
    from maya import cmds

    MODULE = os.path.join(repo, "plug-ins", "Release")
    target_dir = os.path.join(MODULE, "maya{0}".format(cmds.about(q=1, v=1)), "pyd")
    sys.path.insert(0, target_dir) if target_dir not in sys.path else None
    import py_hello

    py_hello.hello_world_maya("my string")

    maya.standalone.uninitialize()
