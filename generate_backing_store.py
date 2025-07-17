# generate_backing_store.py
with open("BACKING_STORE.bin", "wb") as f:
    for i in range(256):  # 256 pages
        f.write(bytes([i] * 256))  # Each page is 256 byte-size
print("✅ BACKING_STORE.bin created.")