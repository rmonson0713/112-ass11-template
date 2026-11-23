#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "unity.h"
#include "code.h"

/*
 * Every test below is a skeleton that already compiles and FAILS on purpose.
 * Replace the TEST_FAIL_MESSAGE line with your assertions.
 *
 * HOW A UNITY TEST LOOKS
 *   void test_something(void)
 *   {
 *       TEST_ASSERT_EQUAL_INT_MESSAGE(5, (int)strlen("hello"), "strlen(\"hello\") should be 5");
 *   }
 *   The FIRST argument is the value you EXPECT, the SECOND is the value the
 *   code under test ACTUALLY produces, the LAST is the text shown on failure.
 *   The first assertion that fails stops the test, so put the simple cases first.
 *
 * RULES
 *  - Only the standard library and Unity. No setUp()/tearDown().
 *  - Every value you assert must be written as a literal in your test - do NOT
 *    compute the expected value with the same function you are testing.
 *  - Compile with:
 *      gcc -std=c99 -g -Wall -Wextra -fsanitize=address,undefined main.c code.c unity.c -lm -o main
 *    (adjust the file names to your repository layout).
 */

/* ======================================================================
 * HELPERS (not tests)
 * ====================================================================== */

/* Given: the pattern for the other comparators. */
int cmp_int(const void *a, const void *b)
{
    int x = *(const int *)a;
    int y = *(const int *)b;
    return (x > y) - (x < y);
}

/*
 * HELPER cmp_char
 *
 * Compare two chars ascending. Cast the pointers to const unsigned char *
 * and return (x > y) - (x < y).
 */

int cmp_char(const void *a, const void *b)
{
    const unsigned char *x = (const unsigned char *)a;
    const unsigned char *y = (const unsigned char *)b;

    return (*x > *y) - (*x < *y);
}

/*
 * HELPER cmp_strlen
 *
 * Each element is a char *, so a points to a char *:
 *     const char *x = *(const char * const *)a;
 * Return the comparison of strlen(x) and strlen(y), shorter first.
 */

int cmp_strlen(const void *a, const void *b)
{
    const char *x = *(const char * const *)a;
    const char *y = *(const char * const *)b;

    return (strlen(x) > strlen(y)) - (strlen(x) < strlen(y));
}

/*
 * HELPER cmp_str
 *
 * Each element is a char *. Get both strings out of the elements (see
 * cmp_strlen) and return strcmp(x, y).
 */

int cmp_str(const void *a, const void *b)
{
    const char *x = *(const char * const *)a;
    const char *y = *(const char * const *)b;

    return strcmp(x, y);
}

/*
 * HELPER is_anagram
 *
 * Return 1 if a and b are anagrams, 0 otherwise.
 *  - If strlen(a) != strlen(b), or the length is 63 or more, return 0.
 *  - Copy both strings into char first[64] and char second[64] (strcpy).
 *  - Sort the characters of each copy: qsort(first, strlen(first), sizeof(char), cmp_char);
 *  - Return 1 if strcmp(first, second) == 0.
 */

int is_anagram(const char *a, const char *b)
{
    if (strlen(a) != strlen(b) || strlen(a) >= 63)
    {
        return 0;
    }

    char first[64];
    char second[64];

    strcpy(first, a);
    strcpy(second, b);

    qsort(first, strlen(first), sizeof(char), cmp_char);
    qsort(second, strlen(second), sizeof(char), cmp_char);

    return strcmp(first, second) == 0;
}

/* ======================================================================
 * PART 1: Text and numbers (every case is listed - just turn them into assertions)
 * ====================================================================== */

/*
 * STEP 1: test_strlen_basic
 *
 * Verify strlen() on ordinary strings. Check that strlen("hello") is 5,
 * strlen("a") is 1, strlen("Hello, World!") is 13 and strlen("  x ") is 4
 * (spaces count). strlen() returns size_t, so cast the result to int before
 * you compare it.
 *
 * HINT: One assertion per string:  TEST_ASSERT_EQUAL_INT_MESSAGE(5, (int)strlen("hello"), "strlen(\"hello\") should be 5");
 *       The first argument is the EXPECTED value, the second is the ACTUAL value.
 */

void test_strlen_basic(void)
{
    TEST_ASSERT_EQUAL_INT_MESSAGE(5, (int)strlen("hello"), "strlen(\"hello\") should be 5");

    TEST_ASSERT_EQUAL_INT_MESSAGE(1, (int)strlen("a"), "strlen(\"a\") should be 1");

    TEST_ASSERT_EQUAL_INT_MESSAGE(13, (int)strlen("Hello, World!"), "strlen(\"Hello, World!\") should be 13");

    TEST_ASSERT_EQUAL_INT_MESSAGE(4, (int)strlen("  x "), "strlen(\"  x \") should be 4");
}

/*
 * STEP 2: test_strlen_edge_cases
 *
 * Verify the edge cases of strlen(). The empty string has length 0. For char
 * buf[10] = "abc"; strlen(buf) is 3 although sizeof(buf) is 10 (assert both
 * numbers). A string with a terminator inside, such as "ab\0cd", has length 2
 * because strlen() stops at the first '\0'. A string ending in a newline,
 * "line\n", has length 5.
 *
 * HINT: Declare the array and the embedded string as variables first:  const char embedded[] = "ab\0cd";
 */

void test_strlen_edge_cases(void)
{
    char buf[10] = "abc";
    const char embedded[] = "ab\0cd";

    TEST_ASSERT_EQUAL_INT_MESSAGE(0, (int)strlen(""), "Empty string should have length 0");

    TEST_ASSERT_EQUAL_INT_MESSAGE(3, (int)strlen(buf), "strlen(buf) should be 3");

    TEST_ASSERT_EQUAL_INT_MESSAGE(10, (int)sizeof(buf), "sizeof(buf) should be 10");

    TEST_ASSERT_EQUAL_INT_MESSAGE(2, (int)strlen(embedded), "strlen should stop at the first null terminator");

    TEST_ASSERT_EQUAL_INT_MESSAGE(5, (int)strlen("line\n"), "String ending in newline should have length 5");
}

/*
 * STEP 3: test_strcmp_signs
 *
 * strcmp() returns a negative number, zero or a positive number. Only the SIGN
 * is specified, so never compare the result with -1 or 1. Assert that
 * strcmp("same", "same") is exactly 0. Assert every other result by checking a
 * comparison: for example TEST_ASSERT_EQUAL_INT_MESSAGE(1, strcmp("apple",
 * "banana") < 0, ...). Check: "apple" < "banana"; "banana" > "apple"; a prefix
 * is smaller ("app" < "apple" and "apple" > "app"); uppercase comes before
 * lowercase ("Zebra" < "apple" and "a" > "A"); the empty string is smaller
 * than any other string ("" < "a").
 *
 * HINT: A comparison in C gives 1 when it is true and 0 when it is false, so  (strcmp(a, b) < 0)  can be compared with the number 1.
 */

void test_strcmp_signs(void)
{
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, strcmp("same", "same"), "Identical strings should return 0");

    TEST_ASSERT_EQUAL_INT_MESSAGE(1, strcmp("apple", "banana") < 0, "apple should come before banana");

    TEST_ASSERT_EQUAL_INT_MESSAGE(1, strcmp("banana", "apple") > 0, "banana should come after apple");

    TEST_ASSERT_EQUAL_INT_MESSAGE(1, strcmp("app", "apple") < 0, "A prefix should come before the longer string");

    TEST_ASSERT_EQUAL_INT_MESSAGE(1, strcmp("apple", "app") > 0, "The longer string should come after its prefix");

    TEST_ASSERT_EQUAL_INT_MESSAGE(1, strcmp("Zebra", "apple") < 0, "Uppercase letters should come before lowercase letters");

    TEST_ASSERT_EQUAL_INT_MESSAGE(1, strcmp("a", "A") > 0, "Lowercase a should come after uppercase A");

    TEST_ASSERT_EQUAL_INT_MESSAGE(1, strcmp("", "a") < 0, "The empty string should come before any nonempty string");
}

/*
 * STEP 4: test_atoi_valid
 *
 * Verify atoi() on valid input. Check: "123" gives 123, "0" gives 0, "-45"
 * gives -45, "+7" gives 7, "  42" (leading spaces are skipped) gives 42, "007"
 * gives 7 and "2147483647" gives 2147483647.
 *
 * HINT: atoi() returns int, so TEST_ASSERT_EQUAL_INT_MESSAGE can compare it directly.
 */

void test_atoi_valid(void)
{
    TEST_ASSERT_EQUAL_INT_MESSAGE(123, atoi("123"), "123 should return 123");

    TEST_ASSERT_EQUAL_INT_MESSAGE(0, atoi("0"), "0 should return 0");

    TEST_ASSERT_EQUAL_INT_MESSAGE(-45, atoi("-45"), "-45 should return -45");

    TEST_ASSERT_EQUAL_INT_MESSAGE(7, atoi("+7"), "+7 should return 7");

    TEST_ASSERT_EQUAL_INT_MESSAGE(42, atoi("  42"), "Leading spaces should be skipped");

    TEST_ASSERT_EQUAL_INT_MESSAGE(7, atoi("007"), "007 should return 7");

    TEST_ASSERT_EQUAL_INT_MESSAGE(2147483647, atoi("2147483647"), "Maximum 32-bit signed integer should return 2147483647");
}

/*
 * STEP 5: test_atoi_partial_and_invalid
 *
 * Verify what atoi() does with text that is not a clean number. Check: "12abc"
 * gives 12 (it reads the digits and stops), "abc" gives 0, "" gives 0, "-"
 * gives 0, "3.9" gives 3 (the fraction is ignored) and "1 2" gives 1 (it stops
 * at the space).
 *
 * HINT: atoi() has no way to report an error - it simply returns 0. That is why serious programs use strtol().
 */

void test_atoi_partial_and_invalid(void)
{
    TEST_ASSERT_EQUAL_INT_MESSAGE(12, atoi("12abc"), "Should stop reading at the first non-digit");

    TEST_ASSERT_EQUAL_INT_MESSAGE(0, atoi("abc"), "Non-numeric string should return 0");

    TEST_ASSERT_EQUAL_INT_MESSAGE(0, atoi(""), "Empty string should return 0");

    TEST_ASSERT_EQUAL_INT_MESSAGE(0, atoi("-"), "A minus sign without digits should return 0");

    TEST_ASSERT_EQUAL_INT_MESSAGE(3, atoi("3.9"), "Should ignore the decimal portion");

    TEST_ASSERT_EQUAL_INT_MESSAGE(1, atoi("1 2"), "Should stop reading at the space");
}

/*
 * STEP 6: test_atol
 *
 * Verify atol(). Check: "123456" gives 123456, "2000000000" gives 2000000000,
 * "-2000000000" gives -2000000000, "  -99" gives -99, "42xyz" gives 42 and "x"
 * gives 0. Keep all values within +-2,000,000,000: on a 32-bit system a long
 * has only 32 bits.
 *
 * HINT: atol() returns long. Cast it to int before the comparison:  (int)atol("123456")
 */

void test_atol(void)
{
    TEST_ASSERT_EQUAL_INT_MESSAGE(123456, (int)atol("123456"), "123456 should return 123456");

    TEST_ASSERT_EQUAL_INT_MESSAGE(2000000000, (int)atol("2000000000"), "2000000000 should return 2000000000");

    TEST_ASSERT_EQUAL_INT_MESSAGE(-2000000000, (int)atol("-2000000000"), "-2000000000 should return -2000000000");

    TEST_ASSERT_EQUAL_INT_MESSAGE(-99, (int)atol("  -99"), "Leading spaces should be skipped");

    TEST_ASSERT_EQUAL_INT_MESSAGE(42, (int)atol("42xyz"), "Should stop reading at the first non-digit");

    TEST_ASSERT_EQUAL_INT_MESSAGE(0, (int)atol("x"), "Non-numeric string should return 0");
}

/*
 * STEP 7: test_atof
 *
 * Verify atof(). Check: "3.5" gives 3.5, "-0.25" gives -0.25, "1e3" gives
 * 1000.0 (scientific notation), "  2.5xyz" gives 2.5, "abc" gives 0.0, ".5"
 * gives 0.5 and "42" gives 42.0.
 *
 * HINT: Use TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(expected, actual, "message"). Unity compares doubles with a very small tolerance, because most decimal numbers cannot be stored exactly in binary.
 */

void test_atof(void)
{
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(3.5, atof("3.5"), "3.5 should return 3.5");

    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(-0.25, atof("-0.25"), "-0.25 should return -0.25");

    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(1000.0, atof("1e3"), "Scientific notation should return 1000.0");

    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(2.5, atof("  2.5xyz"), "Should stop reading at the first invalid character");

    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(0.0, atof("abc"), "Non-numeric string should return 0.0");

    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(0.5, atof(".5"), ".5 should return 0.5");

    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(42.0, atof("42"), "42 should return 42.0");
}

/* ======================================================================
 * PART 2: Math (cases are listed; choose the right assertion)
 * ====================================================================== */

/*
 * STEP 8: test_sqrt
 *
 * Verify sqrt(). Check: sqrt(0) is 0, sqrt(1) is 1, sqrt(16) is 4, sqrt(144)
 * is 12, sqrt(0.25) is 0.5, sqrt(1000000) is 1000 and sqrt(2) is
 * 1.4142135623730951.
 *
 * HINT: sqrt(2) is irrational. TEST_ASSERT_EQUAL_DOUBLE_MESSAGE still works, because of the tolerance.
 */

void test_sqrt(void)
{
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(0.0, sqrt(0.0), "sqrt(0) should be 0");

    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(1.0, sqrt(1.0), "sqrt(1) should be 1");

    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(4.0, sqrt(16.0), "sqrt(16) should be 4");

    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(12.0, sqrt(144.0), "sqrt(144) should be 12");

    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(0.5, sqrt(0.25), "sqrt(0.25) should be 0.5");

    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(1000.0, sqrt(1000000.0), "sqrt(1000000) should be 1000");

    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(1.4142135623730951, sqrt(2.0), "sqrt(2) should be approximately 1.4142135623730951");
}

/*
 * STEP 9: test_pow
 *
 * Verify pow(). Check: pow(2, 10) is 1024; pow(5, 0) is 1; pow(0, 0) is 1;
 * pow(2, -1) is 0.5 (negative exponent); pow(-2, 3) is -8 and pow(-2, 2) is 4
 * (negative base); pow(9, 0.5) is 3 (fractional exponent); pow(10, 2) is 100;
 * pow(1, 1000) is 1.
 *
 * HINT: All arguments of pow() are doubles: write 2.0 and 10.0.
 */

void test_pow(void)
{
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(1024.0, pow(2.0, 10.0), "pow(2, 10) should be 1024");

    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(1.0, pow(5.0, 0.0), "Any number to the power of 0 should be 1");

    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(1.0, pow(0.0, 0.0), "pow(0, 0) should be 1");

    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(0.5, pow(2.0, -1.0), "Negative exponent should return 0.5");

    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(-8.0, pow(-2.0, 3.0), "pow(-2, 3) should be -8");

    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(4.0, pow(-2.0, 2.0), "pow(-2, 2) should be 4");

    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(3.0, pow(9.0, 0.5), "pow(9, 0.5) should be 3");

    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(100.0, pow(10.0, 2.0), "pow(10, 2) should be 100");

    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(1.0, pow(1.0, 1000.0), "pow(1, 1000) should be 1");
}

/*
 * STEP 10: test_floor
 *
 * Verify floor(), which rounds DOWN. Check: floor(2.7) is 2, floor(2.0) is 2,
 * floor(0.5) is 0, floor(-2.1) is -3, floor(-2.0) is -2 and floor(-0.5) is -1.
 * The negative numbers are the interesting part: down means toward minus
 * infinity, not toward zero.
 *
 * HINT: The result is a double: use TEST_ASSERT_EQUAL_DOUBLE_MESSAGE with 2.0, -3.0, ...
 */

void test_floor(void)
{
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(2.0, floor(2.7), "floor(2.7) should be 2");

    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(2.0, floor(2.0), "floor(2.0) should be 2");

    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(0.0, floor(0.5), "floor(0.5) should be 0");

    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(-3.0, floor(-2.1), "floor(-2.1) should be -3");

    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(-2.0, floor(-2.0), "floor(-2.0) should be -2");

    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(-1.0, floor(-0.5), "floor(-0.5) should be -1");
}

/*
 * STEP 11: test_ceil
 *
 * Verify ceil(), which rounds UP. Check: ceil(2.1) is 3, ceil(2.0) is 2,
 * ceil(0.5) is 1, ceil(-2.7) is -2, ceil(-2.0) is -2 and ceil(5.0) is 5. A
 * value that is already an integer must stay unchanged.
 *
 * HINT: Compare with floor() from the previous step: they differ for 2.1 and agree for 2.0.
 */

void test_ceil(void)
{
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(3.0, ceil(2.1), "ceil(2.1) should be 3");

    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(2.0, ceil(2.0), "ceil(2.0) should be 2");

    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(1.0, ceil(0.5), "ceil(0.5) should be 1");

    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(-2.0, ceil(-2.7), "ceil(-2.7) should be -2");

    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(-2.0, ceil(-2.0), "ceil(-2.0) should be -2");

    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(5.0, ceil(5.0), "ceil(5.0) should be 5");
}

/*
 * STEP 12: test_round
 *
 * Verify round(), which rounds to the NEAREST integer, with halves going AWAY
 * from zero. Check: round(2.4) is 2, round(2.5) is 3, round(2.6) is 3,
 * round(-2.4) is -2, round(-2.5) is -3, round(-2.6) is -3, round(0.0) is 0 and
 * round(7.0) is 7.
 *
 * HINT: The negative halves (-2.5) are where a wrong rounding rule shows up.
 */

void test_round(void)
{
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(2.0, round(2.4), "round(2.4) should be 2");

    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(3.0, round(2.5), "round(2.5) should be 3");

    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(3.0, round(2.6), "round(2.6) should be 3");

    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(-2.0, round(-2.4), "round(-2.4) should be -2");

    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(-3.0, round(-2.5), "round(-2.5) should be -3");

    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(-3.0, round(-2.6), "round(-2.6) should be -3");

    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(0.0, round(0.0), "round(0.0) should be 0");

    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(7.0, round(7.0), "round(7.0) should be 7");

}

/*
 * STEP 13: test_fmod
 *
 * Verify fmod(), the remainder of a floating point division. Check: fmod(7.5,
 * 2) is 1.5; fmod(10, 3) is 1; fmod(9, 3) is 0; fmod(-7.5, 2) is -1.5 and
 * fmod(7.5, -2) is 1.5 (the result has the sign of the FIRST argument);
 * fmod(2, 5) is 2.
 *
 * HINT: This is the operator % for doubles, which C does not have.
 */

void test_fmod(void)
{
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(1.5, fmod(7.5, 2.0), "fmod(7.5, 2) should be 1.5");

    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(1.0, fmod(10.0, 3.0), "fmod(10, 3) should be 1");

    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(0.0, fmod(9.0, 3.0), "fmod(9, 3) should be 0");

    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(-1.5, fmod(-7.5, 2.0), "fmod(-7.5, 2) should be -1.5");

    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(1.5, fmod(7.5, -2.0), "fmod(7.5, -2) should be 1.5");

    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(2.0, fmod(2.0, 5.0), "fmod(2, 5) should be 2");
}

/*
 * STEP 14: test_floor_ceil_round_relation
 *
 * Combine floor(), ceil() and round() in one test, using loops over arrays.
 * For every value of {0.1, 2.5, -2.5, -0.9, 100.99, -100.01} check that
 * ceil(x) - floor(x) is exactly 1, that floor(x) <= x, that x <= ceil(x), and
 * that round(x) lies between floor(x) and ceil(x). For every value of {-3.0,
 * 0.0, 8.0} check that floor(x), ceil(x) and round(x) all equal x.
 *
 * HINT: Declare const double arrays and loop over them. Turn the comparisons into numbers with TEST_ASSERT_EQUAL_INT_MESSAGE(1, floor(x) <= x, ...). A relation that must hold for MANY values is a stronger test than a few single examples.
 */

void test_floor_ceil_round_relation(void)
{
    const double values[] = {0.1, 2.5, -2.5, -0.9, 100.99, -100.01};
    const double integers[] = {-3.0, 0.0, 8.0};

    for (int i = 0; i < 6; i++)
    {
        double x = values[i];

        TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(1.0, ceil(x) - floor(x),
            "ceil(x) - floor(x) should equal 1");

        TEST_ASSERT_EQUAL_INT_MESSAGE(1, floor(x) <= x,
            "floor(x) should be less than or equal to x");

        TEST_ASSERT_EQUAL_INT_MESSAGE(1, x <= ceil(x),
            "x should be less than or equal to ceil(x)");

        TEST_ASSERT_EQUAL_INT_MESSAGE(1, floor(x) <= round(x) && round(x) <= ceil(x),
            "round(x) should be between floor(x) and ceil(x)");
    }

    for (int i = 0; i < 3; i++)
    {
        double x = integers[i];

        TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(x, floor(x),
            "floor(x) should equal x for integers");

        TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(x, ceil(x),
            "ceil(x) should equal x for integers");

        TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(x, round(x),
            "round(x) should equal x for integers");
    }
}

/* ======================================================================
 * PART 3: Memory, strings and formatting
 * ====================================================================== */

/*
 * STEP 15: test_memcpy_int_array
 *
 * Verify memcpy() on an int array. Copy int src[5] = {1, 2, 3, 4, -1} into a
 * zeroed int dest[5] with memcpy(dest, src, sizeof src) and check that dest
 * equals src. Check that memcpy() returns the destination pointer. Then copy
 * only the first 3 ints (3 * sizeof(int) bytes) into a freshly zeroed array
 * and check that the result is {1, 2, 3, 0, 0}.
 *
 * HINT: TEST_ASSERT_EQUAL_INT_ARRAY_MESSAGE(expected, actual, number_of_elements, "message") compares two int arrays element by element. The size of memcpy() is in BYTES, the number of elements is not.
 *       Why -1 as the last value? The int -1 has ALL its bytes non-zero. If memcpy() copied one byte too few, the missing byte would be 0 in a zeroed destination and a value like 5 would still look correct - a test that cannot see the bug.
 *       A pointer comparison such as  (returned == dest)  gives 1 or 0.
 */

void test_memcpy_int_array(void)
{
    int src[5] = {1, 2, 3, 4, -1};
    int dest[5] = {0};

    void *returned = memcpy(dest, src, sizeof src);

    TEST_ASSERT_EQUAL_INT_ARRAY_MESSAGE(src, dest, 5,
        "memcpy should copy all 5 integers");

    TEST_ASSERT_EQUAL_INT_MESSAGE(1, returned == dest,
        "memcpy should return the destination pointer");

    int dest2[5] = {0};

    memcpy(dest2, src, 3 * sizeof(int));

    int expected[5] = {1, 2, 3, 0, 0};

    TEST_ASSERT_EQUAL_INT_ARRAY_MESSAGE(expected, dest2, 5,
        "memcpy should copy only the first 3 integers");
}

/*
 * STEP 16: test_memcpy_string_and_zero_bytes
 *
 * Verify memcpy() on text. Prepare char buf[10] with the first 9 bytes set to
 * 'X' (use memset) and the last byte set to '\0'. (1) memcpy(buf, "hello", 6)
 * copies the text and its terminator: buf must be the string "hello" and
 * buf[6] must still be 'X'. (2) Refill buf with 'X', copy only 5 bytes, and
 * check that the first 9 bytes are "helloXXXX" (no terminator was copied). (3)
 * Refill buf and copy 0 bytes: buf[0] must still be 'X'.
 *
 * HINT: Pre-filling the buffer is what makes the test meaningful: a bug that writes too much or too little would be invisible in a buffer full of zeros.
 *       TEST_ASSERT_EQUAL_MEMORY_MESSAGE(expected, actual, number_of_bytes, "message") compares raw bytes. Single characters can be compared as numbers with TEST_ASSERT_EQUAL_INT_MESSAGE('X', buf[6], ...).
 */

void test_memcpy_string_and_zero_bytes(void)
{
    char buf[10];

    // Fill first 9 bytes with 'X' and terminate the buffer
    memset(buf, 'X', 9);
    buf[9] = '\0';

    // (1) Copy "hello" including the null terminator
    memcpy(buf, "hello", 6);

    TEST_ASSERT_EQUAL_STRING_MESSAGE("hello", buf,
        "memcpy should copy hello and its null terminator");

    TEST_ASSERT_EQUAL_INT_MESSAGE('X', buf[6],
        "Byte after the copied string should remain X");

    // (2) Refill buffer and copy only 5 bytes
    memset(buf, 'X', 9);
    buf[9] = '\0';

    memcpy(buf, "hello", 5);

    char expected[9] = "helloXXXX";

    TEST_ASSERT_EQUAL_MEMORY_MESSAGE(expected, buf, 9,
        "memcpy should copy only 5 bytes without the terminator");

    // (3) Refill buffer and copy 0 bytes
    memset(buf, 'X', 9);
    buf[9] = '\0';

    memcpy(buf, "hello", 0);

    TEST_ASSERT_EQUAL_INT_MESSAGE('X', buf[0],
        "Copying 0 bytes should leave the buffer unchanged");
}

/*
 * STEP 17: test_memset_and_memcmp
 *
 * Verify memset() and memcmp(). With unsigned char buf[8]: memset(buf, 'A', 8)
 * must fill every byte (check the last one); memset(buf, 0, 8) must give 8
 * zero bytes (compare with an array of zeros); after memset(buf, 'B', 4) the
 * 4th byte is 'B' and the 5th byte is still 0. Then compare the strings a =
 * "abcdX" and b = "abcdY": memcmp(a, a, 5) is 0; memcmp(a, b, 4) is 0 (the
 * difference is outside the first 4 bytes); memcmp(a, b, 5) is negative and
 * memcmp(b, a, 5) is positive.
 *
 * HINT: Like strcmp(), memcmp() promises only the sign of its result. The last byte decides here - a buggy memcmp() that looks only at the first byte would not notice it.
 */

void test_memset_and_memcmp(void)
{
    unsigned char buf[8];

    // Fill all 8 bytes with 'A'
    memset(buf, 'A', 8);

    TEST_ASSERT_EQUAL_INT_MESSAGE('A', buf[7],
        "memset should fill every byte with A");

    // Fill all 8 bytes with zero
    memset(buf, 0, 8);

    unsigned char zeros[8] = {0};

    TEST_ASSERT_EQUAL_MEMORY_MESSAGE(zeros, buf, 8,
        "memset should set all 8 bytes to zero");

    // Fill only the first 4 bytes with 'B'
    memset(buf, 'B', 4);

    TEST_ASSERT_EQUAL_INT_MESSAGE('B', buf[3],
        "Fourth byte should be B");

    TEST_ASSERT_EQUAL_INT_MESSAGE(0, buf[4],
        "Fifth byte should remain zero");

    // Test memcmp
    const char a[] = "abcdX";
    const char b[] = "abcdY";

    TEST_ASSERT_EQUAL_INT_MESSAGE(0, memcmp(a, a, 5),
        "Comparing a with itself should return zero");

    TEST_ASSERT_EQUAL_INT_MESSAGE(0, memcmp(a, b, 4),
        "First 4 bytes should be identical");

    TEST_ASSERT_EQUAL_INT_MESSAGE(1, memcmp(a, b, 5) < 0,
        "a should be less than b");

    TEST_ASSERT_EQUAL_INT_MESSAGE(1, memcmp(b, a, 5) > 0,
        "b should be greater than a");
}

/*
 * STEP 18: test_strcpy_strcat
 *
 * Verify strcpy() and strcat(). Prepare char buf[20] with the first 19 bytes
 * set to 'X' and the last byte set to '\0'. strcpy(buf, "abc") must give the
 * string "abc", buf[3] must be '\0' (the terminator is copied), buf[4] must
 * still be 'X', and the function must return buf itself. strcat(buf, "def")
 * must give "abcdef" and return buf. strcat(buf, "") must change nothing.
 * strcat(buf, "!") must give "abcdef!", whose strlen() is 7.
 *
 * HINT: A pre-filled buffer shows whether the terminator was really copied: if strcpy() forgot it, the 'X' characters after "abc" would still be part of the string.
 */

void test_strcpy_strcat(void)
{
    char buf[20];

    // Fill first 19 bytes with 'X' and terminate
    memset(buf, 'X', 19);
    buf[19] = '\0';

    // Test strcpy
    char *returned = strcpy(buf, "abc");

    TEST_ASSERT_EQUAL_STRING_MESSAGE("abc", buf,
        "strcpy should copy abc");

    TEST_ASSERT_EQUAL_INT_MESSAGE('\0', buf[3],
        "strcpy should copy the null terminator");

    TEST_ASSERT_EQUAL_INT_MESSAGE('X', buf[4],
        "Byte after the terminator should remain X");

    TEST_ASSERT_EQUAL_INT_MESSAGE(1, returned == buf,
        "strcpy should return the destination pointer");

    // Test strcat
    returned = strcat(buf, "def");

    TEST_ASSERT_EQUAL_STRING_MESSAGE("abcdef", buf,
        "strcat should append def");

    TEST_ASSERT_EQUAL_INT_MESSAGE(1, returned == buf,
        "strcat should return the destination pointer");

    // Appending an empty string should change nothing
    strcat(buf, "");

    TEST_ASSERT_EQUAL_STRING_MESSAGE("abcdef", buf,
        "Appending an empty string should change nothing");

    // Append an exclamation mark
    strcat(buf, "!");

    TEST_ASSERT_EQUAL_STRING_MESSAGE("abcdef!", buf,
        "strcat should append an exclamation mark");

    TEST_ASSERT_EQUAL_INT_MESSAGE(7, (int)strlen(buf),
        "The final string should have length 7");
}

/*
 * STEP 19: test_strchr
 *
 * Verify strchr(). In s = "hello": the first 'l' is at index 2, 'h' is at
 * index 0 and 'o' is at index 4. A character that does not occur ('z') gives
 * NULL. The terminator can be searched for too: strchr(s, '\0') points to
 * index 5. In an empty string nothing is found: strchr("", 'a') is NULL.
 *
 * HINT: strchr() returns a POINTER. The index is the difference of two pointers:  (int)(found - s).
 *       Compare a pointer with NULL like this:  TEST_ASSERT_EQUAL_INT_MESSAGE(1, found == NULL, "...");
 */

void test_strchr(void)
{
    const char *s = "hello";
    const char *found;

    found = strchr(s, 'l');
    TEST_ASSERT_EQUAL_INT_MESSAGE(2, (int)(found - s),
        "First l should be at index 2");

    found = strchr(s, 'h');
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, (int)(found - s),
        "h should be at index 0");

    found = strchr(s, 'o');
    TEST_ASSERT_EQUAL_INT_MESSAGE(4, (int)(found - s),
        "o should be at index 4");

    found = strchr(s, 'z');
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, found == NULL,
        "z should not be found");

    found = strchr(s, '\0');
    TEST_ASSERT_EQUAL_INT_MESSAGE(5, (int)(found - s),
        "Null terminator should be at index 5");

    found = strchr("", 'a');
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, found == NULL,
        "a should not be found in an empty string");
}

/*
 * STEP 20: test_sprintf_integers
 *
 * Verify sprintf() with integer formats, using char buf[64]. Check the text
 * produced and, for the first two calls, also the return value (the number of
 * characters written): "%d" of 42 gives "42" (returns 2); "%d" of -7 gives
 * "-7" (returns 2); "%05d" of 42 gives "00042"; "%5d" of 42 gives "   42";
 * "%-5d|" of 42 gives "42   |"; "%x" of 255 gives "ff"; "%X" of 255 gives
 * "FF"; "%o" of 8 gives "10"; "%+d" of 5 gives "+5".
 *
 * HINT: TEST_ASSERT_EQUAL_STRING_MESSAGE(expected, actual, "message") compares two strings with strcmp().
 */

void test_sprintf_integers(void)
{
    char buf[64];
    int result;

    result = sprintf(buf, "%d", 42);
    TEST_ASSERT_EQUAL_STRING_MESSAGE("42", buf,
        "%d should format 42 as 42");
    TEST_ASSERT_EQUAL_INT_MESSAGE(2, result,
        "sprintf should return 2 characters");

    result = sprintf(buf, "%d", -7);
    TEST_ASSERT_EQUAL_STRING_MESSAGE("-7", buf,
        "%d should format -7 as -7");
    TEST_ASSERT_EQUAL_INT_MESSAGE(2, result,
        "sprintf should return 2 characters");

    sprintf(buf, "%05d", 42);
    TEST_ASSERT_EQUAL_STRING_MESSAGE("00042", buf,
        "%05d should pad with zeros");

    sprintf(buf, "%5d", 42);
    TEST_ASSERT_EQUAL_STRING_MESSAGE("   42", buf,
        "%5d should right-align the number");

    sprintf(buf, "%-5d|", 42);
    TEST_ASSERT_EQUAL_STRING_MESSAGE("42   |", buf,
        "%-5d should left-align the number");

    sprintf(buf, "%x", 255);
    TEST_ASSERT_EQUAL_STRING_MESSAGE("ff", buf,
        "%x should produce lowercase hexadecimal");

    sprintf(buf, "%X", 255);
    TEST_ASSERT_EQUAL_STRING_MESSAGE("FF", buf,
        "%X should produce uppercase hexadecimal");

    sprintf(buf, "%o", 8);
    TEST_ASSERT_EQUAL_STRING_MESSAGE("10", buf,
        "%o should format 8 as octal 10");

    sprintf(buf, "%+d", 5);
    TEST_ASSERT_EQUAL_STRING_MESSAGE("+5", buf,
        "%+d should include a plus sign");
}

/*
 * STEP 21: test_sprintf_strings_and_doubles
 *
 * Verify sprintf() with text and floating point formats, using char buf[64].
 * "%s" of "abc" gives "abc"; "%c" of 'Z' gives "Z"; "%-6s|" of "ab" gives "ab
 * |"; "%6s" of "ab" gives "    ab"; "%.2s" of "abcdef" gives "ab"; "%.2f" of
 * 3.14159 gives "3.14"; "%.3f" of 2.0 gives "2.000"; "%8.2f" of 3.14159 gives
 * "    3.14"; "%f" of 0.5 gives "0.500000"; and the format "100%%" gives
 * "100%".
 *
 * HINT: Count the spaces carefully: a width such as %8.2f pads the number on the left to 8 characters.
 */

void test_sprintf_strings_and_doubles(void)
{
    char buf[64];

    sprintf(buf, "%s", "abc");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("abc", buf, "%s should format abc");

    sprintf(buf, "%c", 'Z');
    TEST_ASSERT_EQUAL_STRING_MESSAGE("Z", buf, "%c should format Z");

    sprintf(buf, "%-6s|", "ab");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("ab    |", buf, "%-6s should left-align");

    sprintf(buf, "%6s", "ab");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("    ab", buf, "%6s should right-align");

    sprintf(buf, "%.2s", "abcdef");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("ab", buf, "%.2s should limit string length");

    sprintf(buf, "%.2f", 3.14159);
    TEST_ASSERT_EQUAL_STRING_MESSAGE("3.14", buf, "%.2f should show 2 decimal places");

    sprintf(buf, "%.3f", 2.0);
    TEST_ASSERT_EQUAL_STRING_MESSAGE("2.000", buf, "%.3f should show 3 decimal places");

    sprintf(buf, "%8.2f", 3.14159);
    TEST_ASSERT_EQUAL_STRING_MESSAGE("    3.14", buf, "%8.2f should use width 8");

    sprintf(buf, "%f", 0.5);
    TEST_ASSERT_EQUAL_STRING_MESSAGE("0.500000", buf, "%f should show 6 decimal places");

    sprintf(buf, "100%%");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("100%", buf, "%% should produce a percent sign");
}

/*
 * STEP 22: test_sprintf_return_matches_strlen
 *
 * sprintf() returns the number of characters it wrote (not counting the
 * terminator). For each of these calls assert the expected return value AND
 * that it equals strlen(buf): "%d" of 12345 (5); "%s-%s" of "ab" and "cd" (5);
 * "%5.2f" of 3.14159 (5); "%s" of "" (0 - and the buffer must be the empty
 * string); "%s: %s, %s: %d" of "Name", "Ann", "Age", 21, which writes "Name:
 * Ann, Age: 21" (18 characters).
 *
 * HINT: This test uses sprintf() and strlen() together: if either one is wrong, the two numbers disagree.
 */

void test_sprintf_return_matches_strlen(void)
{
    char buf[64];
    int result;

    result = sprintf(buf, "%d", 12345);
    TEST_ASSERT_EQUAL_INT_MESSAGE(5, result, "Should return 5 characters");
    TEST_ASSERT_EQUAL_INT_MESSAGE((int)strlen(buf), result, "Return value should match strlen");

    result = sprintf(buf, "%s-%s", "ab", "cd");
    TEST_ASSERT_EQUAL_INT_MESSAGE(5, result, "Should return 5 characters");
    TEST_ASSERT_EQUAL_INT_MESSAGE((int)strlen(buf), result, "Return value should match strlen");

    result = sprintf(buf, "%5.2f", 3.14159);
    TEST_ASSERT_EQUAL_INT_MESSAGE(5, result, "Should return 5 characters");
    TEST_ASSERT_EQUAL_INT_MESSAGE((int)strlen(buf), result, "Return value should match strlen");

    result = sprintf(buf, "%s", "");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, result, "Should return 0 characters");
    TEST_ASSERT_EQUAL_INT_MESSAGE((int)strlen(buf), result, "Return value should match strlen");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("", buf, "Buffer should be empty");

    result = sprintf(buf, "%s: %s, %s: %d", "Name", "Ann", "Age", 21);
    TEST_ASSERT_EQUAL_INT_MESSAGE(18, result, "Should return 18 characters");
    TEST_ASSERT_EQUAL_INT_MESSAGE((int)strlen(buf), result, "Return value should match strlen");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("Name: Ann, Age: 21", buf, "Formatted string should match");
}

/* ======================================================================
 * PART 4: qsort and integration (you choose the inputs - the checklist tells you what must be covered)
 * ====================================================================== */

/*
 * STEP 23: test_qsort_ints
 *
 * Verify qsort() on int arrays, sorted ascending with the given comparator
 * cmp_int. Your test must cover: an unsorted array {5, 2, 9, 1, 7} (becomes
 * {1, 2, 5, 7, 9}); a reversed array; an already sorted array (stays
 * unchanged); an array with duplicates {3, 1, 3, 1}; an array with negative
 * numbers; an array whose SMALLEST element is the LAST one (for example {9, 8,
 * 7, 0}); an array with a single element; and count 0, which must not touch
 * the array (use an array you can check afterwards).
 *
 * HINT: qsort(base, count, sizeof(int), cmp_int);  Compare the whole array with TEST_ASSERT_EQUAL_INT_ARRAY_MESSAGE against an array that holds the expected result.
 *       A test that only uses already sorted data can never fail - use unsorted inputs.
 */

void test_qsort_ints(void)
{
    int arr1[] = {5, 2, 9, 1, 7};
    int expected1[] = {1, 2, 5, 7, 9};
    qsort(arr1, 5, sizeof(int), cmp_int);
    TEST_ASSERT_EQUAL_INT_ARRAY_MESSAGE(expected1, arr1, 5, "Unsorted array should be sorted");

    int arr2[] = {9, 7, 5, 2, 1};
    int expected2[] = {1, 2, 5, 7, 9};
    qsort(arr2, 5, sizeof(int), cmp_int);
    TEST_ASSERT_EQUAL_INT_ARRAY_MESSAGE(expected2, arr2, 5, "Reversed array should be sorted");

    int arr3[] = {1, 2, 5, 7, 9};
    int expected3[] = {1, 2, 5, 7, 9};
    qsort(arr3, 5, sizeof(int), cmp_int);
    TEST_ASSERT_EQUAL_INT_ARRAY_MESSAGE(expected3, arr3, 5, "Sorted array should remain unchanged");

    int arr4[] = {3, 1, 3, 1};
    int expected4[] = {1, 1, 3, 3};
    qsort(arr4, 4, sizeof(int), cmp_int);
    TEST_ASSERT_EQUAL_INT_ARRAY_MESSAGE(expected4, arr4, 4, "Duplicates should be sorted");

    int arr5[] = {-5, 3, -1, 0, -10};
    int expected5[] = {-10, -5, -1, 0, 3};
    qsort(arr5, 5, sizeof(int), cmp_int);
    TEST_ASSERT_EQUAL_INT_ARRAY_MESSAGE(expected5, arr5, 5, "Negative numbers should be sorted");

    int arr6[] = {9, 8, 7, 0};
    int expected6[] = {0, 7, 8, 9};
    qsort(arr6, 4, sizeof(int), cmp_int);
    TEST_ASSERT_EQUAL_INT_ARRAY_MESSAGE(expected6, arr6, 4, "Smallest element at the end should be sorted");

    int arr7[] = {42};
    int expected7[] = {42};
    qsort(arr7, 1, sizeof(int), cmp_int);
    TEST_ASSERT_EQUAL_INT_ARRAY_MESSAGE(expected7, arr7, 1, "Single element should remain unchanged");

    int arr8[] = {4, 2, 8};
    int expected8[] = {4, 2, 8};
    qsort(arr8, 0, sizeof(int), cmp_int);
    TEST_ASSERT_EQUAL_INT_ARRAY_MESSAGE(expected8, arr8, 3, "Zero count should not modify the array");
}

/*
 * STEP 24: test_qsort_chars
 *
 * Verify qsort() on the characters of a string, using the comparator cmp_char
 * that you write (see below) and strlen() to get the count. Your test must
 * cover: "hello" (becomes "ehllo"); "dcba" (becomes "abcd"); an already sorted
 * string "abc"; a string of equal letters "zzz"; and the empty string ""
 * (stays empty). Sort a char array (not a string literal - literals are read-
 * only).
 *
 * HINT: char word[] = "hello";  qsort(word, strlen(word), sizeof(char), cmp_char);  then TEST_ASSERT_EQUAL_STRING_MESSAGE("ehllo", word, "...");
 */

void test_qsort_chars(void)
{
    char word1[] = "hello";
    qsort(word1, strlen(word1), sizeof(char), cmp_char);
    TEST_ASSERT_EQUAL_STRING_MESSAGE("ehllo", word1, "hello should become ehllo");

    char word2[] = "dcba";
    qsort(word2, strlen(word2), sizeof(char), cmp_char);
    TEST_ASSERT_EQUAL_STRING_MESSAGE("abcd", word2, "dcba should become abcd");

    char word3[] = "abc";
    qsort(word3, strlen(word3), sizeof(char), cmp_char);
    TEST_ASSERT_EQUAL_STRING_MESSAGE("abc", word3, "Sorted string should remain unchanged");

    char word4[] = "zzz";
    qsort(word4, strlen(word4), sizeof(char), cmp_char);
    TEST_ASSERT_EQUAL_STRING_MESSAGE("zzz", word4, "Equal characters should remain unchanged");

    char word5[] = "";
    qsort(word5, strlen(word5), sizeof(char), cmp_char);
    TEST_ASSERT_EQUAL_STRING_MESSAGE("", word5, "Empty string should remain empty");
}

/*
 * STEP 25: test_qsort_strings
 *
 * Verify qsort() on an array of strings (char * elements), using the
 * comparators cmp_strlen and cmp_str that you write (see below). Sort {"ccc",
 * "a", "dddd", "bb"} by length: the result is "a", "bb", "ccc", "dddd". Sort
 * {"pear", "apple", "Zed", "app"} alphabetically with strcmp(): the result is
 * "Zed", "app", "apple", "pear" (uppercase first, and a prefix comes before
 * the longer word). Check every element of both results.
 *
 * HINT: Here every element is a char *, so the comparators receive pointers to char * (really char **) - see compare_strptr_asc from the previous assignment. The element size is sizeof(char *).
 *       qsort() is not stable, so these inputs contain no ties (no two strings with the same length).
 */

void test_qsort_strings(void)
{
    char *by_length[] = {"ccc", "a", "dddd", "bb"};
    char *expected_length[] = {"a", "bb", "ccc", "dddd"};

    qsort(by_length, 4, sizeof(char *), cmp_strlen);

    for (int i = 0; i < 4; i++)
    {
        TEST_ASSERT_EQUAL_STRING_MESSAGE(expected_length[i], by_length[i],
            "Strings should be sorted by length");
    }

    char *alphabetical[] = {"pear", "apple", "Zed", "app"};
    char *expected_alpha[] = {"Zed", "app", "apple", "pear"};

    qsort(alphabetical, 4, sizeof(char *), cmp_str);

    for (int i = 0; i < 4; i++)
    {
        TEST_ASSERT_EQUAL_STRING_MESSAGE(expected_alpha[i], alphabetical[i],
            "Strings should be sorted alphabetically");
    }
}

/*
 * STEP 26: test_roundtrip_sprintf_atoi_atof
 *
 * A round trip: format a number with sprintf(), parse the text back, and
 * expect the original number. For the ints {0, 7, -15, 1234567, -2000000000}
 * use "%d" and check that the text has the expected length (1, 1, 3, 7, 11),
 * that atoi() gives the number back and that atol() gives it back. For the
 * doubles {0.5, -2.25, 100.75, 3.0} use "%.2f" and check that atof() gives the
 * number back. Use loops over arrays.
 *
 * HINT: This test uses sprintf, strlen, atoi, atol and atof together. Use constant arrays for the values and for the expected lengths.
 */

void test_roundtrip_sprintf_atoi_atof(void)
{
    char buf[64];

    const int int_values[] = {0, 7, -15, 1234567, -2000000000};
    const int expected_lengths[] = {1, 1, 3, 7, 11};

    for (int i = 0; i < 5; i++)
    {
        sprintf(buf, "%d", int_values[i]);

        TEST_ASSERT_EQUAL_INT_MESSAGE(expected_lengths[i], (int)strlen(buf),
            "Formatted integer should have the expected length");

        TEST_ASSERT_EQUAL_INT_MESSAGE(int_values[i], atoi(buf),
            "atoi should recover the original integer");

        TEST_ASSERT_EQUAL_INT_MESSAGE(int_values[i], (int)atol(buf),
            "atol should recover the original integer");
    }

    const double double_values[] = {0.5, -2.25, 100.75, 3.0};

    for (int i = 0; i < 4; i++)
    {
        sprintf(buf, "%.2f", double_values[i]);

        TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(double_values[i], atof(buf),
            "atof should recover the original double");
    }
}

/*
 * STEP 27: test_anagram_with_qsort
 *
 * Write the helper is_anagram(a, b) (see below), then test it. Two strings are
 * anagrams if they contain the same letters in a different order. Your test
 * must cover: "listen" and "silent" (yes); "hello" and "world" (no); "abc" and
 * "abcd" (no - different lengths); "" and "" (yes); "aab" and "abb" (no - same
 * length, different letters); "Listen" and "silent" (no - case matters); "a"
 * and "a" (yes).
 *
 * HINT: The function returns 1 for yes and 0 for no, so TEST_ASSERT_EQUAL_INT_MESSAGE(1, is_anagram("listen", "silent"), "...") works.
 */

void test_anagram_with_qsort(void)
{
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, is_anagram("listen", "silent"),
        "listen and silent should be anagrams");

    TEST_ASSERT_EQUAL_INT_MESSAGE(0, is_anagram("hello", "world"),
        "hello and world should not be anagrams");

    TEST_ASSERT_EQUAL_INT_MESSAGE(0, is_anagram("abc", "abcd"),
        "Strings of different lengths should not be anagrams");

    TEST_ASSERT_EQUAL_INT_MESSAGE(1, is_anagram("", ""),
        "Two empty strings should be anagrams");

    TEST_ASSERT_EQUAL_INT_MESSAGE(0, is_anagram("aab", "abb"),
        "Different letters should not be anagrams");

    TEST_ASSERT_EQUAL_INT_MESSAGE(0, is_anagram("Listen", "silent"),
        "Anagrams should be case-sensitive");

    TEST_ASSERT_EQUAL_INT_MESSAGE(1, is_anagram("a", "a"),
        "Identical single-character strings should be anagrams");
}
