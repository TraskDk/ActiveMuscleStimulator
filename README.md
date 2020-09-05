# ActiveMuscleStimulator

This is the backing project for the __Active Muscle Stimulator__ device from https://blog.trask.dk.

Development efforts are initially done using Visual Studio 2019 using the Cross Platform feature.
This enables development and testing of essential algorithms on Windows, while at the same time
enabling hardware specific features on the Raspberry device.

Code that is shared between the Windows test application and the Raspberry service application
can be put in the _SharedItems_ project, and will be automatically included in the compilations 
of both applications.

## Todo-List for Release Version Software

### Architecture

- [x] Create developent platform, which allows for abstract testing in Windows.
  - [x] Ensure the abstract platform component is documented.
  - [x] Ensure we have established ways of testing the math from data files under Windows.

### Hardware

- [x] Add driver for Sb Components Relay 4 Zero 3v 4 Channel.
  - [x] Ensure driver quality (resource consumption).
  - [x] Ensure the driver is tested (correctness).
  - [x] Ensure the driver is documented. 
- [x] Add driver for Waveshare Sense HAT (B) accelerometer + gyroscope.
  - [x] Ensure driver quality (resource consumption).
  - [x] Ensure the driver is tested (correctness).
  - [x] Ensure the driver is documented. 
- [x] Add driver for controller.
  - [x] Ensure driver quality (resource consumption).
  - [x] Ensure the driver is tested (correctness).
  - [x] Ensure the driver is documented. 

### Math

- [x] Add fast translation applicator (translating movement into muscle action).
  - [x] Ensure the module quality (resource consumption).
  - [x] Ensure the modele is tested (correctness).
  - [x] Ensure the module is documented.
- [x] Add translation learner (teaching or extending models with movement instructions).
  - [x] Ensure the module quality (resource consumption).
  - [x] Ensure the modele is tested (correctness).
  - [x] Ensure the module is documented.
- [x] Ensure that all tranlation logic is abstract, tested, and documented.
  - [x] Ensure the module quality (resource consumption).
  - [x] Ensure the modele is tested (correctness).
  - [x] Ensure the module is documented.

### Service Application

- [x] Add control logic for the service application.
- [x] Ensure that service application logic is documented.
