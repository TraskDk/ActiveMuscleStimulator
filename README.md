# ActiveMuscleStimulator
This is the backing project for the Active Muscle Stimulator device from https://blog.trask.dk.

Development efforts are initially done using Visual Studio 2019 using the Cross Platform feature.
This enables development and testing of essential algorithms on Windows, while at the same time
enabling hardware specific features on the Raspberry device.

Code that is shared between the Windows test application and the Raspberry service application
can be put in the SharedItems project, and will be automatically included in both applications.

## Todo-List for Release Version Software

### Architecture

- [ ] Create developent platform, which allows for abstract testing in Windows.
  - [ ] Ensure the abstract platform component is documented.
  - [ ] Ensure we have established ways of testing the math from data files under Windows.

### Hardware

- [x] Add driver for Sb Components Relay 4 Zero 3v 4 Channel.
  - [x] Ensure driver quality (resource consumption).
  - [x] Ensure the driver is tested.
  - [x] Ensure the driver is documented. 
- [x] Add driver for Wave Share Sense HAT (B) accelerometer + gyroscope.
  - [x] Ensure driver quality (resource consumption).
  - [x] Ensure the driver is tested.
  - [x] Ensure the driver is documented. 
- [ ] Add driver for controller.
  - [ ] Ensure driver quality (resource consumption).
  - [ ] Ensure the driver is tested.
  - [ ] Ensure the driver is documented. 

### Math

- [ ] Add fast translation applicator (translating movement into muscle action).
  - [ ] Ensure the module quality (resource consumption).
  - [ ] Ensure the modele is tested.
  - [ ] Ensure the module is documened.
- [ ] Add translation learner (teaching or extending models with movement instructions).
  - [ ] Ensure the module quality (resource consumption).
  - [ ] Ensure the modele is tested.
  - [ ] Ensure the module is documened.
- [ ] Ensure that all tranlation logic is abstract, tested, and documented.
  - [ ] Ensure the module quality (resource consumption).
  - [ ] Ensure the modele is tested.
  - [ ] Ensure the module is documened.

### Service Application

- [ ] Add control logic for the service application.
- [ ] Ensure that service application application logic is documented.
