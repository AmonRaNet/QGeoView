import sys
import argparse
import os
import subprocess

project_path='.'

def clang_format(file_path):
    global project_path
    subprocess.call(['clang-format', '-i', file_path], cwd = project_path)
    print("formated")

def format_in_path(path):
    print(path)
    for root, dirs, files in os.walk(path):
        for file in files:
            if (file.endswith('.h') or file.endswith('.cpp')) and \
                not file.startswith('moc') and \
                not 'build/' in root :
                    fullname = root + "/" + file
                    print(fullname)
                    clang_format(fullname)

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("path", help="path to project")
    args = parser.parse_args()
    project_path = args.path
    format_in_path(project_path)

if __name__ == "__main__":
    sys.exit(main())
