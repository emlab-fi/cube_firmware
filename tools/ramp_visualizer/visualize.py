import os
import subprocess
import json
import matplotlib.pyplot as plt
from enum import Enum
from typing import List, Tuple

STEPPER_RESOLUTION: int = 1600
TIM_CLOCK: int = 16000000
TIM_CLOCK_kHz: int = TIM_CLOCK // 1000

GENERATOR_BINARY = "RampingVisualizer"

def create_binary() -> str:
    base_dir = os.path.dirname(__file__)
    build_dir = os.path.join(base_dir, "build")

    files = os.listdir(base_dir)
    assert "CMakeLists.txt" in files
    if "build" not in files:
        print("Creating build directory...")
        os.makedirs(build_dir)
        print()
    
    print("Running Cmake...")
    subprocess.run(["cmake", ".."], cwd=build_dir, check=True)

    print("\nRunning Ninja...")
    subprocess.run(["ninja"], cwd=build_dir, check=True)
    
    print(f"\nBinary '{GENERATOR_BINARY}' ready")
    return os.path.join(build_dir, GENERATOR_BINARY)

def dma_to_plot(dma: List[Tuple[int, int, int]]):
    result: List[Tuple[float, float]] = []
    t = 10

    result.append((0, 0))
    result.append((t, 0))
    for arr, rcr, _ in dma:
        arr += 1
        rcr += 1
        step_time_ms = arr / TIM_CLOCK_kHz
        duration = step_time_ms * rcr
        # speed = s / t (for one step)
        speed = (1000 / STEPPER_RESOLUTION) / step_time_ms # mm/s
        result.append((t, speed))
        result.append((t + duration, speed))
        t += duration
    result.append((t, 0))
    result.append((t + 10, 0))
    return result

def transform_and_plot(data):
    for section in data:
        plt.plot(*zip(*dma_to_plot(section)), linewidth=3)

class Mode(Enum):
    SINGLE = "single"
    CORE_XY = "corexy"

def generate_dma(binary:str, mode: Mode, steps: List[int]) -> List[List[Tuple[int, int, int]]]:
    result = subprocess.run([binary, "-m", mode.value, "-s", *map(str, steps)], stdout=subprocess.PIPE)
    data = json.loads(result.stdout.decode("utf-8"))
    generated = []
    for dma in data.values():
        dma_tuple = [(dma[i], dma[i+1], dma[i+2]) for i in range(0, len(dma), 3)]
        dma_tuple.pop()
        dma_tuple.pop()
        generated.append(dma_tuple)
    return generated

if __name__ == "__main__":
    generator_bin = create_binary()
    data = []

    # Single
    data.extend(generate_dma(generator_bin, Mode.SINGLE, [224000]))
        
    # Single multiple
    # steps = [2000, 5000, 10000, 20000, 40000, 60000, 100000, 150000]
    # for i in steps:    
    #     data.extend(generate_dma(generator_bin, Mode.SINGLE, [i]))
        
    # Core XY
    # data.extend(generate_dma(generator_bin, Mode.CORE_XY, [40000, 90000]))
    
    transform_and_plot(data)

    plt.xlabel('t[ms]', fontsize=24)
    plt.ylabel('v[mm/s]', fontsize=24)
    plt.xticks(fontsize=18)
    plt.yticks(fontsize=18)  
    plt.grid(True)
    plt.show()
