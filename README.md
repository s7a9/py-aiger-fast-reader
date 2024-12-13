# Minimum AIGER Fast Reader

A fast Python library for reading AIGER circuit files `.aig` without any dependency.

## Installation

```bash
pip install .
```

## Usage

```python
from aiger_fast_reader import AIGReader

# Create an AIGReader instance
reader = AIGReader()

# Read an AIG file
reader.read("path/to/your/circuit.aig")

# Get circuit metadata
print(f"Number of inputs: {reader.num_inputs()}")
print(f"Number of outputs: {reader.num_outputs()}")
print(f"Number of AND gates: {reader.num_and_gates()}")
print(f"Number of latches: {reader.num_latches()}")
print(f"Number of total nodes: {reader.num_total_nodes()}")

# Access circuit components as python native types
inputs = reader.get_inputs()
and_gates = reader.get_and_gates()
outputs = reader.get_outputs()
```

## Requirements

- Python 3.6+
- pybind11
- CMake 3.10+

## Building from Source

```bash
pip install .
```

## License

MIT License
