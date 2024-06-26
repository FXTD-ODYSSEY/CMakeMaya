"""dodo file."""

# Import future modules
from __future__ import absolute_import, division, print_function

# Import built-in modules
import glob
import logging
import os
import re
import shutil
import signal
import subprocess
import urllib
import winreg
from platform import system
from zipfile import ZipFile

# Import third-party modules
import requests
from doit.action import CmdAction
from tqdm import tqdm

# NOTES(timmyliang): for ctrl+C quit
signal.signal(signal.SIGINT, signal.SIG_DFL)
logging.basicConfig(level=logging.INFO)

DIR = os.path.dirname(__file__)
DOIT_CONFIG = {  # noqa: WPS407
    # NOTES(timmyliang): list task with definition order.
    "sort": "definition",
    "verbosity": 2,
    "continue": True,
    "default_tasks": ["init"],
}


def add_short_name(short_name):
    """Doit for short decorator.

    Args:
        short_name (str): short alias name.

    Returns:
        callable: decoartor function.
    """

    def decorator(func):
        globals()["task_{0}".format(short_name)] = func  # noqa: WPS421
        return func

    return decorator


def task_init():
    """Download Maya SDK from official site."""

    def install_package():
        # NOTES(timmyliang): install choco
        choco_install_command = [
            "PowerShell",
            "-Command",
            "Set-ExecutionPolicy Bypass -Scope Process -Force; [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))",
        ]
        subprocess.call(choco_install_command, shell=True)

        # NOTES(timmyliang): install Visual studio env
        vs_build_command = [
            "choco install visualstudio2015community --version=14.0.23107.0 --yes",
            "choco install visualstudio2017buildtools --yes",
            "choco install visualstudio2019buildtools --yes",
            "choco install vcredist140 --yes",
            "choco install cmake --installargs 'ADD_CMAKE_TO_PATH=System' --yes",
        ]
        for command in vs_build_command:
            subprocess.call(command, shell=True)

        logging.info("Please add VS C++ module with CMake Modules.")
        setup_path = os.path.join(
            os.getenv("ProgramFiles(x86)", ""),
            "Microsoft Visual Studio",
            "Installer",
            "setup.exe",
        )
        if os.path.isfile(setup_path):
            os.startfile(setup_path)
        else:
            logging.warning("VS setup.exe not found")

    return {"actions": [install_package]}


class DownloadProgressBar(tqdm):
    def update_to(self, b=1, bsize=1, tsize=None):
        if tsize is not None:
            self.total = tsize
        self.update(b * bsize - self.n)


class MayaSDKDownloader(object):
    platform_url = (
        "https://www.autodesk.com/developer-network/platform-technologies/maya"
    )

    @staticmethod
    def download_file(url, path):
        with DownloadProgressBar(
            unit="B", unit_scale=True, miniters=1, desc=path
        ) as bar:
            urllib.request.urlretrieve(url, filename=path, reporthook=bar.update_to)

    @staticmethod
    def get_download_urls(url):
        headers = {
            "user-agent": "Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/94.0.4606.81 Safari/537.36",
        }
        response = requests.request("GET", url, headers=headers)
        regx = r"https:.*s3-us-west-2\.amazonaws\.com.*zip"
        return re.findall(regx, response.text)

    @staticmethod
    def extract_zip(path, directory):
        with ZipFile(file=path) as zip_file:
            for file in tqdm(
                iterable=zip_file.namelist(), total=len(zip_file.namelist())
            ):
                zip_file.extract(member=file, path=directory)

    @staticmethod
    def check_devkit_exists(devkit_folder):
        if os.path.exists(devkit_folder):
            message = "{0} SDK folder already exists,do you want to download again.[y/N]".format(
                os.path.basename(devkit_folder)
            )
            response = input(message)  # noqa: WPS421
            if not response or response.lower() in ("n", "no"):
                return True
            shutil.rmtree(devkit_folder, ignore_errors=True)

    @staticmethod
    def cleanup_devkit(devkit_folder, version):
        folder_path = "devkitBase"
        if version == "2016":
            folder_path = os.path.join("Windows", folder_path)
        devkit = os.path.join(devkit_folder, folder_path)
        for files in os.listdir(devkit):
            shutil.move(os.path.join(devkit, files), devkit_folder)
        shutil.rmtree(devkit, ignore_errors=True)

    @staticmethod
    def get_latest_devkit_url(pattern, urls):
        # NOTES(timmyliang): get the latest devkit url
        filter_urls = filter(lambda url: pattern.search(url), urls)
        return next(filter_urls, "")

    @classmethod
    def download_maya_sdk(cls, versions, platform="Windows"):
        urls = cls.get_download_urls(cls.platform_url)
        for version in versions:
            pattern = re.compile(r"Maya(_*){0}.*_{1}".format(version, platform))
            # NOTES(timmyliang): get the latest devkit url
            url = cls.get_latest_devkit_url(pattern, urls)
            if not url:
                logging.warning("version {0} not found".format(version))
                break
            SDK_folder = os.path.join(DIR, "SDK")
            os.makedirs(SDK_folder, exist_ok=True)
            devkit_folder = os.path.join(SDK_folder, "maya{0}".format(version))
            if cls.check_devkit_exists(devkit_folder):
                continue

            zip_path = os.path.join(DIR, "SDK", url.split("/")[-1])
            cls.download_file(url, zip_path)
            cls.extract_zip(zip_path, devkit_folder)
            os.remove(zip_path)
            cls.cleanup_devkit(devkit_folder, version)


@add_short_name("sdk")
def task_download_sdk():
    """Download Maya SDK from official site."""
    return {
        "actions": [MayaSDKDownloader.download_maya_sdk],
        "params": [
            {
                "name": "versions",
                "short": "v",
                "type": list,
                "default": [],
                "help": "download version",
            },
            {
                "name": "platform",
                "short": "p",
                "type": str,
                "default": "Windows",
                "help": "show help",
            },
        ],
    }


@add_short_name("n")
def task_new():
    """Create a new project base on cookiecutter template."""
    command = " ".join(["poetry run cookiecutter", "-o", "projects", "template"])
    return {
        "actions": [lambda: subprocess.call(command, shell=True) or None],
        "verbosity": 0,
    }


@add_short_name("c")
def task_compile():
    """Run cmake to compile C++ plugin."""
    is_win = system() == "Windows"

    def run_cmake(version, project,skip_build):
        project_dict = {}
        for proj in glob.iglob("projects/**/CMakeLists.txt", recursive=True):
            project_path = os.path.dirname(os.path.relpath(proj, "projects"))
            project_dict[os.path.basename(project_path)] = project_path

        default_projects = ";".join(project_dict.values())

        project = project if project else default_projects
        project = project_dict.get(project, project)

        path = r"SOFTWARE\Autodesk\Maya\{0}\Setup\InstallPath".format(version)

        try:
            handle = winreg.OpenKey(winreg.HKEY_LOCAL_MACHINE, path)
        except FileNotFoundError:
            logging.error("maya {0} not found".format(version))
            return

        SDK_path = os.path.join(DIR, "SDK", "maya{0}".format(version))
        if not os.path.isdir(SDK_path):
            logging.error("maya {0} SDK not found".format(version))
            return

        # NOTES(timmyliang): add maya install location into env
        location, _ = winreg.QueryValueEx(handle, "MAYA_INSTALL_LOCATION")
        os.environ["MAYA_INSTALL_LOCATION"] = os.path.dirname(
            os.path.abspath(location)
        ).replace("\\", "/")

        compiler = "Visual Studio 16 2019" if is_win else "Unix Makefiles"
        build_command = 'cmake -Wno-dev -G "{compiler}" -DMAYA_VERSION={version} -DMAYA_PROJECT={project} . -B build'.format(
            compiler=compiler, version=version, project=project
        )
        compile_command = "cmake --build build --config Release"
        # NOTE windows set to utf-8 codec for chinese
        codec_command = "chcp 65001"
        commands = [codec_command, build_command, compile_command]
        if skip_build:
            commands.remove(build_command)
        else:
            shutil.rmtree(os.path.join(DIR, "build"), ignore_errors=True)
            
        print(" & ".join(commands))
        subprocess.call(" & ".join(commands),shell=True)
        # return " & ".join(commands)

    return {
        "actions": [run_cmake],
        "verbosity": 2,
        "params": [
            {
                "name": "version",
                "short": "v",
                "type": str,
                "default": "2020",
                "help": "maya version",
            },
            {
                "name": "project",
                "short": "p",
                "type": str,
                "default": "",
                "help": "compile project",
            },
            {
                "name": "skip_build",
                "long": "skip_build",
                "short": "sb",
                "type": bool,
                "default": False,
                "help": "skip build project",
            },
        ],
    }
