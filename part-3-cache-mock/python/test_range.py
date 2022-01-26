from unittest import TestCase, mock
from range import Range

class TestRange(TestCase):
    def test_init(self):
        r = Range(10, 20)
        self.assertEqual(10, r.offset)
        self.assertEqual(20, r.size)
    
    def test_end(self):
        r = Range(10, 20)
        self.assertEqual(30, r.end())
    
    def test_overlap(self):
        r1 = Range(0, 10)
        r2 = Range(10, 20)
        r3 = Range(5, 20)
        r4 = Range(30, 20)
        self.assertFalse(Range.overlap(r1, r2))
        self.assertTrue(Range.overlap(r1, r3))
        self.assertTrue(Range.overlap(r2, r3))
        self.assertFalse(Range.overlap(r2, r4))
        self.assertFalse(Range.overlap(r3, r4))
        self.assertFalse(Range.overlap(r3, r4))

    def test_exclude_middle(self):
        left, right = Range.exclude(Range(0, 20), Range(5, 5))
        self.assertEqual(Range(0, 5), left)
        self.assertEqual(Range(10, 10), right)

    def test_exclude_left(self):
        left, right = Range.exclude(Range(0, 20), Range(0, 5))
        self.assertEqual(Range(0, 0), left)
        self.assertEqual(Range(5, 15), right)

    def test_exclude_right(self):
        left, right = Range.exclude(Range(0, 20), Range(15, 5))
        self.assertEqual(Range(0, 15), left)
        self.assertEqual(Range(20, 0), right)
    
    def test_intersect_left(self):
        res = Range.intersect(Range(0,15), Range(10, 10))
        self.assertEqual(Range(10, 5), res)

    def test_intersect_middle(self):
        res = Range.intersect(Range(12,5), Range(10, 10))
        self.assertEqual(Range(12, 5), res)
    
    def test_intersect_right(self):
        res = Range.intersect(Range(15,20), Range(10, 10))
        self.assertEqual(Range(15, 5), res)
