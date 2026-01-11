## Description
RfidLock is designed for ESP32-WROOM dev module. It allows an ESP32 to rotate a servo when an approved UID is detected over a RC522 RFID module.
By connected the servo to a lock/deadbolt this can be used to create an RFID based lock.


## Pinout
#### Servo
```
Vin -> Servo power
Gnd -> Servo and RC522 Gnd
D25 -> Servo signal
```
#### RC522
```
3v3 -> 3.3
D18 -> SCK
D19 -> MISO
D21 -> SDA
D22 -> RST
D23 -> MOSI
```

