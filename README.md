# Embedded Hardware Monitor

This project is an embedded hardware monitor system that collects and displays system metrics. It consists of a client application running on your Windows or Linux host and an embedded device that collects system metrics and sends them to the client application. The application is designed to show statistics of a Primary device (Windows) and a Secondary device (Linux) on the same screen.

## Features
### Primary Device (Windows)
- CPU Usage & Temperature
- GPU Usage & Temperature (NVIDIA only)
- Memory Usage
- FPS (Frames Per Second) of currently active window
### Secondary Device (Linux)
- CPU Usage & Temperature
- Memory Usage

# Setup Instructions

## Client Setup

### Initial Setup

1. Open ```main.py``` in the ```client``` directory of the project.

2. Modify the ```BLE_ADDRESS``` variable to the MAC address of the embedded device.

3. Set the ```PRIMARY_DEVICE``` to ```True``` / ```False```. Currently, the primary device has to be Windows while the secondary device has to be Linux.

### Windows

First, you will need to unblock the OpenHardwareMonitorLib.dll file to allow the client application to access system metrics.

1. Right-click on the OpenHardwareMonitorLib.dll file in the ```client``` directory.

2. Click on Properties.

3. Check the Unblock checkbox at the bottom of the General tab.

4. Click OK.

Now its time to start the client application. I developed the project with Python 3.13.1, so I recommend using this version.

1. Open PowerShell as Administrator.

2. From the project root, navigate to the client directory of the project:
```
cd client
```

3. Install the required Python packages:
```
pip install -r requirements.txt
```

4. Run the provided Task Scheduler script to have the client application to auto run on startup:

```
.\windows_task_scheduler.ps1
```

### Linux

1. Navigate to the project root directory.

2. Run the Docker Compose file to start the client services:

```
docker compose up -d --build
```

## Embedded Device Setup

The embedded project was built and tested on the CYD development board mentioned [here](https://github.com/witnessmenow/ESP32-Cheap-Yellow-Display).

Since display configurations were adapted from the examples in this project, you may need to adjust settings if using a different device.


### Setup Steps

1. Ensure you have PlatformIO installed.

2. From the project root, navigate to the embedded directory of the project:

```
cd path/to/project/embedded
```

3. Build and upload the embedded project:

```
pio run --target upload
```

