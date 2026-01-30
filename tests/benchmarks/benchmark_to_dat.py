"""
Converts pytest benchmarks to .dat files for gnuplot.
"""
import dataclasses
import glob
import json
import logging
import os.path
import re
import sys

import typing

logger = logging.getLogger(__file__)

BENCHMARK_DIR = os.path.join(
    os.path.dirname(__file__), os.pardir, os.pardir, '.benchmarks',
)
OUTPUT_DAT_DIR = os.path.join(
    os.path.dirname(__file__), os.pardir, os.pardir, 'docs', 'source', 'plots', 'dat',
)


@dataclasses.dataclass
class BenchmarkEnv:
    """Example JSON::

        {
            "node": "Pauls-MBP",
            "processor": "i386",
            "machine": "x86_64",
            "python_compiler": "Clang 13.0.0 (clang-1300.0.29.30)",
            "python_implementation": "CPython",
            "python_implementation_version": "3.11.1",
            "python_version": "3.11.1",
        }
    """
    node: str
    processor: str
    machine: str
    python_compiler: str
    python_implementation: str
    python_implementation_version: str
    python_version: str


def environment_from_json(j: typing.Dict[str, typing.Any]) -> BenchmarkEnv:
    machine_info = j['machine_info']
    return BenchmarkEnv(
        machine_info['node'],
        machine_info['processor'],
        machine_info['machine'],
        machine_info['python_compiler'],
        machine_info['python_implementation'],
        machine_info['python_implementation_version'],
        machine_info['python_version'],
    )


@dataclasses.dataclass
class Benchmark:
    """Example JSON::

        {
            "group": null,
            "name": "test_float_iar_begin[     512]",
            "fullname": "tests/benchmarks/test_benchmark_SkipList_floats.py::test_float_iar_begin[     512]",
            "params": {
                "length": "     512"
            },
            "param": "     512",
            "extra_info": {},
            "options": {
                "disable_gc": false,
                "timer": "perf_counter",
                "min_rounds": 5,
                "max_time": 1.0,
                "min_time": 5e-06,
                "warmup": false
            },
            "stats": {
                "min": 4.4397893361747265e-07,
                "max": 7.951000588946044e-05,
                "mean": 6.153237291452876e-07,
                "stddev": 6.098304112714216e-07,
                "rounds": 60263,
                "median": 5.629844963550568e-07,
                "iqr": 1.259904820472002e-07,
                "q1": 5.199981387704611e-07,
                "q3": 6.459886208176613e-07,
                "iqr_outliers": 2295,
                "stddev_outliers": 219,
                "outliers": "219;2295",
                "ld15iqr": 4.4397893361747265e-07,
                "hd15iqr": 8.3498889580369e-07,
                "ops": 1625160.7936346694,
                "total": 0.03708125388948247,
                "iterations": 1
            }
        },
    """
    name: str
    scale: int
    min: float
    max: float
    mean: float
    stddev: float


RE_NAME_AND_SCALE = re.compile(r'(\S+)\[(.+)]')


def split_name_and_scale(name: str) -> typing.Tuple[str, int]:
    m = RE_NAME_AND_SCALE.match(name)
    if m:
        return m.group(1), int(m.group(2))
    raise ValueError(f'Can not parse name "{name}"')


def benchmark_from_json(j: typing.Dict[str, typing.Any]) -> Benchmark:
    name, scale = split_name_and_scale(j['name'])
    stats = j['stats']
    return Benchmark(
        name,
        scale,
        stats['min'],
        stats['max'],
        stats['mean'],
        stats['stddev'],
    )


@dataclasses.dataclass
class Benchmarks:
    """All the benchmarks from a JSON file."""
    environment: BenchmarkEnv
    benchmarks: typing.List[Benchmark]


def benchmarks_from_json(j: typing.Dict[str, typing.Any]) -> Benchmarks:
    ret = Benchmarks(environment_from_json(j), [])
    for benchmark_dict in j['benchmarks']:
        ret.benchmarks.append(benchmark_from_json(benchmark_dict))
    return ret


def parse_json_file(path: str) -> Benchmarks:
    with open(path) as file:
        j = json.load(file)
        return benchmarks_from_json(j)


DAT_FILE_EXTENSION = '.dat'

def remove_existing_dat_files(benchmarks: Benchmarks) -> None:
    for f in glob.glob(
            os.path.join(
                OUTPUT_DAT_DIR,
                f'{benchmarks.environment.python_implementation}'
                f'_{benchmarks.environment.python_version}'
                f'*{DAT_FILE_EXTENSION}'
            )
    ):
        logger.info(f'Removing {f}')
        os.remove(f)


def write_benchmarks_to_dat_files(benchmarks: Benchmarks) -> None:
    # Create and index into the benchmarks {name : { scale : index, ...}, ...}
    index: typing.Dict[str, typing.Dict[int, int]] = {}
    for i, benchmark in enumerate(benchmarks.benchmarks):
        if benchmark.name not in index:
            index[benchmark.name] = {}
        if benchmark.scale not in index[benchmark.name]:
            index[benchmark.name][benchmark.scale] = i
    # Write each file
    for benchmark_name in index.keys():
        out_file_name = '_'.join(
            [
                f'{benchmarks.environment.python_implementation}',
                f'{benchmarks.environment.python_version}',
                benchmark_name,
            ]
        ) + DAT_FILE_EXTENSION
        out_path = os.path.normpath(os.path.join(OUTPUT_DAT_DIR, out_file_name))
        logger.info(f'Writing DAT file: {out_path}')
        with open(out_path, 'w') as dat_file:
            # $1        $2           $3           $4           $5          $6
            # Scale     Min          Mean         StdDev       Max         Name
            # 512       0.000000451  0.000000599  0.000000691  0.000066975 test_float_iar_begin
            dat_file.write(
                '# $1      $2           $3           $4           $5          $6\n'
            )
            dat_file.write(
                '# Scale   Min          Mean         StdDev       Max         Name\n'
            )
            for scale in sorted(index[benchmark_name]):
                line = [f'{scale:<8d}']
                b = benchmarks.benchmarks[index[benchmark_name][scale]]
                line.append(f'{b.min:12.9f}')
                line.append(f'{b.mean:12.9f}')
                line.append(f'{b.stddev:12.9f}')
                line.append(f'{b.max:12.9f}')
                line.append(f'{benchmark_name:s}')

                dat_file.write(' '.join(line))
                dat_file.write('\n')


def walk_benchmark_directory():
    for root, dirs, files in os.walk(BENCHMARK_DIR):
        files.sort()
        for name in files:
            path = os.path.normpath(os.path.join(root, name))
            logger.info(f'Reading {path}')
            try:
                benchmarks = parse_json_file(path)
                remove_existing_dat_files(benchmarks)
                write_benchmarks_to_dat_files(benchmarks)
            except json.JSONDecodeError as err:
                logger.error(str(err))


def main():
    LOG_FORMAT_NO_PROCESS = "%(asctime)s - %(filename)24s#%(lineno)-4d - %(levelname)-8s - %(message)s"
    logging.basicConfig(
        level=logging.INFO, format=LOG_FORMAT_NO_PROCESS, stream=sys.stdout
    )
    walk_benchmark_directory()
    return 0


if __name__ == '__main__':
    exit(main())
