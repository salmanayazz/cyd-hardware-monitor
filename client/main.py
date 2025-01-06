import asyncio
import connection_handler

BLE_ADDRESS = "A0:B7:65:05:68:AE"
PRIMARY_DEVICE = True

async def main():
    await connection_handler.connect(BLE_ADDRESS, PRIMARY_DEVICE)
        
if __name__ == "__main__":
    asyncio.run(main())
