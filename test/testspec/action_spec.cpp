#include <cstring>
#include <vector>
#include <typeinfo>
#include <iostream>
#include "gtest/gtest.h"
#include "action.h"

using namespace std;

TEST(ActionSpecTest, ValueSpec) {
    // Test default values
    Value value;
    EXPECT_EQ(value.type, ValueType::Undefined);
    EXPECT_EQ(value.IntValue, 0);

    Value ivalue(9);
    EXPECT_EQ(ivalue.type, ValueType::Integer);
    EXPECT_EQ(ivalue.IntValue, 9);

    char* str = new char[12];
    strcpy(str, "StringValue");

    Value svalue(str);
    EXPECT_EQ(svalue.type, ValueType::String);
    EXPECT_STREQ(svalue.StrValue, "StringValue");
    delete[] str;
}

TEST(ActionSpecTest, PayloadSpecs) {
    RPayload rpayload = {
        .name = "RPayload"
    };

    // Test default values
    EXPECT_EQ(rpayload.name, "RPayload");
    EXPECT_EQ(rpayload.type, ValueType::Undefined);
    EXPECT_EQ(rpayload.size, 0);


    IPayload ipayload = {
        .name = "IPayload"
    };

    // Test default values
    EXPECT_EQ(ipayload.name, "IPayload");

    DPayload dpayload = {
        .name = "DPayload",
        .value = new Value()
    };

    // Test default values
    EXPECT_EQ(dpayload.name, "DPayload");
    EXPECT_EQ(dpayload.value->type, ValueType::Undefined);
    EXPECT_EQ(dpayload.value->IntValue, 0);

    delete dpayload.value;
}

TEST(ActionSpecTest, ActionSpec) {
    RPayload rpayload = {
        .name = "RPayload"
    };

    IPayload ipayload = {
        .name = "IPayload"
    };

    DPayload dpayload = {
        .name = "DPayload"
    };

    Action actionr(rpayload);
    EXPECT_EQ(actionr.payload.r.name, "RPayload");
    EXPECT_EQ(actionr.type, ActionType::R);

    Action actioni(ipayload);
    EXPECT_EQ(actioni.payload.i.name, "IPayload");
    EXPECT_EQ(actioni.type, ActionType::I);

    Action actiond(dpayload);
    EXPECT_EQ(actiond.payload.d.name, "DPayload");
    EXPECT_EQ(actiond.type, ActionType::D);
}
