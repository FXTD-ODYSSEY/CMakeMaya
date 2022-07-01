
## source code license

https://sonictk.github.io/maya_hot_reload_example_public/

mll call dll function

fix some problem with hot reload source code

## blog 


## cmake

cmake will try to remove the dll and mll 
if remove fail then skip the mll compile and rename the `logic.dll` to `logic_old.dll`
then recompile the logic dll.
after that, you can run `cmds.dgdirty( 'hotReloadableDeformer1' )` for reload the logic.dll function.



