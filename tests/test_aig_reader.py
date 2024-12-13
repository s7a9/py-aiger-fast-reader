import unittest
from aiger_fast_reader import AIGReader
import os

class TestAIGReader(unittest.TestCase):
    def setUp(self):
        self.reader = AIGReader()
    
    def test_read_aig_file(self):
        test_file_path = os.path.join(os.path.dirname(__file__), 'i10.aig')
        
        if not os.path.exists(test_file_path):
            self.skipTest(f"Test AIG file not found: {test_file_path}")
        
        self.reader.read(test_file_path)
        
        # Basic assertions to validate file reading
        self.assertEqual(self.reader.num_inputs(), 257)
        self.assertEqual(self.reader.num_outputs(), 224)
        self.assertEqual(self.reader.num_latches(), 0)
        self.assertEqual(self.reader.num_and_gates(), 2675)
        
        # Check inputs, and gates have expected types
        inputs = self.reader.get_inputs()
        and_gates = self.reader.get_and_gates()
        
        self.assertIsInstance(inputs, list)
        self.assertIsInstance(and_gates, dict)


if __name__ == '__main__':
    unittest.main()
