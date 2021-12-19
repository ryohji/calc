"""test-factorial.py"""
import unittest
import operator
import functools


def factorial(x: int) -> int:
    """Returns factorial of `x`."""
    return functools.reduce(operator.mul, list(range(x+1)[1:])) if x else 1


class TestFactorial(unittest.TestCase):
    """Test factorial implementation."""

    def test_handles_zero_input(self):
        """Tests factorial of 0."""
        self.assertEqual(1, factorial(0))

    def test_factorial_of_1_is_1(self):
        """Tests factorial of 1."""
        self.assertEqual(1, factorial(1))

    def test_factorial_of_2_is_2(self):
        """Tests factorial of 2."""
        self.assertEqual(2, factorial(2))

    def test_factorial_of_3_is_6(self):
        """Tests factorial of 3."""
        self.assertEqual(6, factorial(3))

    def test_factorial_of_8_is_40320(self):
        """Tests factorial of 8."""
        self.assertEqual(40320, factorial(8))
