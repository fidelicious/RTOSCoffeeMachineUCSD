import ctypes
from time import sleep

''' Parameters '''
DLL = "./FirmwareApi.dll"
COM = b"COM6"
BAUD = 115200

''' Connection '''
coffee_api = ctypes.WinDLL(DLL)                     # Load the DLL

if coffee_api.initialize_uart(COM, BAUD):           # Initialize UART
    print("UART initialized successfully!")
else:
    print("UART initialized Failed!")

''' API Execution '''
result = coffee_api.get_pwr_status()
print("Power Status:", result)

sleep(3)

if coffee_api.toggle_power():
    print("Power Toggled Successfully")
else:
    print("Power Toggling Failed!")

sleep(3)

result = coffee_api.get_pwr_status()
print("Power Status:", result)

'''
status = (ctypes.c_uint8 * 3)()                     # Get Status
if coffee_api.get_status(status, len(status)):
    print(f"Power: {status[0]}, Strong Brew: {status[1]}, Water Level: {status[2]}")

if coffee_api.set_water_level(50):                  # Set Water Level
    print("Water level set successfully")
'''

coffee_api.close_uart()                             # Close UART

