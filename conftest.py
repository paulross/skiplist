import pytest


def pytest_addoption(parser):
    parser.addoption(
        "--runslow", action="store_true", default=False, help="run slow tests"
    )
    parser.addoption(
        "--runveryslow", action="store_true", default=False,
        help="run very slow tests (usually benchmarks on large data)"
    )


def pytest_configure(config):
    config.addinivalue_line("markers", "slow: mark test as slow to run")
    config.addinivalue_line("markers", "veryslow: mark test as veryslow to run")


def pytest_collection_modifyitems(config, items):
    if config.getoption("--runslow"):
        # --runslow given in cli: do not skip slow tests
        return
    if config.getoption("--runveryslow"):
        # --runslow given in cli: do not skip slow tests
        return
    skip_slow = pytest.mark.skip(reason="need --runslow option to run")
    for item in items:
        if "slow" in item.keywords:
            item.add_marker(skip_slow)
    skip_veryslow = pytest.mark.skip(reason="need --runveryslow option to run")
    for item in items:
        if "veryslow" in item.keywords:
            item.add_marker(skip_veryslow)

