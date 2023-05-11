#ifndef ROUTER_H
#define ROUTER_H

#include <vector>
#include "fraction.h"
#include "dynamicmatrix.h"

struct CBPoint {
    int32_t inv_length; // length = 1 / inv_lenght
    enum Direction : int8_t {
        UNDEFINED = 0,
        UP		= 	1,
        RIGHT	=	2,
        DOWN	=	-1,
        LEFT	=	-2,
        WITHOUT =	3
    } direction = UNDEFINED;
};

using CBPosition = std::pair<Fraction, Fraction>;
// ---------------------------class router-----------------------------------
static constexpr size_t CBoardSize = 10;
// // ---------------------------class router-----------------------------------
int32_t extern inv_length;
// ---------------------------class router-----------------------------------
uint8_t extern reduce_coefficient;
// ---------------------------class router-----------------------------------
using CBoard = my::dynamic_matrix<CBPoint, CBoardSize>;

// template<typename Type, size_t csize>
my::dynamic_matrix<CBPoint, CBoardSize> extern chips_board;


// CBoard extern chips_board;

// ---------------------------class router-----------------------------------
using route = std::pair<CBPosition, CBPosition>;

std::vector<route> extern not_routed;

inline CBPosition get_pos(const CBPoint& pt,const CBPosition& pos);
void wave_router(CBoard& board, std::vector<route> routes);
void p_wave_router(CBoard& board);

bool back_edge_router(CBoard& board, 
					  const CBPosition& start, 
					  const CBPosition& end,
					  CBPoint::Direction dir);
void be_router(CBoard& board, const std::vector<std::pair<route, CBPoint::Direction>>&);
#endif // ROUTER_H
