import mysql.connector
from dotenv import load_dotenv
import os

load_dotenv()

DB_HOST = os.getenv("DB_HOST")
DB_USER = os.getenv("DB_USER")
DB_PASS = os.getenv("DB_PASS")
DB_NAME = os.getenv("DB_NAME")

def hae_tiedot():
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
            print("Tietokannassa ei ole dataa.")
            return

        print("---- VIIMEISET 20 RIVIÄ ----")
        for r in rows:
            print(
                f"X:{r['sensorvalue_x']} "
                f"Y:{r['sensorvalue_y']} "
                f"Z:{r['sensorvalue_z']} "
                f"ORI:{r['orientation']}"
            )

    except Exception as e:
        print("Virhe tietokantayhteydessä:", e)

    finally:
        try:
            cursor.close()
            conn.close()
        except:
            pass

if __name__ == "__main__":
    hae_tiedot()
