#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cstdint>
#include <stdexcept>
#include <string>

class AIGReader {
private:
    uint32_t num_inputs;
    uint32_t num_latches;
    uint32_t num_outputs;
    uint32_t num_and_gates;
    uint32_t total_nodes;

    using and_gate_map_t = pybind11::dict;

    std::vector<uint32_t> input_literals;
    std::vector<uint32_t> next_state_literals;
    std::vector<uint32_t> output_literals;
    and_gate_map_t and_gates;

    // Read a byte from the file
    uint8_t get_byte(std::ifstream &file) {
        int ch = file.get();
        if (ch == EOF) {
            throw std::runtime_error("Unexpected EOF");
        }
        return static_cast<uint8_t>(ch);
    }

    // Decode a variable-length integer from the file
    uint32_t decode_uint(std::ifstream &file) {
        uint32_t x = 0;
        uint32_t i = 0;
        uint8_t ch;
        while ((ch = get_byte(file)) & 0x80) {
            x |= (ch & 0x7f) << (7 * i++);
        }
        return x | (ch << (7 * i));
    }

    void read_header(std::ifstream &file) {
        // Check if the file is an AIGER binary file
        char format[4];
        file.read(format, 3);
        format[3] = '\0';
        if (std::string(format) != "aig") {
            throw std::runtime_error("Not an AIGER binary file");
        }

        file >> total_nodes >> num_inputs >> num_latches >> num_outputs >> num_and_gates;

        if (file.fail()) {
            throw std::runtime_error("Failed to read header values");
        }

        if (total_nodes != num_inputs + num_latches + num_and_gates) {
            throw std::runtime_error("M != I + L + A");
        }

        file.get(); 
    }

    void read_next_state_literals(std::ifstream &file) {
        next_state_literals.resize(num_latches);
        for (uint32_t i = 0; i < num_latches; ++i) {
            file >> next_state_literals[i];
            if (file.fail()) {
                throw std::runtime_error("Failed to read next state literal for latch " + std::to_string(i));
            }
        }
    }

    void read_output_literals(std::ifstream &file) {
        output_literals.resize(num_outputs);
        for (uint32_t i = 0; i < num_outputs; ++i) {
            file >> output_literals[i];
            if (file.fail()) {
                throw std::runtime_error("Failed to read output literal for output " + std::to_string(i));
            }
        }
    }

    void read_and_gates(std::ifstream &file) {
        for (uint32_t i = 0; i < num_and_gates; ++i) {
            uint32_t delta0 = decode_uint(file);
            uint32_t delta1 = decode_uint(file);
            auto lhs = 2 * (num_inputs + num_latches + i + 1);
            and_gates[pybind11::cast(lhs)] = pybind11::make_tuple(delta0, delta1);
        }
    }

public:
    // 读取 AIG 文件
    void read_aig_file(const std::string& filename) {
        std::ifstream file(filename, std::ios::binary);
        if (!file) {
            throw std::runtime_error("Could not open file '" + filename + "'");
        }

        read_header(file);
        read_next_state_literals(file);
        read_output_literals(file);
        read_and_gates(file);

        // Set input literals
        for (uint32_t i = 0; i < num_inputs; ++i) {
            input_literals.push_back(i * 2);
        }

        file.close();
    }

    // Getters for circuit metadata
    uint32_t get_num_inputs() const { return num_inputs; }
    uint32_t get_num_latches() const { return num_latches; }
    uint32_t get_num_outputs() const { return num_outputs; }
    uint32_t get_num_and_gates() const { return num_and_gates; }
    uint32_t get_total_nodes() const { return total_nodes; }

    // Getters for circuit components
    const std::vector<uint32_t>& get_inputs() const { return input_literals; }
    const std::vector<uint32_t>& get_next_state_literals() const { return next_state_literals; }
    const std::vector<uint32_t>& get_outputs() const { return output_literals; }
    const and_gate_map_t& get_and_gates() const { return and_gates; }
};

// Pybind11 module definition
PYBIND11_MODULE(aiger_fast_reader, m) {
    pybind11::class_<AIGReader>(m, "AIGReader")
        .def(pybind11::init<>())
        .def("read", &AIGReader::read_aig_file)
        .def("num_inputs", &AIGReader::get_num_inputs)
        .def("num_latches", &AIGReader::get_num_latches)
        .def("num_outputs", &AIGReader::get_num_outputs)
        .def("num_and_gates", &AIGReader::get_num_and_gates)
        .def("num_total_nodes", &AIGReader::get_total_nodes)
        .def("get_inputs", &AIGReader::get_inputs)
        .def("get_and_gates", &AIGReader::get_and_gates)
        .def("get_outputs", &AIGReader::get_outputs);
}
