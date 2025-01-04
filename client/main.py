import asyncio
import connection_handler

BLE_ADDRESS = "A0:B7:65:05:68:AE"

async def main():
    await connection_handler.connect(BLE_ADDRESS)
        
if __name__ == "__main__":
    asyncio.run(main())
