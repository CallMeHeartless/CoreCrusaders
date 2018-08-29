#ifndef __LEVELINFO_H__
#define __LEVELINFO_H__

#include <vector>

namespace LEVEL_INFO {
	static std::vector<std::vector<unsigned int>> SPAWNS = {
	// Drone, tank, sprinter, Hunters
	{10, 0, 0, 0},
	{15, 0, 0, 0},
	{10, 1, 0, 0},
	{15, 0, 1, 0},
	{15, 1, 1, 0},
	{15, 0, 0, 2},
	{20, 2, 2, 2},
	{25, 0, 5, 0},
	{25, 5, 0, 0},
	{25, 0, 0, 5},
	{10, 5, 5, 5},
	{30, 3, 2, 2},

	};
}


#endif // !__LEVELINFO_H__
