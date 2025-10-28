import subprocess
import os
import random

def run_codec(mode, input_data):
    """Run the compiled C codec with mode 'c' or 'd'."""
    proc = subprocess.run(
        ['./codec', mode],
        input=input_data,
        capture_output=True,
    )
    if proc.returncode != 0:
        print(f"stderr: {proc.stderr.decode()}")
    return proc.stdout

def make_test_data():
    # handcrafted test cases
    tests = {
        "unit": b'\x41',
        "zero bytes": b'\x00' * 100,
        "no data": b'',
        "all literals": bytes(range(0, 100)),
        "repetitive": bytes([0x42] * 1000),
        "alternating": bytes([0, 1] * 500),
        "edge": bytes([127] * 45 + [0] + [127] * 25),
    }

    # Random data tests, varying bias towards repetitive sequences
    for epsilon in [0.2, 0.5, 0.8, 0.95]:
        for i in range(3):
            size = random.randint(1000, 50000)
            arr, last = [], 0
            for _ in range(size):
                if random.random() < epsilon:
                    arr.append(last)
                else:
                    last = random.randint(0, 127)
                    arr.append(last)
            tests[f"random_{i} eps={epsilon}"] = bytes(arr)

    return tests

def test_codec():
    test_cases = make_test_data()
    for name, data in test_cases.items():
        compressed = run_codec('c', data)
        decompressed = run_codec('d', compressed)
        if decompressed != data:
            print(f"❌ {name} failed! (len={len(data)}, compressed={len(compressed)})")
        else:
            if len(data) == 0:
                ratio = 1.0
            else:
                ratio = len(compressed) / len(data)
            print(f"✅ {name} passed! Compression ratio: {ratio:.2f}")

if __name__ == "__main__":
    test_codec()
        
