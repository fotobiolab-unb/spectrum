#!/bin/bash

# Base image setup
echo "Setting up base image..."
export DEBIAN_FRONTEND=noninteractive
apt update
apt upgrade -y
apt install -y curl git unzip

# Install Arduino CLI
echo "Installing Arduino CLI..."
curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | BINDIR=/usr/local/bin sh

# Update library index
echo "Updating library index..."
arduino-cli lib update-index -v

# Login to GitHub
echo "Logging in to GitHub..."
gh auth login --with-token < .token

# Install libraries
echo "Installing libraries..."
arduino_home=$(arduino-cli config dump | grep 'user:' | awk '{print $2}')
gh release download v0 --archive=zip --repo fotobiolab-unb/arduino-libraries
mv arduino-libraries-0.zip libraries.zip
unzip libraries.zip -d $arduino_home
rm libraries.zip
mv $arduino_home/arduino-libraries-0 $arduino_home/arduino-libraries

# Download source code
echo "Downloading source code..."
gh release download v0 --archive=zip --repo fotobiolab-unb/spectrum
mv spectrum-0.zip spectrum.zip
unzip spectrum.zip -d $HOME
rm spectrum.zip
mv $HOME/spectrum-0 $HOME/spectrum

# Build image
echo "Building image..."
arduino-cli compile -b arduino:avr:mega $HOME/spectrum/src
arduino-cli upload -t -b arduino:avr:mega -p "$(arduino-cli board list | grep 'USB' | awk '{print $1}')" $HOME/spectrum/src
