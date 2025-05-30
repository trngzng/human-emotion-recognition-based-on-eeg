#include "inc/filter.h"

#include "filter.h"
#include <cmath>

Filter::Filter(QObject *parent) : QObject(parent)
{
    reset();
}

void Filter::reset()
{
    input.clear();
    output.clear();
}

float Filter::process(float input)
{

}
