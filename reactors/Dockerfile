FROM ubuntu:latest AS base

#Disable frontend
ENV DEBIAN_FRONTEND=noninteractive

# Update and install dependencies
RUN apt update
RUN apt install -y curl git unzip gh
RUN curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | BINDIR=/usr/local/bin sh
RUN arduino-cli lib update-index -v

#Login to github
COPY .token /root/.token
RUN gh auth login --with-token < /root/.token

# Install libraries
WORKDIR /root
RUN gh release download v0 --archive=zip --repo fotobiolab-unb/arduino-libraries
RUN mv arduino-libraries-0.zip libraries.zip
RUN unzip libraries.zip -d $(arduino-cli config dump | grep 'user:' | awk '{print $2}')
RUN rm libraries.zip
RUN mv $(arduino-cli config dump | grep 'user:' | awk '{print $2}')/arduino-libraries-0 $(arduino-cli config dump | grep 'user:' | awk '{print $2}')/arduino-libraries
#Download source code
WORKDIR /root
RUN gh release download v0 --archive=zip --repo fotobiolab-unb/spectrum
RUN mv spectrum-0.zip spectrum.zip
RUN unzip spectrum.zip -d /root
RUN rm spectrum.zip
RUN mv /root/spectrum-0 /root/spectrum

FROM base AS build
#Compile and upload
WORKDIR /root/spectrum/src
RUN arduino-cli compile -b arduino:avr:mega .
RUN arduino-cli upload -t -b arduino:avr:mega -p $(arduino-cli board list | grep "USB" | awk '{print $1}') .