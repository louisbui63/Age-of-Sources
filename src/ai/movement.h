//! @file steering_behavior.h
#pragma once

#include "../data_structures/ecs.h"

//! apply the required steering behaviors to move all the units in `w` that have
//! a `SteeringManager` and a `Position`
void move_units(World *w);
