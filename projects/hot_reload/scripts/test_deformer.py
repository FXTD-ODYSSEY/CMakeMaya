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

signal.signal(signal.SIGINT, signal.SIG_DFL)

logging.basicConfig()
logger = logging.getLogger(__name__)
logger.setLevel(logging.INFO)

PORT = 2323

REPO = (lambda f: f(f, os.path.dirname(__file__)))(
    lambda f, p: p
    if [d for d in os.listdir(p) if d == ".git"]
    else f(f, os.path.dirname(p))
    if os.path.dirname(p) != p
    else None
)


def check_port_in_use(port, host="localhost"):
    sock = None
    try:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        result = sock.connect_ex((host, int(port)))
        return result == 0
    except:
        traceback.print_exc()
        return False
    finally:
        if sock:
            sock.close()


def setup_maya_scene():

    import pymel.core as pm

    pm.newFile(f=1)

    if pm.pluginInfo("hot_reload", q=1, l=1):
        pm.unloadPlugin("hot_reload")

    # NOTE set to compile mll
    maya_version = "maya{0}".format(pm.about(q=1, v=1))
    plugin_path = os.path.join(
        REPO, "plug-ins", "Release", "hot_reload", maya_version, "hot_reload.mll"
    )
    pm.loadPlugin(plugin_path)

    pm.polySphere()
    pm.deformer(type="hotReloadableDeformer")


def update_deformer():
    import pymel.core as pm
    import pymel.core.datatypes as dt
    from maya import OpenMaya

    tempdir = tempfile.gettempdir()
    json_path = os.path.join(tempdir, "maya_hot_reload","pt_data.json")
    
    for deformer in pm.ls(type="hotReloadableDeformer"):
        pm.dgdirty(deformer)

    sphere = pm.PyNode('pSphere1')
    itr = OpenMaya.MItMeshVertex(sphere.__apimdagpath__())
    pt_data = {}
    while not itr.isDone():
        pt = itr.position(OpenMaya.MSpace.kWorld)
        pt_data[str(itr.index())] = (pt.x,pt.y,pt.z)
        itr.next()

    if os.path.exists(json_path):
        with open(json_path,'r') as rf:
            data = json.load(rf)
        
        is_change = False
        for index,xyz in pt_data.items():
            last_xyz = data[index]
            current = dt.Vector(xyz[0],xyz[1],xyz[2])
            last = dt.Vector(last_xyz[0],last_xyz[1],last_xyz[2])
            if (current - last).length() > 0.01:
                is_change = True
                break
        
        logger.info("sphere change" if is_change else "sphere not change")
    else:
        logger.info("sphere data not exists.")
    
    folder = os.path.dirname(json_path)
    if not os.path.isdir(folder):
        os.makedirs(folder)
    with open(json_path,'w') as wf:
        json.dump(pt_data,wf)

def run_maya_server(port):
    setup_maya_scene()

    sock = socket.socket()
    sock.bind(("localhost", int(port)))
    sock.listen(5)
    logger.info("start to bind port {0}".format(port))
    while True:
        conn, addr = sock.accept()
        result = conn.recv(1024).decode("utf-8")
        if not result:
            continue
        try:
            update_deformer()
            logger.info("execute finished~")
        except:
            traceback.print_exc()
        finally:
            conn.close()


def connect_maya_server(port):

    sock = socket.socket()
    sock.connect(("localhost", port))
    command = r"execfile@{0}"
    logger.info("send command: {0}".format(command))
    sock.send(command)
    sock.close()


def main():

    # NOTES(timmyliang): if run in maya env | then launch server
    is_used = check_port_in_use(PORT)
    logger.debug("is_port_used: {0}".format(is_used))
    if not is_used:
        itr = iter(sys.argv)
        scirpt_path = next(itr, "")
        port = next(itr, "")
        if port:
            run_maya_server(port)
        else:
            subprocess.Popen(
                ["start", "cmd", "/k", sys.executable, scirpt_path, str(PORT)],
                shell=True,
            )
    else:
        connect_maya_server(PORT)


if __name__ == "__main__":
    main()
