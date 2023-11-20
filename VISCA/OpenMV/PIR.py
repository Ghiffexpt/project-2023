import pyb

# Konfigurasi pin sebagai input
pin = pyb.Pin("D0", pyb.Pin.IN)

# Membaca nilai digital dari pin
value = pin.value()
# Melakukan sesuatu dengan nilai yang dibaca
if value == 0:
    print("Nilai pin adalah LOW")
if value == 1:
    print("Nilai pin adalah HIGH")
