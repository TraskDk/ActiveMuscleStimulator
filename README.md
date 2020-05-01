# ActiveMuscleStimulator
This is the backing project for the Active Muscle Stimulator device from https://blog.trask.dk.

Development efforts are initially done using Visual Studio 2019 using the Cross Platform feature.
This enables development and testing of essential algorithms on Windows, while at the same time
enabling hardware specific features on the Raspberry device.

Code that is shared between the Windows test application and the Raspberry service application
can be put in the SharedItems project, and will be automatically included in both applications.
