/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2026 Brian J. Downs
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#define _POSIX_C_SOURCE 199309L
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "crosscheck.h"

#define GREEN "\x1B[32m"
#define RED   "\x1B[31m"
#define RESET "\033[0m"

typedef struct {
    const char *function;
    const char *filename;
    uint64_t line;
    double duration_ms;
    cc_result_t result;
    bool passed;
} test_record_t;

static uint64_t count = 0;
static uint64_t passed = 0;
static uint64_t failed = 0;
static clock_t  start = 0;
static clock_t  end = 0;

static test_record_t *results = NULL;
static size_t result_count = 0;
static size_t result_capacity = 0;
static size_t longest_name = 0;

__attribute__((weak)) void cc_setup(void) {}
__attribute__((weak)) void cc_tear_down(void) {}

static bool
results_add(test_record_t record)
{
    if (result_count == result_capacity) {

        size_t new_capacity =
            result_capacity == 0 ? 64 : result_capacity * 2;

        test_record_t *tmp =
            realloc(results, new_capacity * sizeof(test_record_t));

        if (tmp == NULL)
            return false;

        results = tmp;
        result_capacity = new_capacity;
    }

    results[result_count++] = record;

    return true;
}

void
cc_init()
{
    start = clock();
    printf("Running tests...\n\n"); 
}

/**
* print_fail_info prints relevant data for the failure condition.
*/
static void
print_fail_info(const cc_result_t ret, const double time_spent)
{
    printf("  %-36s%18s:%-12"PRIu64 RED "%-8s" RESET " %-2.3f/ms\n",
        ret.function, ret.filename, ret.line, "failed", (time_spent*1000));

    if (ret.type == test_type_char) {
        printf("        expected: %c, got: %c\n",
            ret.exp.char_val, ret.act.char_val);
    } else if (ret.type == test_type_string) {
        printf("        expected: %s, got: %s\n",
            ret.exp.string_val, ret.act.string_val);
        free(ret.exp.string_val);
        free(ret.act.string_val);
    } else if (ret.type == test_type_bool) {
        if (ret.act.bool_val == false) {
            printf("        expected: true, got: false\n");
        } else {
            printf("        expected: false, got: true\n");
        }
    } else if (ret.type == test_type_float) {
        printf("        expected: %f, got: %f\n",
            ret.exp.float_val, ret.act.float_val);
    } else if (ret.type == test_type_double) {
        printf("        expected: %f, got: %f\n",
            ret.exp.double_val, ret.act.double_val);
    } else if (ret.type == test_type_long) {
        printf("        expected: %ld, got: %ld\n",
            ret.exp.long_val, ret.act.long_val);
    } else if (ret.type == test_type_long_long) {
        printf("        expected: %lld, got: %lld\n",
            ret.exp.long_long_val, ret.act.long_long_val);
    } else if (ret.type == test_type_int) {
        printf("        expected: %d, got: %d\n",
            ret.exp.int_val, ret.act.int_val);
    } else if (ret.type == test_type_int8) {
        printf("        expected: %d, got: %d\n",
            ret.exp.int8_val, ret.act.int8_val);
    } else if (ret.type == test_type_int16) {
        printf("        expected: %d, got: %d\n",
            ret.exp.int16_val, ret.act.int16_val);
    } else if (ret.type == test_type_int32) {
        printf("        expected: %d, got: %d\n",
            ret.exp.int32_val, ret.act.int32_val);
    } else if (ret.type == test_type_int64) {
        printf("        expected: %"PRId64", got: %"PRId64"\n",
            ret.exp.int64_val, ret.act.int64_val);
    } else if (ret.type == test_type_uint) {
        printf("        expected: %u, got: %u\n",
            ret.exp.uint_val, ret.act.uint_val);
    } else if (ret.type == test_type_uint8) {
        printf("        expected: %"PRIu8", got: %"PRIu8"\n",
            ret.exp.uint8_val, ret.act.uint8_val);
    } else if (ret.type == test_type_uint16) {
        printf("        expected: %"PRIu16", got: %"PRIu16"\n",
            ret.exp.uint16_val, ret.act.uint16_val);
    } else if (ret.type == test_type_uint32) {
        printf("        expected: %"PRIu32", got: %"PRIu32"\n",
            ret.exp.uint32_val, ret.act.uint32_val);
    } else if (ret.type == test_type_uint64) {
        printf("        expected: %"PRIu64", got: %"PRIu64"\n",
            ret.exp.uint64_val, ret.act.uint64_val);
    }
}

bool
cc_run(cc_func_t func)
{
    cc_setup();

    count++;

    struct timespec ts_start;
    struct timespec ts_end;
    clock_gettime(CLOCK_MONOTONIC, &ts_start);
    cc_result_t ret = func();
    clock_gettime(CLOCK_MONOTONIC, &ts_end);

    double elapsed_ms = (ts_end.tv_sec - ts_start.tv_sec) * 1000.0
                    + (ts_end.tv_nsec - ts_start.tv_nsec) / 1e6;

    bool ok = ret.result;
    ok ? passed++ : failed++;

    test_record_t record = {
        .function    = ret.function,
        .filename    = ret.filename,
        .line        = ret.line,
        .duration_ms = elapsed_ms,
        .result      = ret,
        .passed      = ok
    };
    if (!results_add(record)) {
        fprintf(stderr, "out of memory, test record lost\n");
        failed++;
    }

    size_t len = ret.function ? strlen(ret.function) : 0;
    if (len > longest_name) longest_name = len;

    cc_tear_down();
    
    return true;
}

uint64_t
cc_complete()
{
    end = clock();
    double ts = (double)(end - start) / CLOCKS_PER_SEC;

    for (size_t i = 0; i < result_count; i++) {
        test_record_t *r = &results[i];

        if (r->passed) {
            printf("  %-*s " GREEN "%-8s" RESET " %8.3f ms\n",
                (int)longest_name, r->function, "passed", r->duration_ms);
        } else {
            print_fail_info(r->result, r->duration_ms);
        }
    }

    printf("\nTotal: %-4"PRIu64 " Passed: %-4"PRIu64 " Failed: %-4"PRIu64 "in  %-2.3f/ms\n",
        count, passed, failed, (ts*1000));

    free(results);
    results = NULL;

    return failed;
}
