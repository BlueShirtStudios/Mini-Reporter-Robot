from machine import Pin, ADC
import aioble
import asyncio
import bluetooth

xAxis = ADC(1)
yAxis = ADC(0)
joystickButton = Pin(22, Pin.IN, Pin.PULL_UP)

# Bluetooth settings
_BLE_SERVICE_UUID = bluetooth.UUID("19b10000-e8f2-537e-4f6c-d104768a1214")
_BLE_CHAR_UUID = bluetooth.UUID("19b10001-e8f2-537e-4f6c-d104768a1214")
_BLE_DEVICE_NAME = "PicoW_Controller"
_ADV_INTERVAL_US = 250_000

ble_service = aioble.Service(_BLE_SERVICE_UUID)
ble_characteristic = aioble.Characteristic(
    ble_service, _BLE_CHAR_UUID, read=True, notify=True
)
aioble.register_services(ble_service)


async def peripheral_task():
    while True:
        print("Waiting for connection...")
        # The connection only stays open INSIDE this 'async with' block,
        # so the send loop must live inside it too.
        async with await aioble.advertise(
            _ADV_INTERVAL_US,
            name=_BLE_DEVICE_NAME,
            services=[_BLE_SERVICE_UUID],
        ) as connection:
            print("Connected to:", connection.device)
            try:
                while connection.is_connected():
                    x = xAxis.read_u16()
                    y = yAxis.read_u16()
                    btn = 0 if joystickButton.value() == 0 else 1  # 0 = pressed

                    data = f"{x},{y},{btn}".encode()
                    ble_characteristic.write(data)
                    ble_characteristic.notify(connection, data)
                    print("Sent:", data)

                    await asyncio.sleep_ms(100)
            except aioble.DeviceDisconnectedError:
                pass

        print("Disconnected. Restarting advertising...")


asyncio.run(peripheral_task())