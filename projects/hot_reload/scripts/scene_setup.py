import pymel.core as pm

pm.newFile(f=1)

if pm.pluginInfo("hot_reload", q=1, l=1):
    pm.unloadPlugin("hot_reload")
    
# NOTE set to compile mll
pm.loadPlugin(r"F:\repo\CMakeMaya\plug-ins\Release\hot_reload.mll")


pm.polySphere()
pm.deformer(type="hotReloadableDeformer")
