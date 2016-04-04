#include <cgreen/cute_reporter.h>
#include <cgreen/reporter.h>
#include <cgreen/breadcrumb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
namespace cgreen {
#endif

typedef struct {
    CutePrinter *printer;
    int error_count;    // For status within the test case process
    int previous_error; // For status outside the test case process
} CuteMemo;

static void cute_suite_started(TestReporter *reporter,
        const char *name, const int number_of_tests);
static void cute_start_test(TestReporter *reporter,
        const char *name);
static void show_fail(TestReporter *reporter, const char *file, int line,
        const char *message, va_list arguments);
static void show_pass(TestReporter *reporter, const char *file, int line,
        const char *message, va_list arguments);
static void cute_failed_to_complete(TestReporter *reporter,
        const char *file, int line, const char *message, va_list arguments);
static void cute_finish_test(TestReporter *reporter,
        const char *filename, int line, const char *message, uint32_t duration_in_milliseconds);
static void cute_finish_suite(TestReporter *reporter,
        const char *filename, int line, uint32_t duration_in_milliseconds);

void set_cute_printer(TestReporter *reporter, CutePrinter *printer) {
    CuteMemo *memo = (CuteMemo *) reporter->memo;
    memo->printer = printer;
}

TestReporter *create_cute_reporter(void) {
    CuteMemo *memo;
    TestReporter *reporter;

    reporter = create_reporter();
    if (reporter == NULL) {
        return NULL;
    }

    memo = (CuteMemo *) malloc(sizeof(CuteMemo) + 100);
    if (memo == NULL) {
        destroy_reporter(reporter);
        return NULL;
    }

    memo->printer = printf;

    reporter->start_suite = &cute_suite_started;
    reporter->start_test = &cute_start_test;
    reporter->show_fail = &show_fail;
    reporter->show_pass = &show_pass;
    reporter->show_incomplete = &cute_failed_to_complete;
    reporter->finish_test = &cute_finish_test;
    reporter->finish_suite = &cute_finish_suite;
    reporter->memo = memo;

    return reporter;
}

static void cute_suite_started(TestReporter *reporter,
        const char *name, const int number_of_tests) {
    CuteMemo *memo = (CuteMemo *) reporter->memo;
    reporter_start(reporter, name);
    memo->printer("#beginning %s %d\n", name, number_of_tests);
}

static void cute_start_test(TestReporter *reporter,
        const char *name) {
    CuteMemo *memo = (CuteMemo *) reporter->memo;
    memo->error_count = reporter->failures + reporter->exceptions;
    memo->previous_error = 0;
    reporter_start(reporter, name);
    memo->printer("#starting %s\n", name);
}

static void cute_finish_test(TestReporter *reporter, const char *filename, int line, const char *message,
                                            uint32_t duration_in_milliseconds) {
    CuteMemo *memo = (CuteMemo *) reporter->memo;
    const char *name = get_current_from_breadcrumb((CgreenBreadcrumb *)reporter->breadcrumb);

    reporter_finish(reporter, filename, line, message, duration_in_milliseconds);
    if (memo->error_count == reporter->failures + reporter->exceptions) {
        memo->printer("#success %s, %d ms OK\n", name);
    }
}

static void cute_finish_suite(TestReporter *reporter, const char *filename, int line,
                                         uint32_t duration_in_milliseconds) {
    CuteMemo *memo = (CuteMemo *) reporter->memo;
    const char *name = get_current_from_breadcrumb((CgreenBreadcrumb *)reporter->breadcrumb);
    reporter_finish(reporter, filename, line, NULL, duration_in_milliseconds);

    memo->printer("#ending %s", name);
    if (get_breadcrumb_depth((CgreenBreadcrumb *) reporter->breadcrumb) == 0) {
        memo->printer(": %d pass%s, %d failure%s, %d exception%s, %d ms.\n",
                reporter->passes, reporter->passes == 1 ? "" : "es",
                reporter->failures, reporter->failures == 1 ? "" : "s",
                reporter->exceptions, reporter->exceptions == 1 ? "" : "s",
                duration_in_milliseconds);
    } else
        memo->printer("\n");
}

static void show_fail(TestReporter *reporter, const char *file, int line,
        const char *message, va_list arguments) {
    CuteMemo *memo = (CuteMemo *) reporter->memo;
    if (!memo->previous_error) {
        char buffer[1000];
        memo->printer("#failure %s",
                get_current_from_breadcrumb(
                        (CgreenBreadcrumb *) reporter->breadcrumb));
        memo->printer(" %s:%d ", file, line);
        vsprintf(buffer, (message == NULL ? "Problem" : message), arguments);
        memo->printer("%s\n", buffer);
        memo->previous_error = 1;
    }
}

static void show_pass(TestReporter *reporter, const char *file, int line,
        const char *message, va_list arguments) {
    (void) reporter;
    (void) file;
    (void) line;
    (void) message;
    (void) arguments;
}

static void cute_failed_to_complete(TestReporter *reporter,
        const char *file, int line, const char *message, va_list arguments) {
    /* TODO: add additional message to output */
    CuteMemo *memo;
    (void)file;
    (void)line;
    (void)message;
    (void)arguments;

    memo = (CuteMemo *) reporter->memo;
    memo->printer("#error %s failed to complete\n",
            get_current_from_breadcrumb((CgreenBreadcrumb *)reporter->breadcrumb));
}

#ifdef __cplusplus
} // namespace cgreen
#endif

/* vim: set ts=4 sw=4 et cindent: */
