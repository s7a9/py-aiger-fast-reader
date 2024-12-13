# AIGER Fast Reader

A fast Python library for reading AIGER circuit files `.aig`.

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
reader.read_aig_file("path/to/your/circuit.aig")

# Get circuit metadata
print(f"Number of inputs: {reader.get_num_inputs()}")
print(f"Number of outputs: {reader.get_num_outputs()}")

# Access circuit components
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
