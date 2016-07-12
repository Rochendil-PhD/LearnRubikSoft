#ifndef _RUBICK_ALGORITHMS_HPP_
#define _RUBICK_ALGORITHMS_HPP_

#include <iostream>
#include <queue>

#include "Common.hpp"

class RubikAlgorithms
{
public:
	enum faceMove { mFront, mBack, mLeft, mRight, mTop, mBottom, mGreen, mBlue, mOrange, mRed, mBreak, mError };
	enum hintMessages { hintYellowCross, hintYellowCorners, hintOuterEdges, hintWhiteCross, hintWhiteUp, hintEdgeRotation, hintCornerRotation};
private:
    static cubicleColor MoveToColor(faceMove, cubicleColor);
	static std::queue<std::pair<faceMove, short>> movesQueue;
	static std::queue<std::string> logQueue;

	static void findNextMove();
	static void doNextMove();

	static void RotateTopAlgorithm(short, cubicleColor);
	static void BotEdgeAlgorithm(short);
	static void BotCornerAlgorithm(short);
	static void OuterEdgeAlgorithm(short);
	static void TopUpEdgeAlgorithm(short);
	static void TopUpCornerAlgorithm(std::vector<short>);
	static short TopRotateEdgeAlgorithm(short, bool);
	static void TopRotateCornerAlgorithm(bool);
	static short TopLookAround(short, short);

	static void getHint(hintMessages);
	static bool hintActivated;

	static void rotateFace(faceMove, short, cubicleColor);
	static cubicleColor forceFace;
	static bool freezeKeys;
public:
	static bool isFrozen();
	static void freeze(bool);
	static void toggleHints(bool);

	static void rotateFace(faceMove, short);
	static void solveRubik();
};
#endif // _RUBICK_ALGORITHMS_HPP_
