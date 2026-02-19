import pytest

from . import benchmark_to_dat


@pytest.mark.parametrize(
    'function_name, expected',
    (
            (
                    'test_rolling_median_sh_mem_create_copy[1048576]',
                    ('test_rolling_median_sh_mem_create_copy', 1048576,),
            ),
            (
                    'test_rolling_median_sh_mem_create_copy[1048576-16]',
                    ('test_rolling_median_sh_mem_create_copy', 1048576 * 16,),
            ),
    )
)
def test_split_name_and_scale(function_name, expected):
    result = benchmark_to_dat.split_name_and_scale(function_name)
    assert result == expected
