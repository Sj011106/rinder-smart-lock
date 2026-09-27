import serial
import time
from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware

app = FastAPI()

# Allow the Rinder frontend to communicate with this API
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=False,
    allow_methods=["*"],
    allow_headers=["*"],
)

# Connect to Arduino - update "COM9" if Device Manager showed a different port
arduino = serial.Serial("COM9", 9600, timeout=1)

# Arduino resets when serial connection opens
time.sleep(2)

print("Rinder Smart Lock connected on COM9")


@app.get("/")
def home():
    return {"status": "Rinder Smart Lock API running"}


@app.post("/unlock")
def unlock():
    arduino.write(b'U')

    print("Rinder unlock request received")

    return {
        "success": True,
        "message": "Door unlocked",
        "autoLock": "5 seconds"
    }