#!/usr/bin/env bash

if [ $EUID -ne 0 ]; then
    echo "you should run this script with root privileges."
    if [ 0 -eq 1 ]; then exit 1; fi
fi

try() {
    $@
    local code=$?
    if [ $code -ne 0 ]; then
        echo "command \"$@\" failed with exit code $code."
        exit 1
    fi
}

echo "this will build the compilation toolchain (GCC + Binutils)."

while true; do
    read -p "proceed? [Y/N] " CHOICE
    case $CHOICE in
        [Yy]*) break;;
        [Nn]*) exit;;
        *) continue;;
    esac
done

while true; do
    echo
    while true; do
        read -p "installation path (absolute)? " PREFIX
        if [ -z $PREFIX ]; then continue;
        else break; fi
    done
    
    echo "(the installation path will be appended to your PATH variable in the provided shell profile.)"
    while true; do
        read -p "path to shell profile (absolute)? " PROFILE
        if [ -z $PROFILE ]; then continue; fi
        if [ -f $PROFILE ]; then break;
        else
            echo "file \"$PROFILE\" does not exist or is not a regular file."
            continue
        fi
    done

    echo -e "\ninstallation path: $PREFIX\nshell profile: $PROFILE\n"
    while true; do
        read -p "is this correct? [Y/N] " CHOICE
        case $CHOICE in
            [Yy]*) break 2;;
            [Nn]*) continue 2;;
            *) continue;;
        esac
    done
done

try mkdir -p $PREFIX
export PATH="$PREFIX/bin:$PATH"

# install dependencies
# dependency list from https://wiki.osdev.org/GCC_Cross-Compiler#Preparing_for_the_build
echo -e "\ninstalling dependencies, this will require user confirmation."
try pacman -S base-devel gmp libmpc mpfr

pushd . > /dev/null 2>&1
cd $(mktemp -d)

echo -e "\ndownloading Binutils..."
try curl -O https://ftp.gnu.org/gnu/binutils/binutils-2.44.tar.gz

echo "downloading GCC..."
try curl -O https://ftp.gnu.org/gnu/gcc/gcc-15.1.0/gcc-15.1.0.tar.gz

echo "unpacking Binutils source..."
tar -xzf binutils-2.44.tar.gz

echo "unpacking GCC source..."
tar -xzf gcc-15.1.0.tar.gz

echo
while true; do
    while true; do
        read -p "cross-compilation target? " TARGET
        if [ -z $TARGET ]; then continue; fi
        
        binutils-2.44/config.sub $TARGET > /dev/null 2>&1
        if [ $? -ne 0 ]; then echo "target not recognized."; continue; fi

        gcc-15.1.0/config.sub $TARGET > /dev/null 2>&1
        if [ $? -ne 0 ]; then echo "target not recognized."; continue; fi

        break
    done

    echo -e "\ncross-compilation target: $TARGET\n"
    while true; do
        read -p "is this correct? [Y/N] " CHOICE
        case $CHOICE in
            [Yy]*) break 2;;
            [Nn]*) continue 2;;
            *) continue;;
        esac
    done
done

echo -e "\nbuilding the toolchain will take some time. you can go brew yourself a coffee in the meantime."
read -s -n 1 -p "press any key to continue. "

echo "\nbuilding Binutils..."
pushd .
mkdir binutils-build
cd binutils-build
../binutils-2.44/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
try make
try make install

echo -e "\nbuilding GCC..."
popd
mkdir gcc-build
cd gcc-build
../gcc-15.1.0/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers --disable-hosted-libstdcxx
try make -j 8 all-gcc
try make all-target-libgcc
try make all-target-libstdc++-v3
try make install-gcc
try make install-target-libgcc
try make install-target-libstdc++-v3

echo -e "\nappending installation path to PATH..."
try echo -e "\n# cross-compilation toolchain for $TARGET\nexport PATH=\"$PREFIX/bin:\$PATH\"" >> $PROFILE

echo "succesfully built a cross-compilation toolchain for $TARGET."

popd