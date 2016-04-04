/* Note: the whole point of these tests is that they should fail!
   This makes it hard to think straight and to give the variables good
   names ;-) */

#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>
#include <signal.h>
#include <float.h>

#ifdef __cplusplus
using namespace cgreen;
#endif

Describe(ConstraintMessage);
BeforeEach(ConstraintMessage) {}
AfterEach(ConstraintMessage) {}

Ensure(ConstraintMessage,for_is_null) {
    int i;
    int *pointer = &i;
    assert_that(pointer, is_null);
}

Ensure(ConstraintMessage,for_is_non_null) {
    int *pointer = NULL;
    assert_that(pointer, is_non_null);
}

Ensure(ConstraintMessage,for_is_equal_to) {
    int fourty_five = 45, thirty_three = 33;
    assert_that(fourty_five, is_equal_to(thirty_three));
}

Ensure(ConstraintMessage, for_is_equal_to_hex) {
    char chars[4];
    memset(chars, 0xaa, sizeof(chars));
    assert_that(chars[0], is_equal_to_hex(0xbb));
}

Ensure(ConstraintMessage,for_is_not_equal_to) {
    int should_not_be_fourty_five = 45, forty_five = 45;
    assert_that(should_not_be_fourty_five, is_not_equal_to(forty_five));
}

Ensure(ConstraintMessage,for_is_greater_than) {
    int fourty_five = 45, thirty_three = 33;
    assert_that(thirty_three, is_greater_than(fourty_five));
}

Ensure(ConstraintMessage,for_is_less_than) {
    int fourty_five = 45, thirty_three = 33;
    assert_that(fourty_five, is_less_than(thirty_three));
}

Ensure(ConstraintMessage,for_is_equal_to_double) {
    double four_point_five = 4.5f, three_point_three = 3.3f;
    assert_that_double(four_point_five, is_equal_to_double(three_point_three));
}

Ensure(ConstraintMessage,for_is_not_equal_to_double) {
    significant_figures_for_assert_double_are(4);
    double epsilon = 1.0e-4;
    double four_point_five = 4.5, almost_four_point_five = 4.5 - epsilon;
    assert_that_double(four_point_five, is_not_equal_to_double(almost_four_point_five));
}

Ensure(ConstraintMessage,for_is_greater_than_double) {
    double four_point_five = 4.5f, three_point_three = 3.3f;
    assert_that_double(three_point_three, is_greater_than_double(four_point_five));
}

Ensure(ConstraintMessage,for_is_less_than_double) {
    double four_point_five = 4.5f, three_point_three = 3.3f;
    assert_that_double(four_point_five, is_less_than_double(three_point_three));
}

Ensure(ConstraintMessage,for_is_equal_to_contents_of) {
    int fourty_five[45] = {45, 44, 43}, thirty_three[33] = {45, 44, 33};
    assert_that(thirty_three, is_equal_to_contents_of(fourty_five, 55));
}

Ensure(ConstraintMessage,for_is_not_equal_to_contents_of) {
    int fourty_five_and_up[3] = {45, 46, 47}, another_fourty_five_and_up[3] = {45, 46, 47};
    assert_that(fourty_five_and_up, is_not_equal_to_contents_of(another_fourty_five_and_up, 3));
}

Ensure(ConstraintMessage,for_is_equal_to_string) {
    const char *fourty_five = "this string is fortyfive", *thirty_three = "this string is thirtythree";
    assert_that(thirty_three, is_equal_to_string(fourty_five));
}

Ensure(ConstraintMessage,for_is_not_equal_to_string) {
    const char *fourty_five = "this string is fourtyfive", *another_fourty_five = "this string is fourtyfive";
    assert_that(another_fourty_five, is_not_equal_to_string(fourty_five));
}

Ensure(ConstraintMessage,for_contains_string) {
    const char *fourty_five = "fortyfive", *not_containing_fourty_five = "this text is thirtythree";
    assert_that(not_containing_fourty_five, contains_string(fourty_five));
}

Ensure(ConstraintMessage,for_does_not_contain_string) {
    const char *contains_fourty_five = "this string is fourtyfive", *fourty_five = "fourtyfive";
    assert_that(contains_fourty_five, does_not_contain_string(fourty_five));
}

Ensure(ConstraintMessage,for_begins_with_string) {
    const char *does_not_begin_with_fourty_five = "this string does not begin with fortyfive", *fourty_five = "fourtyfive";
    assert_that(does_not_begin_with_fourty_five, begins_with_string(fourty_five));
}

Ensure(ConstraintMessage,for_equal_to_double) {
  assert_that_double(0, is_equal_to_double(1));
}

Ensure(ConstraintMessage,for_equal_to_double_negative) {
  assert_that_double(-1, is_equal_to_double(-2));
}

Ensure(ConstraintMessage,for_not_equal_to_double) {
  assert_that_double(0, is_not_equal_to_double(0));
}

Ensure(ConstraintMessage,for_not_equal_to_double_negative) {
  assert_that_double(-1, is_not_equal_to_double(-1));
}

Ensure(ConstraintMessage,for_is_less_than_double_with_accuracy) {
  significant_figures_for_assert_double_are(4);
  assert_that_double(1.0, is_less_than_double(1.0 - 1.0e-3 - DBL_EPSILON));
}

Ensure(ConstraintMessage,for_is_greater_than_double_with_accuracy) {
  significant_figures_for_assert_double_are(4);
  assert_that_double(1.0, is_greater_than_double(1.0 + 1.0e-3 + DBL_EPSILON));
}

Ensure(ConstraintMessage,for_assert_that) {
    assert_that(0 == 1);
}

void some_mock(int parameter) {
    mock(parameter);
}

Ensure(ConstraintMessage, for_mock_called_without_expectation) {
    some_mock(0);
}

Ensure(ConstraintMessage, for_mock_called_more_times_than_expected) {
    expect(some_mock);
    some_mock(0);
    some_mock(0);
}

Ensure(ConstraintMessage, for_mock_called_with_unexpected_parameter_value) {
    expect(some_mock, when(parameter, is_equal_to(1)));
    some_mock(0);
}

/* And here are some tests that will fail with "illegal" type of messages */
Ensure(ConstraintMessage, for_always_followed_by_expectation) {
    always_expect(some_mock, when(parameter, is_equal_to(1)));
    expect(some_mock, when(parameter, is_equal_to(0)));
}

Ensure(ConstraintMessage, for_violated_never_expect) {
    never_expect(some_mock, when(parameter, is_equal_to(1)));
    some_mock(1);
}

Ensure(ConstraintMessage, for_mock_parameter_name_not_matching_constraint_parameter_name) {
    expect(some_mock, when(PARAMETER, is_equal_to(0)));
    some_mock(0);
}

void forgot_to_pass_parameters_mock(int x) {
    (void)x; // to silence compiler warning
    mock();
}

Ensure(ConstraintMessage, for_no_mock_parameters_with_parameter_constraint) {
    expect(forgot_to_pass_parameters_mock, when(x, is_equal_to(0)));
    forgot_to_pass_parameters_mock(0);
}

Ensure(ConstraintMessage, increments_exception_count_when_terminating_via_SIGQUIT) {
    raise(SIGQUIT);
}

Ensure(ConstraintMessage, increments_exception_count_when_terminating_via_SIGTERM) {
    raise(SIGTERM);
}

#ifdef __cplusplus
Ensure(ConstraintMessage, for_incorrect_assert_throws) {
    assert_throws(std::string, throw "something else");
}

Ensure(ConstraintMessage, for_exception_was_expected_but_nothing_thrown) {
    assert_throws(std::string, (void)5);
}
#endif
