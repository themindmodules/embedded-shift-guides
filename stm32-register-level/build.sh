#!/bin/sh
# Build one project with arm-none-eabi-gcc, no vendor library needed.
#   sh build.sh 01-blink          builds 01-blink/main.c
#   sh build.sh all               builds every project
set -e
cd "$(dirname "$0")"
FLAGS="-mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Os -g -Wall -Wextra -Werror -std=gnu11 -ffreestanding -fno-builtin -nostartfiles"

build_one() {
    P="$1"
    mkdir -p build
    arm-none-eabi-gcc $FLAGS -Icommon -Tcommon/linker.ld -Wl,--gc-sections -Wl,-Map="build/$P.map" \
        common/startup.c "$P/main.c" -o "build/$P.elf"
    arm-none-eabi-objcopy -O binary "build/$P.elf" "build/$P.bin"
    arm-none-eabi-size "build/$P.elf"
}

if [ "$1" = "all" ]; then
    for d in [0-9][0-9]-*/; do build_one "${d%/}"; done
else
    build_one "$1"
fi
echo "BUILD OK"
