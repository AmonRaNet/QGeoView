import sys
import argparse
import os
import re

target_head = b''

def replace(file_path):
    global target_head
    head_start = False
    head_end = False
    first_line = False
    body = b''
    with open(file_path, 'rb') as fr:
        for lineBin in fr:
            lineStr = lineBin.decode('utf-8')
            # Detect start line in head
            if not head_start:
                head_start = re.match(r'^\/\*\*\*+', lineStr)
                if head_start:
                    continue
                else:
                    head_start = True
                    head_end = True
            # Detect end line in head
            if head_start and not head_end:
                head_end = re.match(r'^\s+\*\*\*+\/', lineStr)
                continue
            # Wait first line in body
            if not first_line:
                if lineStr == '' or lineStr == '\n' or lineStr == '\r\n':
                    continue
                first_line = True
            # Add line by line
            body += lineBin
    # Output with head and body
    output = target_head + b'\r\n' + body
    # CRLF in text
    output = output.replace(b'\r\n', b'\n')
    output = output.replace(b'\n', b'\r\n')
    # Write file
    fw = open(file_path, 'wb')
    fw.write(output)
    print("replaced")

def load_target_head(file_path):
    global target_head
    fr = open(file_path, 'rb')
    target_head = fr.read()
    # CRLF in text
    target_head = target_head.replace(b'\r\n', b'\n')
    target_head = target_head.replace(b'\n', b'\r\n')

def replace_in_path(path):
    print(path)
    for root, dirs, files in os.walk(path):
        for file in files:
            if (file.endswith('.h') or file.endswith('.cpp')) and \
                not file.startswith('moc') and \
                not 'build/' in root :
                    fullname = root + "/" + file
                    print(fullname)
                    replace(fullname)

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("path", help="path to project")
    parser.add_argument("head", help="path to head file")
    args = parser.parse_args()
    load_target_head(args.head)
    replace_in_path(args.path)

if __name__ == "__main__":
    sys.exit(main())
