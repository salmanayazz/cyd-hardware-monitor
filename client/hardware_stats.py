import psutil

try:
    from pynvml import (
        nvmlInit, nvmlDeviceGetHandleByIndex, nvmlDeviceGetTemperature,
        nvmlDeviceGetUtilizationRates, nvmlShutdown
    )
    import clr
    clr.AddReference(r'OpenHardwareMonitorLib')
    from OpenHardwareMonitor.Hardware import Computer
    import subprocess
except ImportError as e:
    print(f"Failed to import optional modules: {e}")

def get_cpu_temp():
    if 'Computer' in globals():
        try: # windows
            c = Computer()
            c.CPUEnabled = True
            c.Open()

            for hardware in c.Hardware:
                hardware.Update()
                for sensor in hardware.Sensors:
                    if "temperature" in str(sensor.Identifier).lower():
                        return round(sensor.get_Value())
                        
        except Exception as e: 
            print(f"Failed to fetch CPU temp: {e}")
    else:
        try: # linux
            total = 0
            len = 0

            for temp in psutil.sensors_temperatures().get('coretemp'):
                total += temp.current
                len += 1

            return round(total / len)
        except Exception as e:
            print(f"Failed to fetch CPU temp: {e}")

    return None

def get_cpu_util():
    return round(psutil.cpu_percent(interval=0.1))

def get_gpu_stats():
    gpu_temp, gpu_util = None, None
    try:
        nvmlInit()
        handle = nvmlDeviceGetHandleByIndex(0)
        gpu_temp = nvmlDeviceGetTemperature(handle, 0)
        gpu_util = nvmlDeviceGetUtilizationRates(handle).gpu
    except Exception as e:
        print(f"Failed to fetch GPU stats: {e}")
    
    return round(gpu_temp), round(gpu_util)

def get_memory_util():
    return round(psutil.virtual_memory().percent)

def get_fps_stats():
    presentmon_proc = subprocess.Popen([
            r".\PresentMon-2.3.0-x64.exe", 
            "--output_stdout",
            "--stop_existing_session",
            "--timed", "2",
            "--terminate_after_timed",
            "--v1_metrics"
        ],
        stdout=subprocess.PIPE, stderr=subprocess.PIPE
    )

    frame_times_sum = 0
    frames = 0
    process = None
    for line in iter(presentmon_proc.stdout.readline, b''):
        try:
            line = line.decode('utf-8').strip()
            
            if 'exe' in line:
                parts = line.split(',')
                if len(parts) > 13:
                    if process is None:
                        process = parts[0]

                    if process == parts[0] and float(parts[14]) > 0:
                        frame_times_sum += float(parts[14])
                        
                        # dont include dropped frames in count
                        if (not bool(int(parts[6]))): 
                            frames += 1

                    if frame_times_sum > 1000:
                        break

        except Exception as e:
            print(f"Error parsing line: {e}")

    try:
        fps = 1000 / (frame_times_sum / frames)
    except ZeroDivisionError:
        print(f"Error calculating FPS: {frame_times_sum}, {frames}")
        fps = 0
    
    presentmon_proc.kill()
    presentmon_proc.wait()

    return round(fps), process

def get_cpu_util():
    return round(psutil.cpu_percent(interval=0.1))

def get_gpu_stats():
    gpu_temp, gpu_util = None, None
    try:
        nvmlInit()
        handle = nvmlDeviceGetHandleByIndex(0)
        gpu_temp = nvmlDeviceGetTemperature(handle, 0)
        gpu_util = nvmlDeviceGetUtilizationRates(handle).gpu
    except Exception as e:
        print(f"Failed to fetch GPU stats: {e}")
    
    return round(gpu_temp), round(gpu_util)

def get_memory_util():
    return round(psutil.virtual_memory().percent)

def get_fps_stats():
    presentmon_proc = subprocess.Popen([
            r".\PresentMon-2.3.0-x64.exe", 
            "--output_stdout",
            "--stop_existing_session",
            "--timed", "2",
            "--terminate_after_timed",
            "--v1_metrics"
        ],
        stdout=subprocess.PIPE, stderr=subprocess.PIPE
    )

    frame_times_sum = 0
    frames = 0
    process = None
    for line in iter(presentmon_proc.stdout.readline, b''):
        try:
            line = line.decode('utf-8').strip()
            
            if 'exe' in line:
                parts = line.split(',')
                if len(parts) > 13:
                    if process is None:
                        process = parts[0]

                    if process == parts[0] and float(parts[14]) > 0:
                        frame_times_sum += float(parts[14])
                        
                        # dont include dropped frames in count
                        if (not bool(int(parts[6]))): 
                            frames += 1

                    if frame_times_sum > 1000:
                        break

        except Exception as e:
            print(f"Error parsing line: {e}")

    try:
        fps = 1000 / (frame_times_sum / frames)
    except ZeroDivisionError:
        print(f"Error calculating FPS: {frame_times_sum}, {frames}")
        fps = 0
    
    presentmon_proc.kill()
    presentmon_proc.wait()

    return round(fps), process