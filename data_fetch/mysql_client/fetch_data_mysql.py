import mysql.connector
from dotenv import load_dotenv
import os

load_dotenv()

DB_HOST = os.getenv("DB_HOST")
DB_USER = os.getenv("DB_USER")
DB_PASS = os.getenv("DB_PASS")
DB_NAME = os.getenv("DB_NAME")

def fetch_data():
    try:
        conn = mysql.connector.connect(
            host=DB_HOST,
            user=DB_USER,
            password=DB_PASS,
            database=DB_NAME
        )
        cursor = conn.cursor(dictionary=True)

        sql = """
            SELECT sensorvalue_x, sensorvalue_y, sensorvalue_z, orientation
            FROM rawdata
            ORDER BY id DESC
            LIMIT 20;
        """

        cursor.execute(sql)
        rows = cursor.fetchall()

        if not rows:
            print("Database is empty.")
            return

        for row in rows:
            print(
                f"x:{row['sensorvalue_x']} "
                f"y:{row['sensorvalue_y']} "
                f"z:{row['sensorvalue_z']} "
                f"orientation:{row['orientation']}"
            )

    except Exception as e:
        print("Database connection error:", e)

    finally:
        try:
            cursor.close()
            conn.close()
        except:
            pass

if __name__ == "__main__":
    fetch_data()
