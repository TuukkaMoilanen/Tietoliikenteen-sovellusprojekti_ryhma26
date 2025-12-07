import pandas as pd
import requests
from datetime import datetime

url = "http://172.20.241.62/measurement.php"
response = requests.get(url)
text = response.text.replace("<br>", "\n")

lines = text.strip().split("\n")

data = []
for line in lines:
    line = line.strip()
    if not line:
        continue
    try:
        parts = line.split(" - ")
        x = int(parts[0].split("X:")[1].strip())
        y = int(parts[1].split("Y:")[1].strip())
        z = int(parts[2].split("Z:")[1].strip())
        orientation = int(parts[3].split("Orientation:")[1].strip())
        data.append([x, y, z, orientation])
    except (IndexError, ValueError) as e:
        print("Skipping malformed line:", line)
        continue

df = pd.DataFrame(data, columns=["x","y","z","orientation"])

timestamp = datetime.now().strftime("%Y-%m-%d_%H-%M-%S")
df.to_csv(f"output/data_{timestamp}.csv", index=False)

print(f"Saved {len(df)} rows to output/data_{timestamp}.csv")
