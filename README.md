# HPC Disease Propagation Simulation

A C++/MPI implementation of a disease spread model with herd immunity analysis, containerized via Docker.

## Table of Contents
- [Project Overview](#project-overview)
- [Requirements](#requirements)
- [Installation](#installation)
- [Usage](#usage)
- [Configuration](#configuration)
- [Output Files](#output-files)
- [Task 2: Herd Immunity Experiment](#task-2-herd-immunity-experiment)
- [Expected output](#expected-output)

## Project Overview
Implements a parallelized disease spread simulation with:
- Individual-based SIRV model (Susceptible, Infectious, Recovered, Vaccinated)
- Multi-population interactions
- MPI parallelization
- Docker containerization
- Herd immunity threshold analysis (Task 2)

## Requirements
- Docker Engine (Ubuntu Jammy 22.04 LTS recommended)
- MPI runtime (included in Docker image)
- 4GB+ RAM for large populations

## Installation
1. Clone repository:
   ```bash
   git clone https://mygit.th-deg.de/me27164/hpc-disease-simulation.git
    ```

2. Build Docker image:
   ```bash
    cd /hpc-disease-simulation
    docker build . -t submission:latest
    docker run submission:latest
    ```
## Usage
1. Prepare disease_in.ini configuration file (see example below)

2. Run simulation:
    ```bash
    # make sure to have the .ini configuration in the current
    # working directory
    docker run -v .:/scratch submission:latest 
    ```

## Configuration
Example disease_in.ini:

    [global]
    simulation_name = my_simulation    ; A identifier for the simulation
    num_populations = 4                ; total number of populations to model
    simulation_runs = 5                ; total number of runs 

    [disease]                          ; Global disease configuration
    name = "Corona"                    ; Name of the disease
    duration = 5                       ; Days a person is infectious 
    transmissability = 0.1             ; Probability of the disease being 
                                    ;   transmitted on contact
                                    ;   (0.0 = 0%, 1.0 = 100%)

    ; For each population a section is added
    [population_1]                     ; Use "population_" plus number        
    name = Deggendorf                  ; Name of the population, e.g.,
                                    ;   a city or neighbourhood  
    size = 4000                        ; Number of persons in the population
    vaccination_rate = 0.9             ; Fraction of persons vaccinated 
                                    ;   (0.0 = 0%, 1.0 = 100%)
    patient_0 = true                   ; If true, the population starts 
                                    ;   with a single infectious person          

    [population_2]                     ; see population_1
    name = Regensburg
    size = 15000           
    vaccination_rate = 0.0  
    patient_0 = false       

    [population_3]
    name = Zwiesel  
    size = 1500           
    vaccination_rate = 0.0  
    patient_0 = false       

## Output Files

- disease_stats.csv: Summary statistics

- disease_details.csv: Per-timestep details

- Terminal output showing runtime metrics

## Task 2: Herd Immunity Experiment
To reproduce the analysis:

1. Set num_populations = 1 and size = 15000
2. Sweep vaccination rates from 0-100% in 10% increments
3. Calculate average recovered persons vs vaccination rate
4. Compare with theoretical threshold from R₀ calculation

## Expected output

- Plot of final recovered vs vaccination rate
- Timestep-wise progression plot
- Comparison with theoretical herd immunity threshold