#include <iostream>
#include <cmath>
// #include <vector>
#include <queue>
#include <cassert>
#include <set>

// #include "dynamicmatrix.h"
// #include "fraction.h"
#include "router.h"

template<typename Type, size_t csize>
void print_matrix(const my::dynamic_matrix<Type, csize>& matrix, decltype(Fraction().numerator()) precesion = 1) {
	assert(precesion != 0);
	for (Fraction i = Fraction{0, precesion}; i < csize; ++i) {
		for (Fraction j = Fraction{0, precesion}; j < csize; ++j) {
			std::cout << matrix.get(i, j) << std::cout.fill();
		}
		std::cout << std::endl;
	}
}

// template<typename TPos, typename TMatrix>
// std::queue<TPos> wave_search(const TMatrix& matrix, 
// 							 const TPos& start,
// 							 const decltype(matrix[TPos()][TPos()])& end_value, 
// 							 const decltype(matrix[TPos()][TPos()])& def_value = {}) {
// 	;
// 	return {};
// }


// // ---------------------------class router-----------------------------------
// struct CBPoint {
// 	uint8_t inv_length; // length = 1 / inv_lenght
// 	enum Direction : int8_t {
// 		UNDEFINED = 0,
// 		UP		= 	1,
// 		RIGHT	=	2,
// 		DOWN	=	-1,
// 		LEFT	=	-2,
// 		WITHOUT =	3
// 	} direction = UNDEFINED;
// };
//
// using CBPosition = std::pair<Fraction, Fraction>;
// // ---------------------------class router-----------------------------------
// static constexpr size_t CBoardSize = 6;
// // ---------------------------class router-----------------------------------
int32_t inv_length = 1;
// // ---------------------------class router-----------------------------------
 uint8_t reduce_coefficient = 2;
// // ---------------------------class router-----------------------------------
// using CBoard = my::dynamic_matrix<CBPoint, CBoardSize>;
 CBoard chips_board;


// ---------------------------class router-----------------------------------
inline CBPosition get_pos(const CBPoint& pt,const CBPosition& pos) {
	switch (pt.direction) {
		case CBPoint::Direction::LEFT:	return {pos.first, pos.second - Fraction{1, pt.inv_length}};
		case CBPoint::Direction::RIGHT:	return {pos.first, pos.second + Fraction{1, pt.inv_length}};
		case CBPoint::Direction::DOWN:	return {pos.first + Fraction{1, pt.inv_length}, pos.second};
		case CBPoint::Direction::UP:	return {pos.first - Fraction{1, pt.inv_length}, pos.second};
		default: return {};
	}
	return {};
}

// ---------------------------class router-----------------------------------
bool horizontal_checking(const CBPosition& pos, const CBoard& board, const CBoard& tmp_board) {
	if (pos.second < Fraction{0, 1} || pos.second >= Fraction{CBoardSize, 1})
		return false;
	if (board.get(pos.first, pos.second).direction != CBPoint::Direction::UNDEFINED)
		return false;
	if (tmp_board.get(pos.first, pos.second).direction != CBPoint::Direction::UNDEFINED)
		return false;
	if (chips_board.get(pos.first, pos.second).direction != CBPoint::Direction::UNDEFINED)
		return false;
	Fraction fraction = pos.first;
	while (fraction.denominator() != 1) {
		fraction = Fraction{fraction.numerator() / reduce_coefficient,
						    fraction.denominator() / reduce_coefficient};
		if (board.is_exist(fraction, pos.second) == false)
                return true;
		const CBPoint& pt = board.get(fraction, pos.second);
		const CBPoint& cpt = chips_board.get(fraction, pos.second);
		if(pt.direction == CBPoint::Direction::DOWN)
			if (pt.inv_length <= fraction.denominator())
				return false;
			// else
				//return true;
		if(cpt.direction == CBPoint::Direction::DOWN)
			if (cpt.inv_length <= fraction.denominator())
				return false;
		fraction.reduce();
	}
	fraction = pos.first;
	while (fraction.denominator() != 1) {
		fraction = Fraction{fraction.numerator() / reduce_coefficient + ((fraction.numerator() % reduce_coefficient) ? 1 : 0) , 
							fraction.denominator() / reduce_coefficient};
		if (board.is_exist(fraction, pos.second) == false)
                return true;
		const CBPoint& pt = board.get(fraction, pos.second);
		const CBPoint& cpt = chips_board.get(fraction, pos.second);
		if (fraction != CBoardSize && pt.direction == CBPoint::Direction::UP)
			if (pt.inv_length <= fraction.denominator())
				return false;
			// else
				//return true;
		if (fraction != CBoardSize && cpt.direction == CBPoint::Direction::UP)
			if (cpt.inv_length <= fraction.denominator())
				return false;
		fraction.reduce();
	}
	return true;
}

// ---------------------------class router-----------------------------------
bool vertical_checking(const CBPosition& pos, const CBoard& board, const CBoard& tmp_board) {
	if (pos.first < Fraction{0, 1} || pos.first >= Fraction{CBoardSize, 1})
		return false;
	if (board.get(pos.first, pos.second).direction != CBPoint::Direction::UNDEFINED)
		return false;
	if (tmp_board.get(pos.first, pos.second).direction != CBPoint::Direction::UNDEFINED)
		return false;
	if (chips_board.get(pos.first, pos.second).direction != CBPoint::Direction::UNDEFINED)
		return false;
	Fraction fraction = pos.second;
	while (fraction.denominator() != 1) {
		fraction = Fraction{fraction.numerator() / reduce_coefficient,
						    fraction.denominator() / reduce_coefficient};
		if (board.is_exist(pos.first, fraction) == false)
				return true;
		const CBPoint& pt = board.get(pos.first, fraction);
		const CBPoint& cpt = chips_board.get(pos.first, fraction);
		if (pt.direction == CBPoint::Direction::RIGHT) 
			if (pt.inv_length <= fraction.denominator())
				return false;
			// else
				//return true;
		if (cpt.direction == CBPoint::Direction::RIGHT) 
			if (cpt.inv_length <= fraction.denominator())
				return false;
		fraction.reduce();
	}
	fraction = pos.second;
	while (fraction.denominator() != 1) {
		fraction = Fraction{fraction.numerator() / reduce_coefficient + ((fraction.numerator() % reduce_coefficient) ? 1 : 0) , 
							fraction.denominator() / reduce_coefficient};
		if (board.is_exist(pos.first, fraction) == false)
				return true;
		const CBPoint& pt = board.get(pos.first, fraction);
		const CBPoint& cpt = chips_board.get(pos.first, fraction);
		if (fraction != CBoardSize && pt.direction == CBPoint::Direction::LEFT) 
			if (pt.inv_length <= fraction.denominator())
				return false;
			// else
				//return true;
		if (fraction != CBoardSize && cpt.direction == CBPoint::Direction::LEFT) 
			if (cpt.inv_length <= fraction.denominator())
				return false;
		fraction.reduce();
	}
	return true;
}

std::ostream& operator<< (std::ostream& out, const CBPoint& p) {
	int value = static_cast<int>(p.direction);
	switch (value) {
		case CBPoint::Direction::UP:	out << "\x1b[31m ^\x1b[m"; break;
		case CBPoint::Direction::DOWN:	out << "\x1b[31m v\x1b[m"; break;
		case CBPoint::Direction::LEFT:	out << "\x1b[31m <\x1b[m"; break;
		case CBPoint::Direction::RIGHT:	out << "\x1b[31m >\x1b[m"; break;
		case CBPoint::Direction::WITHOUT:	out << " S"; break;
		default: out << " .";
	}
	return out;
}

inline bool operator==(const CBPosition& p1, const CBPosition& p2) {
    return p1.first == p2.first && p1.second == p2.second;
}

// ---------------------------class router-----------------------------------
bool wave_router(CBoard& board, 
				 const CBPosition& start, 
				 const CBPosition& end) {
	bool is_successful = false;
	CBoard tmp_board;
    if (board.get(start.first, start.second).direction != CBPoint::Direction::WITHOUT)
        return false;
	tmp_board.put(start.first, start.second, {1, CBPoint::Direction::WITHOUT});
	std::queue<CBPosition> qu;

	qu.push(start);
	while (qu.empty() == false) {
		CBPosition current_pos = qu.front();
        current_pos.first.reduce();
        current_pos.second.reduce();

        if (current_pos.first == end.first && current_pos.second == end.second) {
            is_successful = true;
            break;
        }
		{
			// {navigator}
			// tmp_board.put(current_pos.first, current_pos.second, Observation::Yes);
			CBPosition ps = {current_pos.first + Fraction{1, inv_length}, current_pos.second};
            if (ps == end || vertical_checking(ps, board, tmp_board)) {
				tmp_board.put(ps.first, ps.second, CBPoint{inv_length, CBPoint::Direction::UP});
				qu.push(ps);
			}

			ps = {current_pos.first, current_pos.second + Fraction{1, inv_length}};
            if (ps == end || horizontal_checking(ps, board, tmp_board)) {
				tmp_board.put(ps.first, ps.second, CBPoint{inv_length, CBPoint::Direction::LEFT});
				qu.push(ps);
			}

            ps = {current_pos.first - Fraction{1, inv_length}, current_pos.second};
            if (ps == end || vertical_checking(ps, board, tmp_board)) {
				tmp_board.put(ps.first, ps.second, CBPoint{inv_length, CBPoint::Direction::DOWN});
				qu.push(ps);
			}

			ps = {current_pos.first, current_pos.second - Fraction{1, inv_length}};
            if (ps == end || horizontal_checking(ps, board, tmp_board)) {
				tmp_board.put(ps.first, ps.second, CBPoint{inv_length, CBPoint::Direction::RIGHT});
				qu.push(ps);
			}
		}
     //print_matrix(tmp_board, inv_length);
		qu.pop();
	}
	if (is_successful == true) {
        CBPosition pos = qu.front();
		while (pos.first != start.first || pos.second != start.second) {
			const CBPoint& pt = tmp_board.get(pos.first, pos.second);
			board.put(pos.first, pos.second, pt);
			pos = get_pos(pt, pos);
		}
		board.put(pos.first, pos.second, CBPoint{1, CBPoint::Direction::WITHOUT}); // start point
	}
	// print_matrix(tmp_board, inv_length);
	// std::cout << std::endl;
	//print_matrix(board, 4);
	//std::cout << std::endl;
	return is_successful;
}

// ---------------------------class router-----------------------------------
void wave_router(CBoard& board, std::vector<route> routes) {
	// if grpah is not planar 
	// 			return nothing or throw exception

    for (auto r : routes) {
        board.put(r.first.first, r.first.second, {1, CBPoint::Direction::WITHOUT});
        board.put(r.second.first, r.second.second, {1, CBPoint::Direction::WITHOUT});
        // inv_length = std::min(inv_length, )
    }

	print_matrix(chips_board, inv_length);
	std::vector<route> not_routed;
    for (/*inv_length*/; routes.size() > 0 && inv_length < pow(reduce_coefficient, 6); inv_length *= reduce_coefficient) {
		for (auto r : routes)
			if (wave_router(board, r.first, r.second) == false)
				not_routed.push_back(r);

		routes.clear();
		std::swap(routes, not_routed);
	}
	inv_length = inv_length != 1 ? inv_length / reduce_coefficient : 1;
	// print_matrix(board, inv_length);
    for (auto r : routes)
        ::not_routed.push_back(r);
    std::cout << std::endl;
}

void p_wave_router(CBoard& board) {
    auto& routes = ::not_routed;

    for (auto r : routes) {
        board.put(r.first.first, r.first.second, {1, CBPoint::Direction::WITHOUT});
        board.put(r.second.first, r.second.second, {1, CBPoint::Direction::WITHOUT});
        // inv_length = std::min(inv_length, )
    }

    print_matrix(chips_board, inv_length);
    std::vector<route> not_routed;
    for (/*inv_length*/; routes.size() > 0 && inv_length < pow(reduce_coefficient, 6); inv_length *= reduce_coefficient) {
        for (auto r : routes)
            if (wave_router(board, r.first, r.second) == false)
                not_routed.push_back(r);

        routes.clear();
        std::swap(routes, not_routed);
    }
    inv_length = inv_length != 1 ? inv_length / reduce_coefficient : 1;
    // print_matrix(board, inv_length);
    std::cout << std::endl;
}


// ---------------------------class router-----------------------------------
bool next_pos_b(const CBoard& board,const CBoard& tmp_board, CBPoint::Direction& direction, CBPosition& pos) {
//	->up->left->down->right->
	switch(direction) {
		case CBPoint::Direction::UP:
			direction = CBPoint::Direction::DOWN;	
		break;
		case CBPoint::Direction::LEFT:	 
			direction = CBPoint::Direction::RIGHT;	
		break;
		case CBPoint::Direction::DOWN:	 
			direction = CBPoint::Direction::UP;	
		break;
		case CBPoint::Direction::RIGHT:	 
			direction = CBPoint::Direction::LEFT;	
		break;
	}
	CBPoint::Direction st = direction;

	do {
		switch(direction) {
			case CBPoint::Direction::UP:
			{	
				CBPosition ps = {pos.first - Fraction{1, inv_length}, pos.second};
				if (vertical_checking(ps, board, tmp_board)) {
					pos = ps;
					return true;
				}
				direction = CBPoint::Direction::LEFT;	
			}
			break;
			case CBPoint::Direction::LEFT:	 
			{
				CBPosition ps = {pos.first, pos.second - Fraction{1, inv_length}};
	            if (horizontal_checking(ps, board, tmp_board)) {
					pos = ps;
					return true;
				}
				direction = CBPoint::Direction::DOWN;	
			}
			break;
			case CBPoint::Direction::DOWN:	 
			{
				CBPosition ps = {pos.first + Fraction{1, inv_length}, pos.second};
	            if (vertical_checking(ps, board, tmp_board)) {
					pos = ps;
					return true;
				}
				direction = CBPoint::Direction::RIGHT;	
			}
			break;
			case CBPoint::Direction::RIGHT:	 
			{
				CBPosition ps = {pos.first, pos.second + Fraction{1, inv_length}};
	            if (horizontal_checking(ps, board, tmp_board)) {
					pos = ps;
					return true;
				}
				direction = CBPoint::Direction::UP;	
			}
			break;
		}
	} while (st != direction);

	return false;
}

CBPoint::Direction around(const CBPosition& p1,const CBPosition& p2) {
	if (p1.second == p2.second)
		if (p1.first + Fraction(1, inv_length) == p2.first)
			return CBPoint::Direction::DOWN;
		else if (p1.first - Fraction(1, inv_length) == p2.first) 
			return CBPoint::Direction::UP;


	if (p1.first == p2.first)
		if (p1.second + Fraction(1, inv_length) == p2.second)
			return CBPoint::Direction::RIGHT;
		else if  (p1.second - Fraction(1, inv_length) == p2.second)
			return CBPoint::Direction::LEFT;
	return CBPoint::UNDEFINED;
}

CBPoint::Direction inv_dir(const CBPoint::Direction& d) {
	if (d == CBPoint::WITHOUT)
		return d;
	return (CBPoint::Direction)(-(int)d);
}

// ---------------------------class router-----------------------------------
bool back_edge_router(CBoard& board, 
					  const CBPosition& start, 
					  const CBPosition& end,
					  CBPoint::Direction dir) {
	bool is_successful = false;
	CBoard tmp_board;

	tmp_board.put(start.first, start.second, {inv_length, CBPoint::Direction::WITHOUT});
	// tmp_board.put(end.first,   end.second,	 {inv_length, CBPoint::Direction::WITHOUT});

	CBPosition current_pos = start;

	while (true) {
        // current_pos.first.reduce();
        // current_pos.second.reduce();
		
        if (around(current_pos, end) != CBPoint::Direction::UNDEFINED) {
			// tmp_board.put(current_pos.first, current_pos.second, {inv_length, inv_dir(around(current_pos, end))});
			tmp_board.put(end.first, end.second, {inv_length, inv_dir(around(current_pos, end))});
            is_successful = true;
            break;
        }
		if(next_pos_b(board,tmp_board,dir,current_pos) == false) {
			std::cout << "end" << std::endl;
			break;
		}
		tmp_board.put(current_pos.first, current_pos.second, {inv_length, inv_dir(dir)});
	}
//    print_matrix(tmp_board, inv_length);
//	std::cout << std::endl;

	if (is_successful == true) {
		current_pos = end;
		while (current_pos.first != start.first || current_pos.second != start.second) {
			const CBPoint& pt = tmp_board.get(current_pos.first, current_pos.second);
			board.put(current_pos.first, current_pos.second, pt);
			current_pos = get_pos(pt, current_pos);
		}
		board.put(current_pos.first, current_pos.second, CBPoint{1, CBPoint::Direction::WITHOUT}); // start point
	} else {
		board.put(end.first, end.second, CBPoint{1, CBPoint::Direction::WITHOUT}); // start point
	}
	return is_successful;
}
std::vector<route> not_routed;
// ---------------------------class router-----------------------------------
void be_router(CBoard& board, const std::vector<std::pair<route, CBPoint::Direction>>& routes) {
    // inv_length *= reduce_coefficient * reduce_coefficient * reduce_coefficient;
    // for(auto& r : rd) {
    // 	back_edge_router(board, r.first.first, r.first.second, r.second);
    // }
   //////////////////////////////////////////////////////////////

    std::vector<std::pair<route, CBPoint::Direction>> not_routed;
    auto rd = routes;
    for (; rd.size() > 0 && inv_length < pow(reduce_coefficient, 8); inv_length *= reduce_coefficient) {
        for (auto r : rd)
            if (back_edge_router(board, r.first.first, r.first.second, r.second) == false)
                not_routed.push_back(r);

        rd.clear();
        std::swap(rd, not_routed);
    }
    for (auto& r : rd)
        ::not_routed.push_back(r.first);

    inv_length = inv_length != 1 ? inv_length / reduce_coefficient : 1;
}
