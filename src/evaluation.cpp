#include "stdafx.h"
#include "evaluation.h"

#include <cassert>
// ##include <pthread.h>
#include <thread>
#include <map>

using namespace std;

Evaluation::Evaluation() :
    nbUsables(0), avgAccess(0.0)
{

}

Evaluation::~Evaluation()
{
}
