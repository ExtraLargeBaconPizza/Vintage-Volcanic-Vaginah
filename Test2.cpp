#include "Test2.h"

void test3()
{
    Serial.println("test3");
}

void Test2()
{
    Serial.println("Test2");
    test3();
}