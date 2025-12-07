import asyncio
import mysql.connector
from datetime import datetime
from bleak import BleakClient
import struct
from dotenv import load_dotenv
import os

load_dotenv()

DEVICE_ADDRESS = os.getenv("DEVICE_ADDRESS")
CHARACTERISTIC_UUID = os.getenv("CHARACTERISTIC_UUID")
DB_HOST = os.getenv("DB_HOST")
DB_USER = os.getenv("DB_USER")
DB_PASS = os.getenv("DB_PASS")
DB_NAME = os.getenv("DB_NAME")

def save_to_database(x, y, z, orientation):
    conn = None
    cursor = None
    try:
        conn = mysql.connector.connect(
            host=DB_HOST,
            port=3306,
            user=DB_USER,
            password=DB_PASS,
            database=DB_NAME,
            connection_timeout=10
        )
        cursor = conn.cursor()

        sql = """
        INSERT INTO rawdata (sensorvalue_x, sensorvalue_y, sensorvalue_z, orientation) VALUES (%s, %s, %s, %s);
        """

        values = (x, y, z, orientation)
        cursor.execute(sql, values)
        conn.commit()
        print(f"[{datetime.now().strftime('%H:%M:%S')}] Tallennettu -> X:{x}, Y:{y}, Z:{z}, Orientation:{orientation}")

    except Exception as e:
        print("Virhe tietokantayhteydessä:", e)
    finally:
        if cursor:
            cursor.close()
        if conn:
            conn.close()

def notification_handler(sender, data):
    try:
        if len(data) != 8:
            print(f"Odottamaton pituus {len(data)}, ohitetaan.")
            return

        x, y, z, orientation = struct.unpack('<hhhh', data)

        save_to_database(x, y, z, orientation)

    except Exception as e:
        print(f"Virhe datan käsittelyssä: {e}")

async def main():
    print(f"Yhdistetään Nordic-laitteeseen ({DEVICE_ADDRESS})...")

    try:
        async with BleakClient(DEVICE_ADDRESS) as client:
            if not client.is_connected:
                print("Yhteys ei onnistunut.")
                return

            print("Yhteys avattu Nordic-laitteeseen!")
            await client.start_notify(CHARACTERISTIC_UUID, notification_handler)

            print("Kuunnellaan BLE-dataa... (Ctrl+C lopettaa)")
            while True:
                await asyncio.sleep(1)

    except Exception as e:
        print("Virhe BLE-yhteydessä:", e)

if __name__ == "__main__":
    try:
        asyncio.run(main())
    except KeyboardInterrupt:
        print("\nOhjelma lopetettu.")
