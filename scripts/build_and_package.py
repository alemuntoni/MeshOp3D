# Usage:
# python3 scripts/build_and_package.py --qt6-dir "/opt/Qt/6.11.1/gcc_64" --version "0.0.1"

import os
import sys
import platform
import argparse
import subprocess
import shutil
import glob

def run_command(cmd, cwd=None):
    print(f"Running: {' '.join(cmd)}")
    result = subprocess.run(cmd, cwd=cwd)
    if result.returncode != 0:
        print(f"Error: Command failed with return code {result.returncode}")
        sys.exit(result.returncode)

def main():
    parser = argparse.ArgumentParser(description="Build and package the application.")
    parser.add_argument("--qt6-dir", help="Path to Qt6_DIR")
    parser.add_argument("--version", help="Project version", default="0.0.1")
    args = parser.parse_args()

    system = platform.system()
    project_root = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
    build_dir = os.path.join(project_root, 'build')
    packages_dir = os.path.join(project_root, 'packages')

    print(f"Detected OS: {system}")

    # 1. Configure
    cmake_config_cmd = [
        "cmake", "-S", ".", "-B", "build",
        "-DCMAKE_BUILD_TYPE=Release",
        f"-DCMAKE_PROJECT_VERSION={args.version}"
    ]
    
    if shutil.which("ninja"):
        print("Ninja build system found, using it.")
        cmake_config_cmd.extend(["-G", "Ninja"])
        
    if args.qt6_dir:
        cmake_config_cmd.append(f"-DCMAKE_PREFIX_PATH={args.qt6_dir}")

    run_command(cmake_config_cmd, cwd=project_root)

    # 2. Build
    cmake_build_cmd = ["cmake", "--build", "build", "--config", "Release"]
    run_command(cmake_build_cmd, cwd=project_root)

    # 3. Package
    cpack_generators = ""
    if system == "Windows":
        cpack_generators = "ZIP;NSIS"
    elif system == "Darwin":
        cpack_generators = "DragNDrop"
    elif system == "Linux":
        cpack_generators = "DEB;TGZ;AppImage"
    else:
        print(f"Unsupported OS for packaging: {system}")
        sys.exit(1)

    cpack_cmd = ["cpack", "-C", "Release", "-G", cpack_generators]
    run_command(cpack_cmd, cwd=build_dir)

    # 4. Copy to packages directory
    if not os.path.exists(packages_dir):
        os.makedirs(packages_dir)

    patterns = ['*.zip', '*.exe', '*.dmg', '*.deb', '*.tar.gz', '*.AppImage']
    
    copied = 0
    for pattern in patterns:
        for file_path in glob.glob(os.path.join(build_dir, pattern)):
            # skip linuxdeploy AppImages as per deploy.yml
            if "linuxdeploy" in os.path.basename(file_path):
                continue
            
            dest_path = os.path.join(packages_dir, os.path.basename(file_path))
            print(f"Copying {os.path.basename(file_path)} to packages/")
            shutil.copy2(file_path, dest_path)
            copied += 1
            
    if copied == 0:
        print("Warning: No package files were found to copy.")
    else:
        print(f"Successfully copied {copied} package(s) to {packages_dir}")

if __name__ == "__main__":
    main()
