# -*- coding: utf-8 -*-
"""

"""

from __future__ import division
from __future__ import print_function
from __future__ import absolute_import

__author__ = "timmyliang"
__email__ = "820472580@qq.com"
__date__ = "2022-07-05 16:23:35"

# Import built-in modules
import logging
import os
import signal
import socket
import subprocess
import sys
import traceback
import tempfile
import json
import winreg

REPO = (lambda f:f(f,os.path.dirname(__file__)))(lambda f,p: p if [d for d in os.listdir(p) if d == '.git'] else f(f,os.path.dirname(p)) if os.path.dirname(p) != p else None)

DIR = os.path.dirname(__file__)
VERSION = 2020


def main():

    # NOTE add maya script folder
    os.environ["MAYA_SCRIPT_PATH"] = DIR

    # NOTES(timmyliang): find maya
    reg_path = r"SOFTWARE\Autodesk\Maya\{0}\Setup\InstallPath".format(VERSION)
    handle = winreg.OpenKey(winreg.HKEY_LOCAL_MACHINE, reg_path)
    location, _ = winreg.QueryValueEx(handle, "MAYA_INSTALL_LOCATION")
    maya_exe = os.path.join(location, "bin", "maya.exe")
    os.startfile(maya_exe)


if __name__ == "__main__":
    main()
