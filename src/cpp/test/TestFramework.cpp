//
// Created by Paul Ross on 07/04/2025.
//

#include "TestFramework.h"
#include <iomanip>
#include <cmath>
#include <cassert>
#include <sstream>

#include "save_stream_state.h"

TestResult::TestResult(std::string name,
                       int failed,
                       double execTime,
                       size_t testCount,
                       size_t scale) : _name(name)
{
    _results[scale] = {testCount, failed, {execTime}};
}

int TestResult::failed(size_t scale) const {
//    return _failed.at(scale);
    return _results.at(scale).failed;
}

int TestResult::failed() const {
    int f = 0;
    for (auto iter: _results) {
        f |= iter.second.failed;
    }
    return f;
}

void
TestResult::execTimeAdd(int failed,
                        double execTime,
                        size_t test_count,
                        size_t scale) {
    // TODO: Raise if _testCount[scale] exists and != test_count
    if (_results.count(scale)) {
        _results.at(scale).count += test_count;
        _results.at(scale).failed |= failed;
        _results.at(scale)._execTimeS.push_back(execTime);
    } else {
        _results[scale] = {test_count, failed, {execTime}};
    }
}

// The total number of tests.
size_t TestResult::numTests() const {
    size_t r = 0;
    for (auto &scale: scaleValues()) {
        r += numTests(scale);
    }
    return r;
}

// The number of tests for a particular scale
size_t TestResult::numTests(size_t scale) const {
    return _results.at(scale)._execTimeS.size();
}

double TestResult::totalTime(size_t scale) const {
    double r = 0.0;
    if (_results.count(scale)) {
        for (auto &iter: _results.at(scale)._execTimeS) {
            r += iter;
        }
    }
    return r;
}

/**
 * Returns the mean of the execution times.
 * This does not take into account the atomic test counts.
 */
double TestResult::execTime(size_t scale) const {
    double r = 0.0;
    if (_results.count(scale)) {
        for (auto &iter: _results.at(scale)._execTimeS) {
            r += iter;
        }
        r /= _results.at(scale)._execTimeS.size();
    }
    return r;
}

/**
 * Returns the sum of the execution times divided by the number of atomic tests.
 */
double TestResult::atomicTestMeanExecTime(size_t scale) const {
    double r = 0.0;
    if (_results.count(scale)) {
        r = totalTime(scale);
        r /= _results.at(scale).count;
    }
    return r;
}

double TestResult::execTimeStdDev(size_t scale) const {
    double mean = execTime(scale);
    double std_dev = 0.0;
    size_t count = 0;
    if (hasExecTimeStdDev(scale)) {
        for (auto &value: _results.at(scale)._execTimeS) {
            assert(value >= 0.0);
            std_dev += pow(mean - value, 2.0);
            ++count;
        }
        std_dev = sqrt(std_dev / count);
    }
    return std_dev;
}

double TestResult::execTimeMin(size_t scale) const {
    double r = 0.0;
    if (_results.count(scale)) {
        for (auto &val: _results.at(scale)._execTimeS) {
            assert(val >= 0.0);
            if (r == 0.0) {
                r = val;
            } else {
                r = std::min(r, val);
            }
        }
    }
    return r;
}

double TestResult::execTimeMax(size_t scale) const {
    double r = 0.0;
    if (_results.count(scale)) {
        for (auto &val: _results.at(scale)._execTimeS) {
            assert(val >= 0.0);
            r = std::max(r, val);
        }
    }
    return r;
}

size_t TestResult::numScaleValues() const {
    return _results.size();
}

std::vector<size_t> TestResult::scaleValues() const {
    std::vector<size_t> r;
    for (auto iter: _results) {
        r.push_back(iter.first);
    }
    return r;
}

static const int TIME_PRECISION = 9;
static const int TIME_WIDTH = 16;

/**
 * Note on The Output
 * ---------------------
 * Example output (with header and $ gnuplot guide).
 *
 * \code
 * #HEAD: Fail   Scale  Repeat         Mean(s)     Std.Dev.(s)         Min.(s)         Max.(s)     Count      Rate(/s) Name
 * #  $1   $2      $3      $4              $5              $6              $7              $8        $9           $10 $11
 * TEST:    0      99       4     2.500000000     1.118033989     1.000000000     4.000000000       400         160.0 test_internal_test_result_string
 * \endcode
 *
 * In this case we have repeated the tests four times, each of these tests has a timed operation (the 'atomic' test)
 * that is repeated 100 times for each individual execution time.
 *
 * The total time to run all of these tests is: Repeat * Mean in seconds, in this case 10.0 (s).
 * The mean time for each atomic test is: Repeat * Mean / Count, in this case 10.0 / 400 = 0.025 (s)
 * The minimum atomic time is: Repeat * Min.(s) / Count, in this case 4 * 1.0 / 400 = 0.010 (s)
 * The maximum atomic time is: Repeat * Max.(s) / Count, in this case 4 * 4.0 / 400 = 0.040 (s)
 * The standard deviation atomic time is: Repeat * Std.Dev.(s) / Count, in this case 4 * 1.11 / 400 = +/- 0.0111 (s)
 *
 * @param os The stream to write to.
 * @param result The tests result.
 * @return The stream for chaining.
 */
std::ostream &operator<<(std::ostream &os, const TestResult &result) {
    StreamFormatState stream_state(os); // Preserve state
    std::string prefix = "";
    for (auto scale: result.scaleValues()) {
        os << prefix << "TEST: ";
        os << std::setw(4) << result.failed(scale);
        // Scale of the test
        os << std::setw(8) << scale;
        // Repeat
        os << std::setw(8) << result.numTests(scale);
        os << std::setw(TIME_WIDTH) << std::fixed << std::setprecision(TIME_PRECISION);
        // Execution time from execTime, normally the mean.
        os << result.execTime(scale);
        // Standard deviation
        if (result.hasExecTimeStdDev(scale)) {
            os << std::setw(TIME_WIDTH) << std::fixed << std::setprecision(TIME_PRECISION);
            os << result.execTimeStdDev(scale);
        } else {
            os << std::setw(TIME_WIDTH) << "N/A";
        }
        // Min and Max
        if (result.hasExecTimeStdDev(scale)) {
            os << std::setw(TIME_WIDTH) << std::fixed << std::setprecision(TIME_PRECISION);
            os << result.execTimeMin(scale);
            os << std::setw(TIME_WIDTH) << std::fixed << std::setprecision(TIME_PRECISION);
            os << result.execTimeMax(scale);
        } else {
            os << std::setw(TIME_WIDTH) << "N/A";
            os << std::setw(TIME_WIDTH) << "N/A";
        }
        // Count
        os << std::setw(10) << result.testCount(scale);
        // Rate, this uses execTime which is usually the mean execution time.
        if (result.testCount(scale) > 0) {
            os << std::setw(14) << std::fixed << std::setprecision(1);
            os << result.testCount(scale) / result.execTime(scale);
        } else {
            os << std::setw(14) << "N/A";
        }
        os << " " << result.name();
        prefix = "\n";
    }
    return os;
}

void TestResultS::push_back(const TestResult &result) {
    _results.push_back(std::move(result));
    _failed |= result.failed();
}

#define REGEX_SPACE_INTEGER "\\s+(\\d+)"
#define REGEX_SPACE_FLOAT "\\s+([0-9+-.]+)"
#define REGEX_SPACE_STRING_NO_SPACE "\\s+(\\S+)"
#define REGEX_SPACE_ANYTHING "\\s+(.+)"

void TestResultS::dump_header(std::ostream &os) const {
    StreamFormatState stream_state(os); // Preserve state
    os << "Number of tests: " << _results.size() << std::endl;
    os << "REGEX_HEAD: ";
    os << "\"HEAD:";
    for (int i = 0; i < 10; ++i) {
        os << REGEX_SPACE_STRING_NO_SPACE;
    }
    os << "\"" << std::endl;
    os << "REGEX_TEST: ";
    os << "\"TEST:";
    for (int i = 0; i < 3; ++i) {
        os << REGEX_SPACE_INTEGER;
    }
    for (int i = 0; i < 4; ++i) {
        os << REGEX_SPACE_FLOAT;
    }
    os << REGEX_SPACE_INTEGER;
    os << REGEX_SPACE_FLOAT;
    os << REGEX_SPACE_STRING_NO_SPACE;
    os << "\"" << std::endl;
    os << "REGEX_TAIL: ";
    os << "\"TAIL:" << REGEX_SPACE_ANYTHING;
    os << "\"" << std::endl;
    os << "#  $1   $2      $3      $4              $5              $6              $7              $8        $9           $10 $11";
    os << std::endl;
    os << "HEAD: ";
    os << std::setw(4) << "Fail";
    os << std::setw(8) << "Scale";
    os << std::setw(8) << "Repeat";
    os << std::setw(TIME_WIDTH) << "Mean(s)";
    os << std::setw(TIME_WIDTH) << "Std.Dev.(s)";
    os << std::setw(TIME_WIDTH) << "Min.(s)";
    os << std::setw(TIME_WIDTH) << "Max.(s)";
    os << std::setw(10) << "Count";
    os << std::setw(14) << "Rate(/s)";
    os << " Name";
    os << std::endl;
}

void TestResultS::dump_tests(std::ostream &os) const {
    for (const auto &result : _results) {
        os << result << std::endl;
    }
}

void TestResultS::dump_tail(std::ostream &os) const {
    StreamFormatState stream_state(os); // Preserve state

    unsigned int count_fail = 0;
    unsigned int count_pass = 0;

    for (const auto &result : _results) {
        if (result.failed()) {
            count_fail += result.numTests();
        } else {
            count_pass += result.numTests();
        }
    }
    os << "TAIL:";
    os << " Passed=" << count_pass << "/" << count_pass + count_fail;
    os << " Failed=" << count_fail << "/" << count_pass + count_fail;
    os << std::endl;
    if (count_fail > 0) {
        os << "Failed tests [" << count_fail << "]:" << std::endl;
        for (const auto &result : _results) {
            if (result.failed()) {
                os << result << std::endl;
            }
        }
    } else {
        os << "All tests pass." << std::endl;
    }
}

std::ostream &operator<<(std::ostream &os, const TestResultS &results) {
    results.dump_header(os);
    results.dump_tests(os);
    results.dump_tail(os);
    return os;
}

void SubTestCount::test(bool condition) {
    if (m_test_count >= 8 * sizeof(int)) {
        throw std::out_of_range("Can not set flag on SubTestCount");
    }
    if (! condition) {
        m_failure |= 1 << m_test_count;
    }
    ++m_test_count;
}

std::vector<size_t> SubTestCount::test_failures() const {
    std::vector<size_t> r;
    int mask = 1;
    for (size_t i = 0; i < m_test_count; ++i) {
        if (m_failure & mask) {
            r.push_back(i);
        }
        mask <<= 1;
    }
    return r;
}

static size_t str_count = 0;

/**
 * Creates a unique \c std::string. This starts as "0", "1" ...
 *
 * @param width If > 0 the string will be at least this width.
 * @return The unique \c std::string.
 */
std::string unique_string(int width) {
    std::ostringstream os;
    if (width > 0) {
        os << std::setfill(' ') << std::setw(width) << str_count;
    } else {
        os << str_count;
    }
    ++str_count;
    return os.str();
}

/**
 * Creates a unique \c std::u16string. This starts as "0", "1" ...
 *
 * @param width If > 0 the string will be at least this width.
 * @return The unique \c std::u16string.
 */
std::u16string unique_u16string(int width) {
    std::string temp = unique_string(width);
    std::u16string ret;
    for (auto c: temp) {
        ret.push_back(c);
    }
    return ret;
}

/**
 * Creates a unique \c std::u32string. This starts as "0", "1" ...
 *
 * @param width If > 0 the string will be at least this width.
 * @return The unique \c std::u32string.
 */
std::u32string unique_u32string(int width) {
    std::string temp = unique_string(width);
    std::u32string ret;
    for (auto c: temp) {
        ret.push_back(c);
    }
    return ret;
}

/**
 * Creates a unique \c std::vector<char>.
 *
 * @param width If > 0 the string will be at least this width.
 * @return The unique \c std::vector<char>.
 */
std::vector<char> unique_vector_char(int width) {
    std::string tmp = unique_string(width);
    std::vector<char> ret(tmp.begin(), tmp.end());
    return ret;
}

/**
 * Returns the number of unique strings created.
 * @return Number of unique strings created.
 */
size_t count_of_unique_string() {
    return str_count;
}

/**
 * Reset the string counter.
 */
void reset_count_of_unique_string() {
    str_count = 0;
}
