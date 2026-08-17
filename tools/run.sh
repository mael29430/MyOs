#!/bin/bash

echo "Démarrage de MyOS..."

qemu-system-aarch64 \
-machine virt \
-cpu cortex-a72 \
-kernel myos.elf \
-nographic
