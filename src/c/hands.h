#pragma once

#include "pebble.h"



static const GPathInfo MINUTE_HAND_POINTS = {
	7,
  (GPoint []) {
    { -1, 17 },
    { -1, -60 },
    { -4, -60 },
    { 0, -70 },
    { 4, -60 },
    { 1, -60 },
	{ 1, 17 }
  }
};

static const GPathInfo HOUR_HAND_POINTS = {
7,
  (GPoint []) {
    { -1, 14 },
    { -1, -35 },
    { -4, -35 },
    { 0, -45 },
	{ 4, -35 },
	{ 1, -35 },
	{ 1, 14 }
  }
};

