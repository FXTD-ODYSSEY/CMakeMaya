# CMake for Maya

source repo from https://github.com/volodinroman/CMakeMaya
I add pydoit to make all the process easier.

## Requirement

Install [Python](https://www.python.org/) 
run `pip install poetry`

## Prerequisites

run `poetry install` to install the packages.
then run `poetry run doit` to init the build process.

Be aware that, VS Build Toolkit need to install cmake manually.
after choco install finish, it will open the `setup.exe` program.

---

## Usage



### doit list

run `doit list` would list all available command.


### doit new

run `doit new` to create a template project for C++ into the projects folder.

### doit compile

run `doit compile` to compile the C++ plugin.

