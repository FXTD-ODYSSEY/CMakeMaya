# CMake for Maya

source repo from https://github.com/volodinroman/CMakeMaya
I add [pydoit](https://pydoit.org/) to make all the process easier.

## 📦Prerequisites

Install [Python3](https://www.python.org/) and [poetry](https://python-poetry.org/)

1. run `git clone git@github.com:FXTD-ODYSSEY/CMakeMaya.git` clone repo
2. run `git submodule update --init` clone all the submodule
3. run `pip install poetry` install poetry (skip this if you already install)
4. run `poetry install` install all python dependencies
5. run `poetry shell` get into the python virtual environment
6. run `doit init` initialize the build environment

### Prepare C++ compile environment

`doit init` will install chocolatey then use `choco` to install `VS Build Toolkit` & `CMake`

Be aware that, `VS Build Toolkit` need to install C++ requirement manually.
after `choco` install finished, it will open the `setup.exe` program.

---

### Download Maya SDK

run `doit sdk -v 2020` could download the SDK toolkit online and extract to the SDK folder with `maya<version>` name.

## 🔨Usage

### doit list

run `doit list` would list all available command.


### doit new

run `doit new` to create a template project for C++ into the projects folder.

### doit compile

run `doit compile` or `doit c` to compile the C++ plugin.
`doit help c` to get command help

`doit c -p weightDriver -v 2020` 
this command compile the weightDriver plugin for Maya2020 
if not specify the project and version, then compile all the projects to 2020 version.

---

`-p` flag send the projects relative folder to locate the compile `CMakeLists.txt`
To compile the weightDriver you can use `doit c -p IngoClemens/weightDriver`.
you can use the end folder for short, eg: `doit c -p weightDriver` 


## ⚠Notice

`IngoClemens/weightDriver` compile to Maya 2020.4 will be fail [issue](https://github.com/IngoClemens/weightDriver/issues/2)
you need to modify the source code for compatibility.

---

`sonictk/pyd` not support python3 yet.

---

`robertjoosten/maya-dem-bones` require `pybind11`.
however, `pyblind11` drop support for python2 in the new version.
if you want the lower version, you can checkout `pyblind11` branch to `v2.9`.

## ✒blog

https://blog.l0v0.com/posts/5875a169.html
