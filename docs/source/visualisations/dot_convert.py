#!/usr/local/bin/python
import os
import subprocess
import sys


def main():
    this_file = os.path.join(os.getcwd(), __file__)
    for root, dirs, files in os.walk(os.path.dirname(this_file)):
        for file in files:
            fname, ext = os.path.splitext(file)
            if ext == '.dot':
                print('Converting: %s' % file)
                fp_in = os.path.join(root, fname + ext)
                # dot -odoc_insert_remove.png -Tpng doc_insert_remove.dot
                for typ in ('svg', 'png'):
                    cmd = ['dot', '-o%s' % os.path.join(root, fname + '.' + typ), '-T%s' % typ, fp_in]
                    print(f'CMD: {cmd}')
                    subprocess.Popen(cmd)
    return 0


if __name__ == '__main__':
    sys.exit(main())
