# GTAV_SDC
Automatic Pedestrian Avoiding Vehicles in GTAV. It works ... when it does.


## Team Members
- Yilun Chen
    - Project Architect. Designed and implemented the Perception Engine. 
    - Helped implementing and debugging the Keyboard Emulator and Data Visualization Engine.
- Jingxuan Wang
    - Designed and implemented the Keyboard Emulator and the Screen Capturer.
    - Designed customized byte packet format to efficiently receive command.
- Yida Wang
    - Designed and implemented the Data Visualization Engine.
    - Boosted capabilities of the engine to support live rendering of multiple objects.
- Bruce Yang
    - Designed and implemented the Data Ingestion Engine.
    - Significantly improved performance of Ingestion Engine by 8-10 times.

## Contents from ECE4122/6122 Used In This Project
- __Classes__: Used by Data Visualization Engine to draw different objects.
- __Sockets__: Used by Data Ingestion Engine, Keyboard Emulator and Data Visualization Engine. 
- __Threading__: Used by Data Visualization Engine. 
- __OpenGL__: Used by Data Visualization Engine.

## Installation
- GTAV_SDC runs on Windows only.
- Make sure to use python 3.7
- python -m pip install -r requirements.txt
- Install protobuf https://github.com/protocolbuffers/protobuf
- Download contents of https://github.com/tensorflow/models into tensorflow directory and add that directory to environment variables.
- (Optional) Install CUDA (for GPU acceleration)

- Install GTAV and run it in windowed mode, resolution 800*600, and pull it to the top left of the screen.
- Run `.\GDIPlus_ScreenCap.exe` to start taking screenshots.
- Run `python3 .\obj_detection.py` to start the Data Ingestion Engine and the Perception Engine.
- Run `.\KeyBoardEmulation.exe` to start the Keyboard Emulator.
- Run `.\visualization_engine\release\visualization_engine.exe` to start the Visualization Engine.
- GTAV_SDC should be fully up and running. Try running into pedestrians and watch GTAV_SDC save them all!
