#!/usr/bin/env python3
import time
import warnings

class MeasureTime:
    def __init__(self):
        self.start_time = 0
        self.stop_time = 0

    def start(self):
        self.start_time = time.perf_counter()

    def stop(self):
        self.stop_time = time.perf_counter()

    def get(self):
        if self.start_time == 0 or self.stop_time == 0:
            warnings.warn("Measurement not started or stopped",
                          RuntimeWarning)
            return 0
        result = self.stop_time - self.start_time
        if result < 0:
            warnings.warn("Timer rolled over or start and stop called in wrong order",
                          RuntimeWarning)
            return 0
        else:
            return result
