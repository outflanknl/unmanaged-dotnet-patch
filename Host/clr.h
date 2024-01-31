#pragma once

#include <metahost.h>

#import <mscorlib.tlb> raw_interfaces_only						\
		high_property_prefixes("_get","_put","_putref")			\
		rename("ReportEvent", "InteropServices_ReportEvent")	\
		rename("or", "InteropServices_or")
using namespace mscorlib;

#pragma comment(lib, "mscoree.lib")
