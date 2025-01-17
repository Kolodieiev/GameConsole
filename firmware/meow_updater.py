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
import filecmp
from pathlib import Path

GITHUB_REPO_URL = "https://github.com/Kolodieiev/meowui.git" 
ROOT_DIR = Path(".")  
VERSION_FILE = ROOT_DIR / "src/meow/version.txt"  
TEMP_DIR =  ROOT_DIR / "temp_meowui"
LIB_DIR = ROOT_DIR / "lib/TFT_eSPI"
MEOW_DIR = ROOT_DIR / "src/meow"

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
    version_file = TEMP_DIR / VERSION_FILE
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

def clone_repo(repo_url, target_folder):
    try:
        remove_dir(target_folder)
        subprocess.run(
            ["git", "clone", "--depth", "1", repo_url, str(target_folder)],
            check=True
        )
        print(f"Бібліотека успішно клонована в {target_folder}")

        remove_dir(target_folder / ".git")
        remove_dir(target_folder / ".github")
        remove_dir(target_folder / "docs")
        remove_dir(target_folder / "include")
        remove_unwanted_files(target_folder, ["README.md", ".gitignore"])
    except Exception as e:
        print(f"Помилка під час клонування: {e}")

def merge_dirs(src_folder, dst_folder):
    src_folder = Path(src_folder)
    dst_folder = Path(dst_folder)
    
    for src_path in src_folder.rglob('*'):
        relative_path = src_path.relative_to(src_folder)
        dst_path = dst_folder / relative_path

        if src_path.is_dir():
            dst_path.mkdir(parents=True, exist_ok=True)
        elif src_path.is_file():
            if not dst_path.exists() or not filecmp.cmp(src_path, dst_path, shallow=False):
                shutil.copy2(src_path, dst_path)  
                print(f"Файл {dst_path} оновлено.")

def remove_dir(folder_path):
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

        clone_repo(GITHUB_REPO_URL, TEMP_DIR)

        remote_version = read_remote_version()
        print(f"Віддалена версія: {remote_version if remote_version else 'немає'}")

        if not remote_version:
            print("Помилка. Не вдалося отримати версію з віддаленого репозиторію.")
            return

        if local_version != remote_version:
            print("Оновлення бібліотеки...")
            merge_dirs(TEMP_DIR, ROOT_DIR)
            print(f"Фреймворк оновлено до версії {remote_version}.")
        else:
            print("Версія meowui вже актуальна.")
    except Exception as e:
        print(f"Сталася помилка: {e}")
    finally:
        if TEMP_DIR.exists():
            remove_dir(TEMP_DIR)


if __name__ == "__main__":
    main()
