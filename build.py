import sys
import os
import json

#
# Usage build <all, krnl, config, os>
#

#
# Req: Python 3.13.3, i686-elf-gcc, i686-elf-as, (QEMU or any other Virtualization Software)
# 

#
# Build Script for the pulseOS Operating System.
#

os.system("@echo off") # leave this alone, it has no purpose else than to look better in CMD

DEBUG  = True
CONFIG = "config.cfg" # DO NOT MOVE 

#
# JSON CONFIG
#

cfgFile = open(CONFIG)
data = json.load(cfgFile)

linkerFile = data["linkFile"]
KRNLASM    = data["krnlAsmSrcFiles"]
KRNLC      = data["krnlCSrcFiles"]
OSASM      = data["osAsmSrcFiles"]
OSC        = data["osCSrcFiles"]
COUT       = data["cOutFiles"]
ASMOUT     = data["asmOutFiles"]
BINOUT     = data["binOutDir"]

#
# FUNCS FOR ARGUMENTS
#

def argcheck():
    arglength = len(sys.argv)

    if arglength == 0:
        print("pulseOS Build System v1")
    elif arglength >= 1:
        if sys.argv[1] == "all":
            all()
        elif sys.argv[1] == "krnl":
            buildkrnl()
        elif sys.argv[1] == "mkcfg":
            makeconfig()
        elif sys.argv[1] == "os":
            buildos()
        elif sys.argv[1] == "run":
            runkrnl()

def all(): # everything. except makeconfig & runkrnl...
    buildkrnl()
    buildos()

def buildkrnl(): # Func for making the Kernel
    debug("[BUILD] Running buildkrnl")

    ckrnls = KRNLC.split(" ")
    asmkrnls = KRNLASM.split(" ")

    for file in ckrnls:
        ccompile(f"{file}") # Compiling all C Files

    for file in asmkrnls:
        asmcompile(f"{file}") # Compiling all ASM Files

    link(ASMOUT, COUT) # Linking everything


def buildos(): # Func for making the OS (currently empty)
    debug("[BUILD] Running buildos")

def makeconfig(): # Func for making the Config
    debug("[BUILD] Running makeconfig")
    baseCfg = """{
    "linkFile"       : "build/linker/linker.ld",
    "krnlCSrcFiles"  : "krnl/src/c/kernel.c",
    "krnlAsmSrcFiles": "krnl/src/asm/boot.s",
    "osCSrcFiles"    : "os/src/c/",
    "osAsmSrcFiles"  : "os/src/asm/",
    "asmOutDir"      : "build/out/asm",
    "cOutDir"        : "build/out/c",
    "binOutDir"      : "build/out/bin"
}"""
    debug("[BUILD] Creating / Editing file config.cfg")

def runkrnl():
    os.system("qemu-system-i386 -kernel build/out/bin/syskrnl.bin")

#
# Funcs
#

def debug(message):
    if DEBUG == True:
        print(message)

def ccompile(file, out="build/out/c/"): # file: Filepath to the C File
    debug(f"[BUILD] Compiling (C) {file}.c")
    os.system(f"i686-elf-gcc -c krnl/src/c/{file}.c -o {out}/{file}.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra")

def asmcompile(file, out="build/out/asm/"): # file: Filepath to the ASM File
    debug(f"[BUILD] Compiling (ASM) {file}.asm")
    os.system(f"i686-elf-as krnl/src/asm/{file}.asm -o {out}/{file}.o")

def link(asmFiles, cFiles, linker=linkerFile, out=BINOUT): # asmFiles: Path to ASM Files to be linked (mostly ASMOUT); cFiles: Path to C Files to be linked (mostly COUT)
    debug(f"[BUILD] Linking...")
    os.system(f"i686-elf-gcc -T {linker} -o build/out/bin/syskrnl.bin -ffreestanding -O2 -nostdlib {asmFiles} {cFiles} -lgcc")

argcheck()

os.system("@echo on") # leave this alone, it has no purpose else than to look better in CMD