FROM ubuntu:jammy as build

WORKDIR /src 
COPY simulation /src/simulation

# Combine package installations to reduce layers and clean up afterwards
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
    build-essential cmake g++ make valgrind openmpi-bin openmpi-common libopenmpi-dev && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*
	

WORKDIR /src/simulation/3_Build 

RUN ls -l /src/simulation/1_Software
RUN pwd
RUN ls -l /src/simulation

RUN make -j4 -f makefile clean_all
RUN make -j4 -f makefile all
RUN make -j4 -f makefile UnitTest


# Second stage for deployment, without any build dependencies or source files
FROM ubuntu:jammy as run 

WORKDIR /app
COPY --from=build /src/simulation/3_Build/exe/  /app/
RUN ls -l /app/
RUN ls -l /app/Sim
RUN ls -l /app/SimUT


# Install necessary runtime dependencies including OpenMPI
RUN apt-get update && \
    apt-get install -y --no-install-recommends openmpi-bin openmpi-common libopenmpi-dev && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*


RUN /app/SimUT


# Change to a working directory
WORKDIR /scratch


# Execute the simulation on image start
ENTRYPOINT ["/app/Sim"]

