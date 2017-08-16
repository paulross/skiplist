import functools

@functools.total_ordering
class TotalOrdered:#(object):
    """A class that has Python total ordering on a Python object."""
    def __init__(self, value):
        self._value = value
    
    @property
    def value(self):
        return self._value

    def __eq__(self, other):
        if not hasattr(other, '_value') or callable(getattr(other, '_value')):
            return NotImplemented
        return self._value == other._value

    def __lt__(self, other):
        if not hasattr(other, '_value') or callable(getattr(other, '_value')):
            return NotImplemented
        return self._value < other._value

class OrderedLt:#(object):
    """A class that has just __lt__ ordering a Python object."""
    def __init__(self, value):
        self._value = value

    def __lt__(self, other):
        if not hasattr(other, '_value') or callable(getattr(other, '_value')):
            return NotImplemented
        return self._value < other._value

