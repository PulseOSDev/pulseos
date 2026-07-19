import sys
import os
import json

#
# Usage build <all, krnl, mkcfg, os, run>
#

#
# Req: Python 3.13.3, i686-elf-gcc, i686-elf-as, (QEMU or any other Virtualization Software)
# 

#
# Build Script for the pulseOS Operating System.
#

os.system("@echo off") # leave this alone, it has no purpose else than to look better in CMD

DEBUG   = True
CONFIG  = "config.cfg" # DO NOT MOVE 

LNKOS   = True
LNKKRNL = True

#
# JSON CONFIG
#

cfgFile = open(CONFIG)
data = json.load(cfgFile)

linkerFile = data["linkFile"]
KRNLASM    = data["krnlAsmSrcFiles"]
KRNLC      = data["krnlCSrcFiles"]
OSC        = data["osCSrcFiles"]
COUT       = data["cOutFiles"]
ASMOUT     = data["asmOutFiles"]
BINOUT     = data["binOutDir"]

#
# FUNCS FOR ARGUMENTS
#

def argcheck():
    arglength = len(sys.argv)

    try:
        if arglength >= 1:
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
            else:
                print("Usage: build <all, krnl, mkcfg, os, run>")

    except IndexError:
        print("Usage: build <all, krnl, mkcfg, os, run>")

def all(): # everything. except makeconfig & run...
    LNKOS = False
    LNKKRNL = False
    buildkrnl()
    buildos()

    link(ASMOUT, COUT, linkerFile)

def buildkrnl(): # Func for making the Kernel
    debug("[BUILD] Running buildkrnl")

    ckrnls = KRNLC.split(" ")
    asmkrnls = KRNLASM.split(" ")

    for file in ckrnls:
        ccompile("krnl/src/c/",f"{file}") # Compiling all C Files

    for file in asmkrnls:
        asmcompile("krnl/src/asm/", f"{file}") # Compiling all ASM Files

     # Linking everything

    if LNKKRNL == True:
        link(ASMOUT, COUT, linkerFile)


def buildos(): # Func for making the OS (currently empty)
    debug("[BUILD] Running buildos")

    ccompile(OSC, "/bridge") # Compiling the Bridge

    if LNKOS == True:
        link(ASMOUT, COUT, linkerFile)
    


def makeconfig(): # Func for making the Config
    debug("[BUILD] Running makeconfig")
    baseCfg = """{
    "linkFile"       : "build/linker/linker.ld",
    "krnlCSrcFiles"  : "krnl/src/c/kernel.c",
    "krnlAsmSrcFiles": "krnl/src/asm/boot.s",
    "osCSrcFiles"    : "os/src/",
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

def ccompile(path, file, out="build/out/c/"): # file: Filepath to the C File
    debug(f"[BUILD] Compiling (C) {file}.c")
    os.system(f"i686-elf-gcc -c {path}{file}.c -o {out}/{file}.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -Wimplicit-function-declaration -Wunused-function -Wunused-but-set-variable")

def asmcompile(path, file, out="build/out/asm/"): # file: Filepath to the ASM File
    debug(f"[BUILD] Compiling (ASM) {file}.asm")
    os.system(f"i686-elf-as {path}{file}.asm -o {out}/{file}.o")

def link(asmFiles, cFiles, linker=linkerFile, out=BINOUT): # asmFiles: Path to ASM Files to be linked (mostly ASMOUT); cFiles: Path to C Files to be linked (mostly COUT)
    debug(f"[BUILD] Linking...")
    os.system(f"i686-elf-gcc -T {linker} -o build/out/bin/syskrnl.bin -ffreestanding -O2 -nostdlib {asmFiles} {cFiles} -lgcc -Wimplicit-function-declaration -Wunused-function -Wunused-but-set-variable")

argcheck()

os.system("@echo on") # leave this alone, it has no purpose else than to look better in CMD