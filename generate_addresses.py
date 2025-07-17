import random

NUM_ADDRESSES = 5000  # Change this value for total addresses

def generate_addresses(filename):
    with open(filename, 'w') as f:
        for _ in range(NUM_ADDRESSES):
            mode = random.choice(['random', 'repeat', 'sequential'])
            if mode == 'random':
                addr = random.randint(0, 65535)
            elif mode == 'repeat':
                addr = random.choice([1234, 4321, 9999, 8888, 7777])  # Frequent hits
            else:
                base = random.randint(0, 256) * 256
                offset = random.randint(0, 255)
                addr = base + offset  # Simulates sequential access

            f.write(f"{addr}\n")

if __name__ == "__main__":
    generate_addresses("addresses.txt")
    print("Generated new addresses.txt")