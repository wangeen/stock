#! /usr/bin/env python
import subprocess
import os, time

def script(cmd):
    subprocess.call(cmd,  shell=True)

os.chdir("./build")
script("rm -rf *")
script("cmake ..")
script("make -j 4")
script("./a_server &")
time.sleep(1);
script("./a_client &")
