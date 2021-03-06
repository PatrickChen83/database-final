#include "gtest/gtest.h"
#include <iostream>
#include <tuple>
#include "action.h"

using namespace std;

TEST(ActionUsageTest, createValueFunction) {
    auto x1 = createValue(1);
    ASSERT_FALSE(x1 == NULL);
    EXPECT_EQ(x1->type, ValueType::Integer);
    EXPECT_EQ(x1->IntValue, 1);

    auto x2 = createValue("Value");
    ASSERT_FALSE(x2 == NULL);
    EXPECT_EQ(x2->type, ValueType::String);
    EXPECT_STREQ(x2->StrValue, "Value");

    delete x1;
    delete x2;
}

TEST(ActionUsageTest, InvalidRecord) {
    EXPECT_FALSE(isValidRecord(NULL));
    EXPECT_FALSE(isValidRecord("No double quotes"));
    EXPECT_FALSE(isValidRecord("\"\""));    // Empty record failed?
}

TEST(ActionUsageTest, ValidRecord) {
    EXPECT_TRUE(isValidRecord("\"Normal record\""));
    EXPECT_TRUE(isValidRecord("\"Normal record with spaces   \""));
    EXPECT_TRUE(isValidRecord("\"   \""));  // Space-filled record is valid?
}

TEST(ActionUsageTest, InvalidValue) {
    EXPECT_FALSE(isValidValue(NULL));
    EXPECT_FALSE(isValidValue("a"));
    EXPECT_FALSE(isValidValue("\"a"));
}

TEST(ActionUsageTest, ValidValue) {
    EXPECT_TRUE(isValidValue("10"));
    EXPECT_TRUE(isValidValue("0"));
    EXPECT_TRUE(isValidValue("\"a\""));
    EXPECT_TRUE(isValidValue("\"10\""));
}

TEST(ActionUsageTest, InvalidNullInput) {
    EXPECT_FALSE(isValidInput(NULL));
}

TEST(ActionUsageTest, InvalidRInput) {
    EXPECT_FALSE(isValidInput("R, Student, String, 0"));
    EXPECT_FALSE(isValidInput("R, Student, Integer, 0"));
    EXPECT_FALSE(isValidInput("R, Student, Integer, -20"));
    EXPECT_FALSE(isValidInput("R, Student, Integer, \"0\""));
    EXPECT_FALSE(isValidInput("R, Student, String,"));
}

TEST(ActionUsageTest, ValidRInput) {
    EXPECT_TRUE(isValidInput("R, Student, integer, 80"));
    EXPECT_TRUE(isValidInput("R, Student, Integer, 80"));
    EXPECT_TRUE(isValidInput("R, Student, string, 80"));
    EXPECT_TRUE(isValidInput("R, Student, String, 80"));
}

TEST(ActionUsageTest, InvalidIInput) {
    EXPECT_FALSE(isValidInput("I, Student,, \"80\""));
    EXPECT_FALSE(isValidInput("I, Student;"));
    EXPECT_FALSE(isValidInput("I, Student, \"string\", \"asd\"f\""));
    EXPECT_FALSE(isValidInput("I, Student, \"string\", \"asdf\"; 39"));
    EXPECT_FALSE(isValidInput("I, Student, \"string\", \"asdf\"; 39, "));
}

TEST(ActionUsageTest, ValidIInput) {
    EXPECT_TRUE(isValidIInput("I, Student, \"index1\", \"record1\""));
    EXPECT_TRUE(isValidIInput("I, Student, \"index1\", \"record1\"; \"index2\", \"record2\""));
    EXPECT_TRUE(isValidIInput("I, Student, \"index1\", \"record1;\"; \"index2\", \"record2\""));
    EXPECT_TRUE(isValidIInput("I, Student, \"index1\", \"record1\"; \"index2\", \"record2;\""));
}

TEST(ActionUsageTest, InvalidDInput) {
    EXPECT_FALSE(isValidInput("D, Student, a"));
    EXPECT_FALSE(isValidInput("D, Student,"));
}

TEST(ActionUsageTest, ValidDInput) {
    EXPECT_TRUE(isValidInput("D, Student, 10"));
    EXPECT_TRUE(isValidInput("D, Student, \"10\""));
}

TEST(ActionUsageTest, InvalidSInput) {
    EXPECT_FALSE(isValidInput("Scan, Student"));
    EXPECT_FALSE(isValidInput("Sca Student"));
    EXPECT_FALSE(isValidInput("Scan"));
    EXPECT_FALSE(isValidInput("Scan "));
    EXPECT_FALSE(isValidInput("Scan Student asdf"));
}

TEST(ActionUsageTest, ValidSInput) {
    EXPECT_TRUE(isValidInput("Scan Student"));
    EXPECT_TRUE(isValidInput("Scan  Student"));
}

TEST(ActionUsageTest, InvalidPInput) {
    EXPECT_FALSE(isValidInput("p"));
    EXPECT_FALSE(isValidInput("p "));
    EXPECT_FALSE(isValidInput("p Student "));
    EXPECT_FALSE(isValidInput("p Student \"asdf\""));
    EXPECT_FALSE(isValidInput("p Student 10.1"));
    EXPECT_FALSE(isValidInput("p Student -2"));
}

TEST(ActionUsageTest, ValidPInput) {
    EXPECT_TRUE(isValidInput("p Student 10"));
    EXPECT_TRUE(isValidInput("p Student  0"));
}

TEST(ActionUsageTest, InvalidQInput) {
    EXPECT_FALSE(isValidInput("q, Student"));
    EXPECT_FALSE(isValidInput("q Student"));
    EXPECT_FALSE(isValidInput("q"));
    EXPECT_FALSE(isValidInput("q "));
    EXPECT_FALSE(isValidInput("q Student asdf"));
    EXPECT_FALSE(isValidInput("q Student \"asdf\" asdf"));
    EXPECT_FALSE(isValidInput("q Student \"asdf\" \"asdf\" \"asdf\""));
    EXPECT_FALSE(isValidInput("q Student 10 10 10"));
}

TEST(ActionUsageTest, ValidQInput) {
    EXPECT_TRUE(isValidInput("q Student \"asdf\" 10"));
    EXPECT_TRUE(isValidInput("q Student 10 \"asdf\""));
    EXPECT_TRUE(isValidInput("q Student 10"));
    EXPECT_TRUE(isValidInput("q Student 10 10"));
    EXPECT_TRUE(isValidInput("q Student \"asdf\""));
    EXPECT_TRUE(isValidInput("q Student \"asdf\" \"asdf2\""));
}

TEST(ActionUsageTest, InvalidCInput) {
    EXPECT_FALSE(isValidInput("c, Student"));
    EXPECT_FALSE(isValidInput("C Student"));
    EXPECT_FALSE(isValidInput("c"));
    EXPECT_FALSE(isValidInput(" c "));
    EXPECT_FALSE(isValidInput("c Student asdf"));
}

TEST(ActionUsageTest, ValidCInput) {
    EXPECT_TRUE(isValidInput("c Student"));
    EXPECT_TRUE(isValidInput("c  Student"));
}

TEST(ActionUsageTest, getRPayload) {
    auto p1 = getRPayload("R,RPayload, String, 200");
    EXPECT_EQ(p1.name, "RPayload");
    EXPECT_EQ(p1.type, ValueType::String);
    EXPECT_EQ(p1.size, 200);
}

TEST(ActionUsageTest, getIPayload) {
    auto p1 = getIPayload("I, IPayload, 1, \"Record1\"; \"2\", \"Record2\"");
    EXPECT_EQ(p1.name, "IPayload");
    ASSERT_EQ(p1.values.size(), 2);
    EXPECT_EQ(std::get<0>(p1.values[0])->type, ValueType::Integer);
    EXPECT_EQ(std::get<0>(p1.values[0])->IntValue, 1);
    EXPECT_EQ(std::get<1>(p1.values[0]), "Record1");
    EXPECT_EQ(std::get<0>(p1.values[1])->type, ValueType::String);
    EXPECT_STREQ(std::get<0>(p1.values[1])->StrValue, "2");
    EXPECT_EQ(std::get<1>(p1.values[1]), "Record2");

    auto p2 = getIPayload("I, Movie, \"nkpgrtgadybxphreddb\", \"xnrmnw osiyjc spidiliexmlfebsrppsccjtxbmqvrnj\"; \"ksxzilnqvghaamez\", \"ximtkhtuuccevaml\"; \"fbajhdwzzvj\", \"ualawrozurougzmdtlievdzyzqjphqz alntmmgxwoiorrdh\"; \"sbscccjpui\", \"blqrebefmgglqfcrzvmahxur iwn fjwiurfgcjumkr y\"; \"dhvldaieuspxv\", \"pjzybavflhrzonjrsenigwpmwaw\"; \"cpbfdoghexzol\", \"drnkulrxnanzhxoqabmpj qk\"");
}

TEST(ActionUsageTest, getSPayload) {
    auto s1 = getSPayload("Scan Student");
    EXPECT_EQ(s1.name, "Student");
}

TEST(ActionUsageTest, getQPayload) {
    auto q1 = getQPayload("q Student 10");
    EXPECT_EQ(q1.name, "Student");
    ASSERT_EQ(q1.value1->type, ValueType::Integer);
    EXPECT_EQ(q1.value1->IntValue, 10);

    auto q2 = getQPayload("q Student 10 20");
    EXPECT_EQ(q2.name, "Student");
    ASSERT_EQ(q2.value1->type, ValueType::Integer);
    EXPECT_EQ(q2.value1->IntValue, 10);
    ASSERT_EQ(q2.value2->type, ValueType::Integer);
    EXPECT_EQ(q2.value2->IntValue, 20);

    auto q3 = getQPayload("q Student \"10\"");
    EXPECT_EQ(q3.name, "Student");
    ASSERT_EQ(q3.value1->type, ValueType::String);
    EXPECT_STREQ(q3.value1->StrValue, "10");

    auto q4 = getQPayload("q Student \"10\" \"20\"");
    EXPECT_EQ(q4.name, "Student");
    ASSERT_EQ(q4.value1->type, ValueType::String);
    EXPECT_STREQ(q4.value1->StrValue, "10");
    ASSERT_EQ(q4.value2->type, ValueType::String);
    EXPECT_STREQ(q4.value2->StrValue, "20");
}

TEST(ActionUsageTest, getPPayload) {
    auto p1 = getPPayload("p Student 10");
    EXPECT_EQ(p1.name, "Student");
    EXPECT_EQ(p1.pid, 10);
}

TEST(ActionUsageTest, getCPayload) {
    auto c1 = getCPayload("c Student");
    EXPECT_EQ(c1.name, "Student");
}
