//
// Created by Paul Ross on 07/04/2025.
//

#ifndef SKIPLIST_TESTFRAMEWORK_H
#define SKIPLIST_TESTFRAMEWORK_H

#include <list>
#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <chrono>

class ExecClock {
public:
    typedef std::chrono::duration<double> tHiResDouble;
    ExecClock() : _start(std::chrono::high_resolution_clock::now()) {}
    /* Returns seconds to a resolution of one microsecond. */
    double seconds() {
        auto end = std::chrono::high_resolution_clock::now();
        auto count = std::chrono::duration_cast<std::chrono::nanoseconds>(end - _start).count();
        return (static_cast<double>(count) + 0.5) / 1e9;
    }
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> _start;
};

/**
 * TestResult is a class that retains correctness and performance information
 * of a particular test.
 *
 * Terminology:
 * scale - The declared scale of the test for time complexity analysis.
 * repeat - The number of times an identical test is conducted to get a
 *          statistical feel for the execution time.
 * count - The number of atomic operations that the execution time has measured.
 *
 * NOTE: execTime() gives the mean time of all of the executions. This needs to be divided by \c count
 * to get the execution time of a single operation.
 *
 * For example timing sorting a vector.
 * scale: might have a range 32, 64, 128, 256, 512, 1024 (vector length).
 * repeat might be 10 to get some min/max/mean/std. dev. values for variety.
 * count: might be 100 to get a reasonable (single) execution time for each test.
 *
 * Typical usage:
 * test_something() does a single test, possibly some fast operation count
 * number of times to get a decent time.
 *
 * test_multiple() does the same but repeats it 10 times to get some statistics
 * on it.
 *
 * test_complexity() does the same as test_multiple() but over a range of values
 * of N so that some sense of the time complexity can be gained.
 *
 * \code
 void test_something(TestResultS &results) {
    ExecClock clk;
    // Do something count number of times here, set failure to 0 | 1.
    results.push_back(TestResult(__FUNCTION__, failure, clk.execTime(), count));
 }

 void test_multiple(TestResultS &results) {
    TestResult test_result(__FUNCTION__);
    for (size_t i = 0; i < 10; ++i) {
        ExecClock clk;
        for (size_t j = 0; j < COUNT; ++j) {
            // Do something small here COUNT number of times
        }
        test_result.execTimeAdd(0, clk.execTime(), COUNT);
    }
    results.push_back(test_result);
 }

 void test_complexity(TestResultS &results) {
    int failure = 0;
    TestResult test_result(__FUNCTION__);
    std::list<size_t> scales = {1024, 2048, 4096, 8192};
    for (auto scale: scales) {
        // Create something appropriate to scale
        vector<char> input;
        for (tTimeVectWord i = 0; i < scale; ++i) {
            input.push_back(i);
        }
        for (size_t repeat = 0; repeat < 10; ++repeat) {
            ExecClock clk;
            // Do something small here COUNT number of times, set failure.
            test_result.execTimeAdd(failure, clk.execTime(), COUNT, scale);
        }
     }
     results.push_back(test_result);
 }

 int main() {
    TestResultS results;

    test_something(results);
    test_multiple(results);
    test_complexity(results);

    std::cout << results;
 }
 * \endcode
 *
 * Note on gnuplot
 * ---------------------
 *
 * See also: <tt>std::ostream &operator<<(std::ostream &os, const TestResult &result)</tt>
 *
 * This example is if the following tests are reported:
 *
 * \code
 * // Add 4 (mock_repeat) tests results
 * mock_test_result.execTimeAdd(0, 1.0, 100, 99);
 * mock_test_result.execTimeAdd(0, 2.0, 100, 99);
 * mock_test_result.execTimeAdd(0, 3.0, 100, 99);
 * mock_test_result.execTimeAdd(0, 4.0, 100, 99);
 * \endcode
 *
 * If the test output is put into individual .dat files such as:
 *
 * \code
 * #HEAD: Fail   Scale  Repeat         Mean(s)     Std.Dev.(s)         Min.(s)         Max.(s)     Count      Rate(/s) Name
 * #  $1   $2      $3      $4              $5              $6              $7              $8        $9           $10 $11
 * TEST:    0      99       4     2.500000000     1.118033989     1.000000000     4.000000000       400         160.0 test_internal_test_result_string
 * \endcode
 *
 * So:
 *
 *  - The total time to run all of these tests is: <tt>($5 * $4)</tt>, in this case 10.0 (s).
 *  - The mean time for each atomic test is: <tt>($5 * $4 / $9)</tt>, in this case 10.0 / 400 = 0.025 (s)
 *  - The standard deviation atomic time is: <tt>($6 / ($9 / $4))</tt> or <tt>($6 * $4 / $9)</tt>, in this case 4 * 1.11 / 400 = +/- 0.0111 (s)
 *  - The minimum atomic time is: <tt>($7 / ($9 / $4))</tt> or <tt>($7 * $4 / $9)</tt>, in this case 4 * 1.0 / 400 = 0.010 (s)
 *  - The maximum atomic time is: <tt>($8 / ($9 / $4))</tt> or <tt>($8 * $4 / $9)</tt>, in this case 4 * 4.0 / 400 = 0.040 (s)
 *
 * Candlestick plots, from gnuplot: "Both require five columns of data: the x value, followed (in order) by
 * the opening, low, high, and closing prices."
 *
 * So a suitable gnuplot statement for a candle stick plot with time in µs is:
 *
 * \code
 * opening  : Mean - Std.Dev    (($5 - $6) * $4 / $9)
 * low      : Min               ($7 * $4 / $9)
 * high     : Max               ($8 * $4 / $9)
 * closing  : Mean + Std.Dev    (($5 + $6) * $4 / $9)
 * \endcode
 *
 * And for a suitable gnuplot statement for a candle stick plot with rate in µs / object is:
 *
 * \code
 * opening  : Mean - Std.Dev    (($5 - $6) * $4 / ($9 * $3))
 * low      : Min               ($7 * $4 / ($9 * $3))
 * high     : Max               ($8 * $4 / ($9 * $3))
 * closing  : Mean + Std.Dev    (($5 + $6) * $4 / ($9 * $3))
 * \endcode
 *
 * So for a time plot:
 *
 * \code
 * plot "dat/test_cpp_vector_char_to_py_bytes.dat" \
 *      using 3:(1e6 * ($5 - $6) * $4 / $9):(1e6 * $7 * $4 / $9):(1e6 * $8 * $4 / $9):(1e6 * ($5 + $6) * $4 / $9) \
 *      t "Python str 8 bit -> C++" with candlesticks whiskerbars 0.5 linewidth 2,\
 *      "dat/test_cpp_vector_char_to_py_bytes.dat" using 3:(1e6 * $7 * $4 / $9) \
 *       t "Minimum Python str 8 bit -> C++" with lines linewidth 2, \
 * \endcode
 *
 * And for a rate plot:
 *
 * \code
 * plot "dat/test_cpp_vector_char_to_py_bytes.dat" \
 *      using 3:(1e6 * ($5 - $6) * $4 / ($9 * $3)):(1e6 * $7 * $4 / ($9 * $3)):(1e6 * $8 * $4 / ($9 * $3)):(1e6 * ($5 + $6) * $4 / ($9 * $3)) \
 *      t "Python str 8 bit -> C++" with candlesticks whiskerbars 0.5 linewidth 2,\
 *      "dat/test_cpp_vector_char_to_py_bytes.dat" using 3:(1e6 * $7 * $4 / ($9 * $3)) \
 *       t "Minimum Python str 8 bit -> C++" with lines linewidth 2, \
 * \endcode
 *
 * NOTE: Time plot was, in error:
 *
 * \code
 * plot "dat/test_cpp_vector_char_to_py_bytes.dat" \
 *      using 3:(1e6 * ($5 - $6) / $4):(1e6 * $7 / $4):(1e6 * $8 / $4):(1e6 * ($5 + $6) / $4) \
 *      t "Python str 8 bit -> C++" with candlesticks whiskerbars 0.5 linewidth 2,\
 *      "dat/test_cpp_vector_char_to_py_bytes.dat" using 3:($7 * 1e6 / $4) \
 *       t "Minimum Python str 8 bit -> C++" with lines linewidth 2, \
 * \endcode
 *
 * Note on Using the Rate Column
 * -------------------------------
 * If the mean value is very small it will loose resolution.
 * For example here is a test that takes 1.23456789 ns that only has 1 S.F. resolution.
 * Inverting the rate can recover the time to a much higher resolution.
 *
 * \code
 * HEAD: Fail   Scale  Repeat         Mean(s)     Std.Dev.(s)         Min.(s)         Max.(s)     Count      Rate(/s) Name
 * TEST:    0      99       1     0.000000001             N/A             N/A             N/A         1   810000007.4 test_internal_test_result_string_using_rate
 * \endcode
 *
 */
class TestResult {
public:
    // Constructor for a single test where all the information is known at
    // construction time.
    TestResult(std::string name,
               int failed,
               double execTime,
               size_t testCount=1,
               size_t scale=1);
    // Constructor for a test that will accumulate performance data.
    // Use execTimeAdd() to add a result.
    TestResult(std::string name) : _name(name) {}

    /// Setters:
    void setFailed(size_t scale=1) { _results[scale].failed = 1; }
    // Add a new result
    void execTimeAdd(int failed, double execTime, size_t test_count, size_t scale=1);
    /// Getters
    std::string name() const { return _name; }
    // Getting failure flag
    int failed() const; // All failures OR'd together
    int failed(size_t scale) const;
    // The total number of tests.
    size_t numTests() const;
    // The number of tests for a particular scale
    size_t numTests(size_t scale) const;
    size_t testCount(size_t scale) const { return _results.at(scale).count; }
    // Execution time
    double totalTime(size_t scale) const;
    // execTime() returns the mean of the results or 0.0 if no results
    double execTime(size_t scale) const;
    double atomicTestMeanExecTime(size_t scale) const;
    // Standard deviation, 0.0 if < 2 results.
    double execTimeStdDev(size_t scale) const;
    bool hasExecTimeStdDev(size_t scale) const { return numTests(scale) > 1; }
    double execTimeMin(size_t scale) const;
    double execTimeMax(size_t scale) const;
    size_t numScaleValues() const;
    std::vector<size_t> scaleValues() const;
public:
    TestResult(const TestResult &rhs) = default;
    TestResult &operator=(const TestResult &rhs) = default;
    TestResult(TestResult &&) = default;
private:
    std::string _name;  /* Arbitrary name. */
    struct test_result {
        size_t count; // Cumulative count of atomic tests for this scale.
        int failed;                     // Failure OR'd together
        std::list<double> _execTimeS;   // Execution times
    };
    // Key: scale, Value: test_result
    std::map<size_t, struct test_result> _results;
};

/** This just accumulates a list of TestResult objects and can print them out
 * in a human readable form.
 */
class TestResultS {
public:
    typedef std::list<TestResult> tResults;
    TestResultS() : _failed(0) {}
    void push_back(const TestResult &result);
    const tResults &results() const { return _results; }
    void dump_header(std::ostream &os) const;
    void dump_tests(std::ostream &os) const;
    void dump_tail(std::ostream &os) const;
    int failed() const { return _failed; }
private:
    /* Result, 0 if tests have passed, non-zero if any failed in some fashion. */
    int _failed;
    tResults _results;
};

std::ostream &operator<<(std::ostream &os, const TestResult &result);
std::ostream &operator<<(std::ostream &os, const TestResultS &results);

/** In a single test function there might be many tests, this class
 * keeps track of which sub-test is being evaluated and uniquely
 * sets the failure flag by bit twiddling.
 */
class SubTestCount {
public:
    SubTestCount() : m_failure(0), m_test_count(0) {}
    int failure() const { return m_failure; }
    size_t test_count() const { return m_test_count; }
    // If the condition is true the test passes.
    void test(bool condition);
    // Returns a vector of test failure indexes.
    std::vector<size_t> test_failures() const;
protected:
    int m_failure;
    size_t m_test_count;
};

std::string unique_string(int width=0);
std::u16string unique_u16string(int width=0);
std::u32string unique_u32string(int width=0);
std::vector<char> unique_vector_char(int width);
size_t count_of_unique_string();
void reset_count_of_unique_string();

#endif //SKIPLIST_TESTFRAMEWORK_H
