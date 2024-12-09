"""
Скрипт для завантаження/оновлення актуальної версії meowui.
Скрипт зберігається в репозиторії виключно для розробника прошивки.
Не потрібно запускати його самостійно. 
Репозиторій завжди знаходиться в актуальному стані. 
"""

import os
import shutil
import subprocess
import errno
import stat
from pathlib import Path

GITHUB_REPO_URL = "https://github.com/Kolodieiev/meowui.git" 
ROOT_FOLDER = Path(".")  
VERSION_FILE = ROOT_FOLDER / "src/meow/version.txt"  
TEMP_FOLDER = Path(ROOT_FOLDER) / "temp_meowui"

def get_latest_commit(repo_url):
    
    try:
        result = subprocess.run(
            ["git", "ls-remote", repo_url, "HEAD"],
            capture_output=True,
            text=True,
            check=True
        )
        return result.stdout.split()[0]
    except Exception as e:
        print(f"Помилка під час отримання останнього коміта: {e}")
        return None

def read_local_version():
    if VERSION_FILE.exists():
        with open(VERSION_FILE, "r") as f:
            return f.read().strip()
    return None

def read_remote_version():
    version_file = TEMP_FOLDER / VERSION_FILE
    if version_file.exists():
        with open(version_file, "r") as f:
            return f.read().strip()
    return None

def remove_unwanted_files(folder_path, files_to_remove):
    for file_name in files_to_remove:
        file_path = folder_path / file_name
        if file_path.exists():
            try:
                file_path.unlink()
                print(f"Файл '{file_name}' видалено з {folder_path}")
            except Exception as e:
                print(f"Помилка під час видалення файлу '{file_name}': {e}")

def clone_library(repo_url, target_folder):
    try:
        remove_folder(target_folder)
        subprocess.run(
            ["git", "clone", "--depth", "1", repo_url, str(target_folder)],
            check=True
        )
        print(f"Бібліотека успішно клонована в {target_folder}")
        remove_unwanted_files(target_folder, ["README.md", ".gitignore"])
    except Exception as e:
        print(f"Помилка під час клонування: {e}")

def merge_folders(src_folder, dst_folder):
    for src_dir, _, files in os.walk(src_folder):
        dst_dir = src_dir.replace(str(src_folder), str(dst_folder), 1)
        os.makedirs(dst_dir, exist_ok=True)
        for file in files:
            src_file = Path(src_dir) / file
            dst_file = Path(dst_dir) / file
            if dst_file.exists():
                if dst_file.is_file():
                    dst_file.unlink() 
            shutil.move(str(src_file), str(dst_file))
    shutil.rmtree(src_folder) 

def remove_folder(folder_path):
    try:
        if folder_path.exists():
            shutil.rmtree(folder_path, onerror=onerror)
            print(f"Видалено: {folder_path}")
    except FileNotFoundError:
        print(f"Директорія '{folder_path}' не знайдена.")
    except Exception as e:
        print(f"Сталася помилка під час видалення директорії: {e}")
    
def onerror(func, path, exc_info):
    if exc_info[1].errno == errno.EACCES: 
        os.chmod(path, stat.S_IWUSR)
        func(path) 
    else:
        raise  

def main():
    print("=== Запуск оновлення meowui ===")
    
    try:
        local_version = read_local_version()
        print(f"Локальна версія: {local_version if local_version else 'немає'}")

        clone_library(GITHUB_REPO_URL, TEMP_FOLDER)

        remote_version = read_remote_version()
        print(f"Віддалена версія: {remote_version if remote_version else 'немає'}")

        if not remote_version:
            print("Помилка. Не вдалося отримати версію з віддаленого репозиторію.")
            return

        if local_version != remote_version:
            print("Оновлення бібліотеки...")
            merge_folders(TEMP_FOLDER, ROOT_FOLDER)
            print(f"Фреймворк оновлено до версії {remote_version}.")
        else:
            print("Версія meowui вже актуальна.")
    except Exception as e:
        print(f"Сталася помилка: {e}")
    finally:
        if TEMP_FOLDER.exists():
            remove_folder(TEMP_FOLDER)


if __name__ == "__main__":
    main()
