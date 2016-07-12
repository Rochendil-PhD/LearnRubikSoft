#include "Algorithms.hpp"
#include "Cube.hpp"

cubicleColor frontalFace = RED;

std::queue<std::pair<RubikAlgorithms::faceMove, short>> RubikAlgorithms::movesQueue;
std::queue<std::string> RubikAlgorithms::logQueue;
cubicleColor RubikAlgorithms::forceFace;
bool RubikAlgorithms::freezeKeys;
bool RubikAlgorithms::hintActivated;

void RubikAlgorithms::freeze(bool x)
{
	freezeKeys = x;
}
bool RubikAlgorithms::isFrozen()
{
	return freezeKeys;
}

void RubikAlgorithms::rotateFace(faceMove move, short modifier)
{
	rotateFace(move, modifier, frontalFace);
}

void RubikAlgorithms::rotateFace(faceMove move, short modifier, cubicleColor face)
{
    if (move < mTop) {
        cubicleColor color = MoveToColor(move, face);
		RubikCube::mainCube.rotateLogic(color, modifier);
		RubikCube::mainCube.rotateGraphic(color, modifier);
    } else if (move == mTop) {
		RubikCube::mainCube.rotateLogic(WHITE, modifier);
		RubikCube::mainCube.rotateGraphic(WHITE, modifier);
    } else if (move == mBottom) {
		RubikCube::mainCube.rotateLogic(YELLOW, modifier);
		RubikCube::mainCube.rotateGraphic(YELLOW, modifier);
	} else {
		fprintf(stderr, "Algorithms::rotateFace says: Something went wrong with the move!");
	}
}

// This function fixes the frontal face after being calculated in Camera::calcFace()
void frontColChange(short isClockwise) // clockwise by axis OY
{
	if (isClockwise)
		switch (frontalFace)
		{
			case RED:       frontalFace = BLUE;     return;
			case BLUE:      frontalFace = ORANGE;   return;
			case ORANGE:    frontalFace = GREEN;    return;
			case GREEN:     frontalFace = RED;      return;
		}
	else
		switch (frontalFace)
		{
			case ORANGE:    frontalFace = BLUE;     return;
			case GREEN:     frontalFace = ORANGE;   return;
			case RED:       frontalFace = GREEN;    return;
			case BLUE:      frontalFace = RED;      return;
		}
	fprintf(stderr, "frontColChange says: frontalFace has an error!\n");
}

cubicleColor RubikAlgorithms::MoveToColor(faceMove move, cubicleColor face) {
	// We read the position of the face, and calculate it into color of the face
    switch (face)
    {
        case RED:
        {
            switch(move)
            {
                case mFront:    return RED;
                case mBack:     return ORANGE;
                case mLeft:     return GREEN;
                case mRight:    return BLUE;
            }
        } break;
        case BLUE:
        {
            switch(move)
            {
                case mFront:    return BLUE;
                case mBack:     return GREEN;
                case mLeft:     return RED;
                case mRight:    return ORANGE;
            }
        } break;
        case ORANGE:
        {
            switch(move)
            {
                case mFront:    return ORANGE;
                case mBack:     return RED;
                case mLeft:     return BLUE;
                case mRight:    return GREEN;
            }
        } break;
        case GREEN:
        {
            switch(move)
            {
                case mFront:    return GREEN;
                case mBack:     return BLUE;
                case mLeft:     return ORANGE;
                case mRight:    return RED;
            }
        } break;
    }
    fprintf(stderr, "Algorithms::MoveToColor says: frontalFace has an error! %d\n", move);
	return NOCOLOR;
}

void RubikAlgorithms::solveRubik()
{
	/**
	*	this is the main solver function. It tests the curent state of the cube.
	*	It stops at first wrong configuration and tries to solve it.
	**/

	if (movesQueue.empty())
	{
		// This means we don't have a previously calculated algorithm that needs to finish.
		// Therefore, we search for next state of the cube that we need to solve.
		findNextMove();

		if (!movesQueue.empty())
			doNextMove(); // Next move it will be the break command
	} else {
		// We do have something in queue, so now we execute moves and name them.
		doNextMove();
	}

}

void RubikAlgorithms::findNextMove()
{
	//check YellowCross
	short flags = 0;
	if (!RubikCube::mainCube.isCubicleOK(1, YELLOW)) // 1 has flag 0001
		flags |= 1;
	if (!RubikCube::mainCube.isCubicleOK(3, YELLOW)) // 3 has flag 0010
		flags |= 2;
	if (!RubikCube::mainCube.isCubicleOK(5, YELLOW)) // 5 has flag 0100
		flags |= 4;
	if (!RubikCube::mainCube.isCubicleOK(7, YELLOW)) // 7 has flag 1000
		flags |= 8;
	if (flags)
	{
		getHint(hintYellowCross);
		if (flags & 1)
		{
			movesQueue.push(std::make_pair(mBreak, 0));
			logQueue.push("YELLOW-ORANGE edge:\n");
			BotEdgeAlgorithm(1);
		}
		else if (flags & 2)
		{
			movesQueue.push(std::make_pair(mBreak, 0));
			logQueue.push("YELLOW-GREEN edge:\n");
			BotEdgeAlgorithm(3);
		}
		else if (flags & 4)
		{
			movesQueue.push(std::make_pair(mBreak, 0));
			logQueue.push("YELLOW-BLUE edge:\n");
			BotEdgeAlgorithm(5);
		}
		else if (flags & 8)
		{
			movesQueue.push(std::make_pair(mBreak, 0));
			logQueue.push("YELLOW-RED edge:\n");
			BotEdgeAlgorithm(7);
		}
		return;
	}
	// Yellow cross Complete!

	// Check Yellow Face
	flags = 0;
	if (!RubikCube::mainCube.isCubicleOK(0, YELLOW)) // 0 has flag 0001
		flags |= 1;
	if (!RubikCube::mainCube.isCubicleOK(2, YELLOW)) // 2 has flag 0010
		flags |= 2;
	if (!RubikCube::mainCube.isCubicleOK(6, YELLOW)) // 6 has flag 0100
		flags |= 4;
	if (!RubikCube::mainCube.isCubicleOK(8, YELLOW)) // 8 has flag 1000
		flags |= 8;
	if (flags)
	{
		getHint(hintYellowCorners);
		if (flags & 1)
		{
			movesQueue.push(std::make_pair(mBreak, 0));
			logQueue.push("YELLOW-ORANGE-GREEN corner:\n");
			BotCornerAlgorithm(0);
		}
		else if (flags & 2)
		{
			movesQueue.push(std::make_pair(mBreak, 0));
			logQueue.push("YELLOW-BLUE-ORANGE corner:\n");
			BotCornerAlgorithm(2);
		}
		else if (flags & 4)
		{
			movesQueue.push(std::make_pair(mBreak, 0));
			logQueue.push("YELLOW-GREEN-RED corner:\n");
			BotCornerAlgorithm(6);
		}
		else if (flags & 8)
		{
			movesQueue.push(std::make_pair(mBreak, 0));
			logQueue.push("YELLOW-RED-BLUE corner:\n");
			BotCornerAlgorithm(8);
		}
		return;
	}
	// Yellow Face Complete!

	// Check Outer Edges
	flags = 0;
	if (!RubikCube::mainCube.isCubicleOK(9, ORANGE)) // 9 has flag 0001
		flags |= 1;
	if (!RubikCube::mainCube.isCubicleOK(11, BLUE)) // 11 has flag 0010
		flags |= 2;
	if (!RubikCube::mainCube.isCubicleOK(15, GREEN)) // 15 has flag 0100
		flags |= 4;
	if (!RubikCube::mainCube.isCubicleOK(17, RED)) // 17 has flag 1000
		flags |= 8;
	if (flags)
	{
		getHint(hintOuterEdges);
		if (flags & 1)
		{
			movesQueue.push(std::make_pair(mBreak, 0));
			logQueue.push("ORANGE-GREEN edge:\n");
			OuterEdgeAlgorithm(9);
		}
		else if (flags & 2)
		{
			movesQueue.push(std::make_pair(mBreak, 0));
			logQueue.push("BLUE-ORANGE edge:\n");
			OuterEdgeAlgorithm(11);
		}
		else if (flags & 4)
		{
			movesQueue.push(std::make_pair(mBreak, 0));
			logQueue.push("GREEN-RED edge:\n");
			OuterEdgeAlgorithm(15);
		}
		else if (flags & 8)
		{
			movesQueue.push(std::make_pair(mBreak, 0));
			logQueue.push("RED-BLUE edge:\n");
			OuterEdgeAlgorithm(17);
		}
		return;
	}
	// Outer Edges Complete!

	std::vector<short> isWhite = RubikCube::mainCube.getWhiteUpPositions();

	// Check White Cross
	short counter = isWhite[1] + isWhite[3] + isWhite[5] + isWhite[7];
	if (counter != 4) // Otherwise it means all the cross is ok
	{
		getHint(hintWhiteCross);
		// flag 01 = L, flag10 = -
		if (!counter)
		{
			flags = 1 | 2;
			movesQueue.push(std::make_pair(mBreak, 0));
			logQueue.push("WHITE cross - Point:\n");
			movesQueue.push(std::make_pair(mRed, 0));
			logQueue.push("Face-Red: ");
			TopUpEdgeAlgorithm(flags);
		}
		else if (counter == 2)
		{
			if (isWhite[1])
			{
				if (isWhite[3])
				{
					flags = 1;
					movesQueue.push(std::make_pair(mBreak, 0));
					logQueue.push("WHITE cross - mirrored-L:\n");
					movesQueue.push(std::make_pair(mRed, 0));
					logQueue.push("Face-Red: ");
					TopUpEdgeAlgorithm(flags);
				}
				else if (isWhite[5])
				{
					flags = 1;
					movesQueue.push(std::make_pair(mBreak, 0));
					logQueue.push("WHITE cross - mirrored-L:\n");
					movesQueue.push(std::make_pair(mRed, 0));
					logQueue.push("Face-Red: ");
					movesQueue.push(std::make_pair(mTop, 3));
					logQueue.push("U' ");
					TopUpEdgeAlgorithm(flags);
				}
				else if (isWhite[7])
				{
					flags = 2;
					movesQueue.push(std::make_pair(mBreak, 0));
					logQueue.push("WHITE cross - line:\n");
					movesQueue.push(std::make_pair(mRed, 0));
					logQueue.push("Face-Red: ");
					movesQueue.push(std::make_pair(mTop, 1));
					logQueue.push("U ");
					TopUpEdgeAlgorithm(flags);
				}
			}
			else if (isWhite[3])
			{
				if (isWhite[5])
				{
					flags = 2;
					movesQueue.push(std::make_pair(mBreak, 0));
					logQueue.push("WHITE cross - line:\n");
					movesQueue.push(std::make_pair(mRed, 0));
					logQueue.push("Face-Red: ");
					TopUpEdgeAlgorithm(flags);
				}
				else if (isWhite[7])
				{
					flags = 1;
					movesQueue.push(std::make_pair(mBreak, 0));
					logQueue.push("WHITE cross - mirrored-L:\n");
					movesQueue.push(std::make_pair(mRed, 0));
					logQueue.push("Face-Red: ");
					movesQueue.push(std::make_pair(mTop, 1));
					logQueue.push("U ");
					TopUpEdgeAlgorithm(flags);
				}
			}
			else if (isWhite[5])
			{
				if (isWhite[7])
				{
					flags = 1;
					movesQueue.push(std::make_pair(mBreak, 0));
					logQueue.push("WHITE cross - mirrored-L:\n");
					movesQueue.push(std::make_pair(mRed, 0));
					logQueue.push("Face-Red: ");
					movesQueue.push(std::make_pair(mTop, 2));
					logQueue.push("2U ");
					TopUpEdgeAlgorithm(flags);
				}
			}
		} else {
				fprintf(stderr, "The cube is BUGGED!\n"
					"One of your edge-cubicles has been forcefully rotated!\n"
					);
		}
		return;
	}
	// White Cross Complete!

	// Check White Faced-Up
	counter = isWhite[0] + isWhite[2] + isWhite[6] + isWhite[8];
	if (counter != 4) // Otherwise it means all the corners have White on the top face
	{
		getHint(hintWhiteUp);
		std::vector<short> noRotations = RubikCube::mainCube.getRotationsNeededForWhite();
		if (!((noRotations[0] + noRotations[1] + noRotations[2] + noRotations[3]) % 3))
		{
			fprintf(stderr, "The cube is BUGGED!\n"
				"One of your corner-cubicles has been forcefully rotated!\n"
				);
		}

		movesQueue.push(std::make_pair(mBreak, 0));
		logQueue.push("Always keep the same face towards you when rotating corners!\n");
		movesQueue.push(std::make_pair(mRed, 0));
		logQueue.push("Face-Red: ");
		TopUpCornerAlgorithm(noRotations);
		return;
	}
	// White Faced-up!

	std::vector<short> whiteIDs = RubikCube::mainCube.getWhiteCubiclesPositions();

	// Check White Edges
	switch (whiteIDs[7])
	{
	case 1:
		flags = 2;
		break;
	case 3:
		flags = 3;
		break;
	case 5:
		flags = 1;
		break;
	default:
		flags = 0;
	}
	if (TopLookAround(whiteIDs[1], 1) == whiteIDs[TopLookAround(1, 1)]) // Blue is on the left of Orange. Correct
	{
		if (TopLookAround(whiteIDs[1], 0) == whiteIDs[TopLookAround(1, -1)]) // Green is across of Orange
		{
			movesQueue.push(std::make_pair(mBreak, 0));
			logQueue.push("Rearange the White edges!\n");
			movesQueue.push(std::make_pair(mRed, 0));
			logQueue.push("Face-Red: ");
			flags += TopRotateEdgeAlgorithm(whiteIDs[3], true);
			counter = 0;
		}
	}
	else if (TopLookAround(whiteIDs[1], 0) == whiteIDs[TopLookAround(1, 1)])
	{
		// Blue is across of Orange. Go figure out the direction we need to spin
		if (TopLookAround(whiteIDs[1], -1) == whiteIDs[TopLookAround(1, -1)])
		{
			// Green is on the right of Orange
			// Red is on the left of Orange
			movesQueue.push(std::make_pair(mBreak, 0));
			logQueue.push("Rearange the White edges!\n");
			movesQueue.push(std::make_pair(mRed, 0));
			logQueue.push("Face-Red: ");
			flags += TopRotateEdgeAlgorithm(whiteIDs[3], false);
			counter = 0;
		}
		else
		{
			// Red is on the right of Orange
			// Green is on the left of Orange
			movesQueue.push(std::make_pair(mBreak, 0));
			logQueue.push("Rearange the White edges!\n");
			movesQueue.push(std::make_pair(mRed, 0));
			logQueue.push("Face-Red: ");
			flags += TopRotateEdgeAlgorithm(whiteIDs[5], false);
			counter = 0;
		}
	}
	else //if (TopLookAround(whiteIDs[1], -1) == whiteIDs[TopLookAround(1, 1)])
	{
		// Blue is on the right of Orange. Go figure out the direction we need to spin
		if (TopLookAround(whiteIDs[1], 0) == whiteIDs[TopLookAround(1, 0)])
		{
			// Red is across of Orange
			// Green is on the left of Orange
			movesQueue.push(std::make_pair(mBreak, 0));
			logQueue.push("Rearange the White edges!\n");
			movesQueue.push(std::make_pair(mRed, 0));
			logQueue.push("Face-Red: ");
			flags += TopRotateEdgeAlgorithm(1, true);
			flags += TopRotateEdgeAlgorithm(5, true);
			counter = 0;
		}
		else
		{
			// Green is across of Orange
			// Red is on the left of Orange
			movesQueue.push(std::make_pair(mBreak, 0));
			logQueue.push("Rearange the White edges!\n");
			movesQueue.push(std::make_pair(mRed, 0));
			logQueue.push("Face-Red: ");
			flags += TopRotateEdgeAlgorithm(whiteIDs[3], true);
			counter = 0;
		}
	}

	if (flags)
	{
		switch (flags)
		{
		case 3:
			movesQueue.push(std::make_pair(mTop, 3));
			logQueue.push("U' ");
			break;
		case 2:
			movesQueue.push(std::make_pair(mTop, 2));
			logQueue.push("2U ");
			break;
		case 1:
			movesQueue.push(std::make_pair(mTop, 1));
			logQueue.push("U ");
			break;
		}
		counter = 0;
	}

	if (!counter)
	{
		getHint(hintEdgeRotation);
		movesQueue.push(std::make_pair(mBreak, 0));
		logQueue.push("\n");
		return;
	}
	// White edges correct!


	// Check White Corners Position
	flags = 0;
	if (TopLookAround(8, 0) == whiteIDs[8])
	{
		flags = 1;
		movesQueue.push(std::make_pair(mBreak, 0));
		logQueue.push("Rearange the White corners!\n");
		movesQueue.push(std::make_pair(mRed, 0));
		logQueue.push("Face-Red: ");
		TopRotateCornerAlgorithm(false);

		if (TopLookAround(2, 0) == whiteIDs[2])
		{
			movesQueue.push(std::make_pair(mBlue, 0));
			logQueue.push("Face-Blue: ");
			TopRotateCornerAlgorithm(false);
		}
		else if (TopLookAround(2, -1) == whiteIDs[2])
		{
			movesQueue.push(std::make_pair(mBlue, 0));
			logQueue.push("Face-Blue: ");
			TopRotateCornerAlgorithm(true);
		}
	}
	else if (TopLookAround(8, -1) == whiteIDs[8])
	{
		flags = 1;
		movesQueue.push(std::make_pair(mBreak, 0));
		logQueue.push("Rearange the White corners!\n");
		movesQueue.push(std::make_pair(mRed, 0));
		logQueue.push("Face-Red: ");
		TopRotateCornerAlgorithm(true);

		if (TopLookAround(2, 0) == whiteIDs[2])
		{
			movesQueue.push(std::make_pair(mBlue, 0));
			logQueue.push("Face-Blue: ");
			TopRotateCornerAlgorithm(false);
		}
		else if (TopLookAround(2, -1) == whiteIDs[2])
		{
			movesQueue.push(std::make_pair(mBlue, 0));
			logQueue.push("Face-Blue: ");
			TopRotateCornerAlgorithm(true);
		}
	}
	else if (TopLookAround(8, 1) == whiteIDs[8])
	{
		flags = 1;
		movesQueue.push(std::make_pair(mBreak, 0));
		logQueue.push("Rearange the White corners!\n");
		if (TopLookAround(0, 0) == whiteIDs[0])
		{
			movesQueue.push(std::make_pair(mRed, 0));
			logQueue.push("Face-Red: ");
			TopRotateCornerAlgorithm(true);
		}
		else if (TopLookAround(0, 1) == whiteIDs[0])
		{
			logQueue.push("Face-Red: ");
			TopRotateCornerAlgorithm(false);
		}

		movesQueue.push(std::make_pair(mGreen, 0));
		logQueue.push("Face-Green: ");
		TopRotateCornerAlgorithm(false);
	} else {
		if (TopLookAround(2, 0) == whiteIDs[2])
		{
			flags = 1;
			movesQueue.push(std::make_pair(mBreak, 0));
			logQueue.push("Rearange the White corners!\n");
			movesQueue.push(std::make_pair(mBlue, 0));
			logQueue.push("Face-Blue: ");
			TopRotateCornerAlgorithm(false);
		}
		else if (TopLookAround(2, -1) == whiteIDs[2])
		{
			flags = 1;
			movesQueue.push(std::make_pair(mBreak, 0));
			logQueue.push("Rearange the White corners!\n");
			movesQueue.push(std::make_pair(mBlue, 0));
			logQueue.push("Face-Blue: ");
			TopRotateCornerAlgorithm(true);
		}
	}

	if (flags)
	{
		getHint(hintCornerRotation);
		movesQueue.push(std::make_pair(mBreak, 0));
		logQueue.push("\nCongratulation! The cube is solved!\n");
		return;
	}

	// Cube Done!
	addHintsToScreen("The cube is already solved!\n");
	freeze(false);
	return;
}

void RubikAlgorithms::BotEdgeAlgorithm(short ID) // Yellow Cross Function
{
	int rotationCode = 4; // Green = 4, Red = 5, Blue = 6, Orange = 7
	// This is the function that takes care of bottom edges
	glm::vec3 pos = RubikCube::mainCube.getCubiclePosition(ID);
	if (pos.y == -1) {
		// Cubicle is on the bottom side of the cube, but on wrong position
		if (pos.x == -1) // onGreen
		{
			movesQueue.push(std::make_pair(mGreen, 0));
			logQueue.push("Face-Green: ");
			rotationCode = 4;
		}
		else if (pos.x == 1) // onBlue
		{
			movesQueue.push(std::make_pair(mBlue, 0));
			logQueue.push("Face-Blue: ");
			rotationCode = 6;
		}
		else if (pos.z == -1) // onOrange
		{
			movesQueue.push(std::make_pair(mOrange, 0));
			logQueue.push("Face-Orange: ");
			rotationCode = 7;
		}
		else if (pos.z == 1) // onRed
		{
			movesQueue.push(std::make_pair(mRed, 0));
			logQueue.push("Face-Red: ");
			rotationCode = 5;
		}

		if (RubikCube::mainCube.isCubicleRotated(pos, YELLOW))
		{
			movesQueue.push(std::make_pair(mFront, 1));
			logQueue.push("(F ");
			movesQueue.push(std::make_pair(mLeft, 3));
			logQueue.push("L' ");
			movesQueue.push(std::make_pair(mTop, 3));
			logQueue.push("U' ");
			movesQueue.push(std::make_pair(mLeft, 1));
			logQueue.push("L)\nThe cubicle is now on the top!\n");
		} else {
			movesQueue.push(std::make_pair(mFront, 2));
			logQueue.push("2F\nThe cubicle is now on the top!\n");

		}
	} else if (pos.y == 0) {
		// Cubicle is on the outer edges of the cube
		if (pos.x == -1) // onGreen
		{
			movesQueue.push(std::make_pair(mGreen, 0));
			logQueue.push("Face-Green: ");
			rotationCode = 4;
			if (pos.z == -1) // andOrange
			{
				if (YELLOW == RubikCube::mainCube.getCubicleColorByPosition(pos, GREEN))
				{
					movesQueue.push(std::make_pair(mLeft, 3));
					logQueue.push("(L' ");
					movesQueue.push(std::make_pair(mTop, 3));
					logQueue.push("U' ");
					movesQueue.push(std::make_pair(mLeft, 1));
					logQueue.push("L)\nThe cubicle is now on the top!\n");
				} else {
					movesQueue.push(std::make_pair(mFront, 1));
					logQueue.push("(F ");
					movesQueue.push(std::make_pair(mTop, 3));
					logQueue.push("U' ");
					movesQueue.push(std::make_pair(mFront, 3));
					logQueue.push("F' ");
					movesQueue.push(std::make_pair(mTop, 1));
					logQueue.push("U)\nThe cubicle is now on the top!\n");
				}
			}
			else if (pos.z == 1) // andRed
			{
				if (YELLOW == RubikCube::mainCube.getCubicleColorByPosition(pos, GREEN))
				{
					movesQueue.push(std::make_pair(mRight, 1));
					logQueue.push("(R ");
					movesQueue.push(std::make_pair(mTop, 1));
					logQueue.push("U ");
					movesQueue.push(std::make_pair(mRight, 3));
					logQueue.push("R')\nThe cubicle is now on the top!\n");
				} else {
					movesQueue.push(std::make_pair(mFront, 3));
					logQueue.push("(F' ");
					movesQueue.push(std::make_pair(mTop, 3));
					logQueue.push("U' ");
					movesQueue.push(std::make_pair(mFront, 1));
					logQueue.push("F ");
					movesQueue.push(std::make_pair(mTop, 1));
					logQueue.push("U)\nThe cubicle is now on the top!\n");
				}
			}
		}
		else if (pos.x == 1) // onBlue
		{
			movesQueue.push(std::make_pair(mBlue, 0));
			logQueue.push("Face-Blue: ");
			rotationCode = 6;
			if (pos.z == 1) // andRed
			{
				if (YELLOW == RubikCube::mainCube.getCubicleColorByPosition(pos, BLUE))
				{
					movesQueue.push(std::make_pair(mLeft, 3));
					logQueue.push("(L' ");
					movesQueue.push(std::make_pair(mTop, 3));
					logQueue.push("U' ");
					movesQueue.push(std::make_pair(mLeft, 1));
					logQueue.push("L)\nThe cubicle is now on the top!\n");
				} else {
					movesQueue.push(std::make_pair(mFront, 1));
					logQueue.push("(F ");
					movesQueue.push(std::make_pair(mTop, 3));
					logQueue.push("U' ");
					movesQueue.push(std::make_pair(mFront, 3));
					logQueue.push("F' ");
					movesQueue.push(std::make_pair(mTop, 1));
					logQueue.push("U)\nThe cubicle is now on the top!\n");
				}
			}
			else if (pos.z == -1)// andOrange
			{
				if (YELLOW == RubikCube::mainCube.getCubicleColorByPosition(pos, BLUE))
				{
					movesQueue.push(std::make_pair(mRight, 1));
					logQueue.push("(R ");
					movesQueue.push(std::make_pair(mTop, 1));
					logQueue.push("U ");
					movesQueue.push(std::make_pair(mRight, 3));
					logQueue.push("R') ");
				} else {
					movesQueue.push(std::make_pair(mFront, 3));
					logQueue.push("(F' ");
					movesQueue.push(std::make_pair(mTop, 3));
					logQueue.push("U' ");
					movesQueue.push(std::make_pair(mFront, 1));
					logQueue.push("F ");
					movesQueue.push(std::make_pair(mTop, 1));
					logQueue.push("U)\nThe cubicle is now on the top!\n");
				}
			}
		}
	} else if (pos.y == 1 && YELLOW != RubikCube::mainCube.getCubicleColorByPosition(pos, WHITE)) {
		// Cubicle is on the top side of the cube, and yellow is not up
		if (pos.x == -1) // onGreen
		{
			movesQueue.push(std::make_pair(mGreen, 0));
			logQueue.push("Face-Green: ");
			rotationCode = 4;
		}
		else if (pos.x == 1) // onBlue
		{
			movesQueue.push(std::make_pair(mBlue, 0));
			logQueue.push("Face-Blue: ");
			rotationCode = 6;
		}
		else if (pos.z == -1) // onOrange
		{
			movesQueue.push(std::make_pair(mOrange, 0));
			logQueue.push("Face-Orange: ");
			rotationCode = 7;
		}
		else if (pos.z == 1) // onRed
		{
			movesQueue.push(std::make_pair(mRed, 0));
			logQueue.push("Face-Red: ");
			rotationCode = 5;
		}
		movesQueue.push(std::make_pair(mFront, 1));
		logQueue.push("(F ");
		movesQueue.push(std::make_pair(mRight, 1));
		logQueue.push("R ");
		movesQueue.push(std::make_pair(mTop, 3));
		logQueue.push("U' ");
		movesQueue.push(std::make_pair(mRight, 3));
		logQueue.push("R' ");
		movesQueue.push(std::make_pair(mFront, 3));
		logQueue.push("F' ");
		movesQueue.push(std::make_pair(mTop, 2));
		logQueue.push("2U)\nThe cubicle is now on the top!\n");
	} else {
		// This is the easiest case, Yellow up, on top face. Just need to calculate how much to rotate Top

		if (pos.x == -1) // onGreen
		{
			rotationCode = 4;
		}
		else if (pos.x == 1) // onBlue
		{
			rotationCode = 6;
		}
		else if (pos.z == -1) // onOrange
		{
			rotationCode = 7;
		}
		else if (pos.z == 1) // onRed
		{
			rotationCode = 5;
		}
	}

	cubicleColor frontSide = NOCOLOR;
	switch (ID) // Need to make sure we get to the right face to place the cubicle
	{
		case 1:
			frontSide = ORANGE;
			break;
		case 3:
			frontSide = GREEN;
			break;
		case 5:
			frontSide = BLUE;
			break;
		case 7:
			frontSide = RED;
			break;
	}
	RotateTopAlgorithm(rotationCode, frontSide);
	movesQueue.push(std::make_pair(mFront, 2));
	logQueue.push("2F \n");
	movesQueue.push(std::make_pair(mBreak, 0));
	logQueue.push("\n");
}

void RubikAlgorithms::BotCornerAlgorithm(short ID) // Yellow Corners
{
	glm::vec3 pos = RubikCube::mainCube.getCubiclePosition(ID);
	int rotationCode = 4; // Green = 4, Red = 5, Blue = 6, Orange = 7
	cubicleColor faceToCheckColor = NOCOLOR;
	if (pos.y == -1) {
		// Cubicle is on the bottom side of the cube, so we need to get it up
		if (pos.x == -1) // onGreen
		{
			faceToCheckColor = GREEN;
			movesQueue.push(std::make_pair(mGreen, 0));
			logQueue.push("Face-Green: ");
			rotationCode = 4;
			if (pos.z == -1) // andOrange
			{
				movesQueue.push(std::make_pair(mLeft, 3));
				logQueue.push("(L' ");
				movesQueue.push(std::make_pair(mTop, 3));
				logQueue.push("U' ");
				movesQueue.push(std::make_pair(mLeft, 1));
				logQueue.push("L)\nThe cubicle is now on the top!\n");
			}
			else if (pos.z == 1) // andRed
			{
				movesQueue.push(std::make_pair(mRight, 1));
				logQueue.push("(R ");
				movesQueue.push(std::make_pair(mTop, 1));
				logQueue.push("U ");
				movesQueue.push(std::make_pair(mRight, 3));
				logQueue.push("R' ");
				movesQueue.push(std::make_pair(mTop, 3));
				logQueue.push("U')\nThe cubicle is now on the top!\n");
			}
		}
		else if (pos.x == 1) // onBlue
		{
			faceToCheckColor = BLUE;
			movesQueue.push(std::make_pair(mBlue, 0));
			logQueue.push("Face-Blue: ");
			rotationCode = 6;
			if (pos.z == 1) // andRed
			{
				movesQueue.push(std::make_pair(mLeft, 3));
				logQueue.push("(L' ");
				movesQueue.push(std::make_pair(mTop, 3));
				logQueue.push("U' ");
				movesQueue.push(std::make_pair(mLeft, 1));
				logQueue.push("L)\nThe cubicle is now on the top!\n");
			}
			else if (pos.z == -1) // andOrange
			{
				movesQueue.push(std::make_pair(mRight, 1));
				logQueue.push("(R ");
				movesQueue.push(std::make_pair(mTop, 1));
				logQueue.push("U ");
				movesQueue.push(std::make_pair(mRight, 3));
				logQueue.push("R' ");
				movesQueue.push(std::make_pair(mTop, 3));
				logQueue.push("U')\nThe cubicle is now on the top!\n");
			}
		}
	} else { // If cubicle is on top side, we need to fix the rotation code

		if (pos.x == -1) // onGreen
		{
			if (pos.z == -1) // andOrange
			{
				rotationCode = 7;
			}
			else if (pos.z == 1) // andRed
			{
				rotationCode = 4;
			}
		}
		else if (pos.x == 1) // onBlue
		{
			if (pos.z == -1) // andOrange
			{
				rotationCode = 6;
			}
			else if (pos.z == 1) // andRed
			{
				rotationCode = 5;
			}
		}
		// Also means we check White
		faceToCheckColor = WHITE;
	}

	cubicleColor frontSide = NOCOLOR, rightSide = NOCOLOR;
	switch (ID) // Need to make sure we get to the right face to place the cubicle
	{
		case 0:
			frontSide = ORANGE;
			rightSide = GREEN;
			break;
		case 6:
			frontSide = GREEN;
			rightSide = RED;
			break;
		case 2:
			frontSide = BLUE;
			rightSide = ORANGE;
			break;
		case 8:
			frontSide = RED;
			rightSide = BLUE;
			break;
	}
	RotateTopAlgorithm(rotationCode, frontSide);
	// As now we are only calculate the algorithm, we can't check top face now.
	// So we need to figure out how the cube will change when we bring it up
	// Turns out the initial face we use, will contain the top side of the cubicle, after we bring it up
	if (YELLOW == RubikCube::mainCube.getCubicleColorByPosition(pos, faceToCheckColor))
	{
		movesQueue.push(std::make_pair(mRight, 1));
		logQueue.push("(R ");
		movesQueue.push(std::make_pair(mTop, 2));
		logQueue.push("2U ");
		movesQueue.push(std::make_pair(mRight, 3));
		logQueue.push("R' ");
		movesQueue.push(std::make_pair(mTop, 3));
		logQueue.push("U') ");

		movesQueue.push(std::make_pair(mRight, 1));
		logQueue.push("(R ");
		movesQueue.push(std::make_pair(mTop, 1));
		logQueue.push("U ");
		movesQueue.push(std::make_pair(mRight, 3));
		logQueue.push("R') ");
	}
	else if (rightSide == RubikCube::mainCube.getCubicleColorByPosition(pos, faceToCheckColor))
	{
		movesQueue.push(std::make_pair(mRight, 1));
		logQueue.push("(R ");
		movesQueue.push(std::make_pair(mTop, 1));
		logQueue.push("U ");
		movesQueue.push(std::make_pair(mRight, 3));
		logQueue.push("R') ");
	}
	else if (frontSide == RubikCube::mainCube.getCubicleColorByPosition(pos, faceToCheckColor))
	{
		movesQueue.push(std::make_pair(mFront, 3));
		logQueue.push("(F' ");
		movesQueue.push(std::make_pair(mTop, 3));
		logQueue.push("U' ");
		movesQueue.push(std::make_pair(mFront, 1));
		logQueue.push("F) ");
	}
	movesQueue.push(std::make_pair(mBreak, 0));
	logQueue.push("\n");
}

void RubikAlgorithms::OuterEdgeAlgorithm(short ID) // Lateral edges
{
	glm::vec3 pos = RubikCube::mainCube.getCubiclePosition(ID);
	int rotationCode = 4; // Green = 4, Red = 5, Blue = 6, Orange = 7
	cubicleColor faceToCheckColor = NOCOLOR;
	if (pos.y == 0) {
		// Cubicle is on the bottom side of the cube, so we need to get it up
		if (pos.x == -1) // onGreen
		{
			faceToCheckColor = GREEN;
			movesQueue.push(std::make_pair(mGreen, 0));
			logQueue.push("Face-Green: ");
			rotationCode = 4;
			if (pos.z == -1) // andOrange
			{
				movesQueue.push(std::make_pair(mLeft, 3));
				logQueue.push("(L' ");
				movesQueue.push(std::make_pair(mTop, 3));
				logQueue.push("U' ");
				movesQueue.push(std::make_pair(mLeft, 1));
				logQueue.push("L ");
				movesQueue.push(std::make_pair(mTop, 1));
				logQueue.push("U ");
				movesQueue.push(std::make_pair(mFront, 1));
				logQueue.push("F ");
				movesQueue.push(std::make_pair(mTop, 1));
				logQueue.push("U ");
				movesQueue.push(std::make_pair(mFront, 3));
				logQueue.push("F' ");
				movesQueue.push(std::make_pair(mTop, 2));
				logQueue.push("2U)    The cubicle is now on the top!\n");
			}
			else if (pos.z == 1) // andRed
			{
				movesQueue.push(std::make_pair(mRight, 1));
				logQueue.push("(R ");
				movesQueue.push(std::make_pair(mTop, 1));
				logQueue.push("U ");
				movesQueue.push(std::make_pair(mRight, 3));
				logQueue.push("R' ");
				movesQueue.push(std::make_pair(mTop, 3));
				logQueue.push("U' ");
				movesQueue.push(std::make_pair(mFront, 3));
				logQueue.push("F' ");
				movesQueue.push(std::make_pair(mTop, 3));
				logQueue.push("U' ");
				movesQueue.push(std::make_pair(mFront, 1));
				logQueue.push("F ");
				movesQueue.push(std::make_pair(mTop, 2));
				logQueue.push("2U)    The cubicle is now on the top!\n");
			}
		}
		else if (pos.x == 1) // onBlue
		{
			faceToCheckColor = BLUE;
			movesQueue.push(std::make_pair(mBlue, 0));
			logQueue.push("Face-Blue: ");
			rotationCode = 6;
			if (pos.z == 1) // andRed
			{
				movesQueue.push(std::make_pair(mLeft, 3));
				logQueue.push("(L' ");
				movesQueue.push(std::make_pair(mTop, 3));
				logQueue.push("U' ");
				movesQueue.push(std::make_pair(mLeft, 1));
				logQueue.push("L ");
				movesQueue.push(std::make_pair(mTop, 1));
				logQueue.push("U ");
				movesQueue.push(std::make_pair(mFront, 1));
				logQueue.push("F ");
				movesQueue.push(std::make_pair(mTop, 1));
				logQueue.push("U ");
				movesQueue.push(std::make_pair(mFront, 3));
				logQueue.push("F' ");
				movesQueue.push(std::make_pair(mTop, 2));
				logQueue.push("2U)    The cubicle is now on the top!\n");
			}
			else if (pos.z == -1) // andOrange
			{
				movesQueue.push(std::make_pair(mRight, 1));
				logQueue.push("(R ");
				movesQueue.push(std::make_pair(mTop, 1));
				logQueue.push("U ");
				movesQueue.push(std::make_pair(mRight, 3));
				logQueue.push("R' ");
				movesQueue.push(std::make_pair(mTop, 3));
				logQueue.push("U' ");
				movesQueue.push(std::make_pair(mFront, 3));
				logQueue.push("F' ");
				movesQueue.push(std::make_pair(mTop, 3));
				logQueue.push("U' ");
				movesQueue.push(std::make_pair(mFront, 1));
				logQueue.push("F ");
				movesQueue.push(std::make_pair(mTop, 2));
				logQueue.push("2U)    The cubicle is now on the top!\n");
			}
		}
	} else { // Just need to calculate how much to rotate Top

		if (pos.x == -1) // onGreen
		{
			rotationCode = 4;
		}
		else if (pos.x == 1) // onBlue
		{
			rotationCode = 6;
		}
		else if (pos.z == -1) // onOrange
		{
			rotationCode = 7;
		}
		else if (pos.z == 1) // onRed
		{
			rotationCode = 5;
		}
		// Also means we check White
		faceToCheckColor = WHITE;
	}

	cubicleColor frontSide = NOCOLOR;
	bool towardsLeft;
	switch (ID) // Need to make sure we get to the right face to place the cubicle
	{
		case 9:
			if (ORANGE == RubikCube::mainCube.getCubicleColorByPosition(pos, faceToCheckColor))
			{
				frontSide = GREEN;
				towardsLeft = true;
			} else {
				frontSide = ORANGE;
				towardsLeft = false;
			}
			break;
		case 15:
			if (GREEN == RubikCube::mainCube.getCubicleColorByPosition(pos, faceToCheckColor))
			{
				frontSide = RED;
				towardsLeft = true;
			} else {
				frontSide = GREEN;
				towardsLeft = false;
			}
			break;
		case 11:
			if (BLUE == RubikCube::mainCube.getCubicleColorByPosition(pos, faceToCheckColor))
			{
				frontSide = ORANGE;
				towardsLeft = true;
			} else {
				frontSide = BLUE;
				towardsLeft = false;
			}
			break;
		case 17:
			if (RED == RubikCube::mainCube.getCubicleColorByPosition(pos, faceToCheckColor))
			{
				frontSide = BLUE;
				towardsLeft = true;
			} else {
				frontSide = RED;
				towardsLeft = false;
			}
			break;
	}
	RotateTopAlgorithm(rotationCode, frontSide);

	if (towardsLeft)
	{
		movesQueue.push(std::make_pair(mTop, 3));
		logQueue.push("(U' ");
		movesQueue.push(std::make_pair(mLeft, 3));
		logQueue.push("L' ");
		movesQueue.push(std::make_pair(mTop, 1));
		logQueue.push("U ");
		movesQueue.push(std::make_pair(mLeft, 1));
		logQueue.push("L) -corner-edge together- ");
		movesQueue.push(std::make_pair(mFront, 3));
		logQueue.push("(F' ");
		movesQueue.push(std::make_pair(mLeft, 1));
		logQueue.push("L ");
		movesQueue.push(std::make_pair(mFront, 1));
		logQueue.push("F ");
		movesQueue.push(std::make_pair(mLeft, 3));
		logQueue.push("L') ");
	} else {
		movesQueue.push(std::make_pair(mTop, 1));
		logQueue.push("(U ");
		movesQueue.push(std::make_pair(mRight, 1));
		logQueue.push("R ");
		movesQueue.push(std::make_pair(mTop, 3));
		logQueue.push("U' ");
		movesQueue.push(std::make_pair(mRight, 3));
		logQueue.push("R') -corner-edge together- ");
		movesQueue.push(std::make_pair(mFront, 1));
		logQueue.push("(F ");
		movesQueue.push(std::make_pair(mRight, 3));
		logQueue.push("R' ");
		movesQueue.push(std::make_pair(mFront, 3));
		logQueue.push("F' ");
		movesQueue.push(std::make_pair(mRight, 1));
		logQueue.push("R) ");
	}
	movesQueue.push(std::make_pair(mBreak, 0));
	logQueue.push("\n");
}
void RubikAlgorithms::TopUpEdgeAlgorithm(short flag) // White cross
{
	// flag 01 = L, flag10 = - , if both, do L, then U, then -
	/*
	*	Moving next linex to main Algorithm, in order to include the Top movement for pre-positioning
	*	movesQueue.push(std::make_pair(mRed, 0));
	*	logQueue.push("Face-Red: ");
	*/

	if (flag & 1)
	{
		movesQueue.push(std::make_pair(mFront, 1));
		logQueue.push("(F ");
		movesQueue.push(std::make_pair(mTop, 1));
		logQueue.push("U ");
		movesQueue.push(std::make_pair(mRight, 1));
		logQueue.push("R ");
		movesQueue.push(std::make_pair(mTop, 3));
		logQueue.push("U' ");
		movesQueue.push(std::make_pair(mRight, 3));
		logQueue.push("R' ");
		movesQueue.push(std::make_pair(mFront, 3));
		logQueue.push("F') ");
		if (flag & 2)
		{
			movesQueue.push(std::make_pair(mTop, 1));
			logQueue.push("U ");
		}
	}

	if (flag & 2)
	{
		movesQueue.push(std::make_pair(mFront, 1));
		logQueue.push("(F ");
		movesQueue.push(std::make_pair(mRight, 1));
		logQueue.push("R ");
		movesQueue.push(std::make_pair(mTop, 1));
		logQueue.push("U ");
		movesQueue.push(std::make_pair(mRight, 3));
		logQueue.push("R' ");
		movesQueue.push(std::make_pair(mTop, 3));
		logQueue.push("U' ");
		movesQueue.push(std::make_pair(mFront, 3));
		logQueue.push("F') ");
	}

	movesQueue.push(std::make_pair(mBreak, 0));
	logQueue.push("\n");
}


void RubikAlgorithms::TopUpCornerAlgorithm(std::vector<short> noRotations) // Yellow Verticles
{
	for (short i = 0; i < 4; ++i)
	{
		if (noRotations[i] == 1)
		{
			movesQueue.push(std::make_pair(mRight, 1));
			logQueue.push("(R ");
			movesQueue.push(std::make_pair(mBottom, 1));
			logQueue.push("D ");
			movesQueue.push(std::make_pair(mRight, 3));
			logQueue.push("R' ");
			movesQueue.push(std::make_pair(mBottom, 3));
			logQueue.push("D') ");
			movesQueue.push(std::make_pair(mRight, 1));
			logQueue.push("(R ");
			movesQueue.push(std::make_pair(mBottom, 1));
			logQueue.push("D ");
			movesQueue.push(std::make_pair(mRight, 3));
			logQueue.push("R' ");
			movesQueue.push(std::make_pair(mBottom, 3));
			logQueue.push("D')    This cubicle has the white face upwards now! NEXT!\n");
		}
		else if (noRotations[i] == 2)
		{
			movesQueue.push(std::make_pair(mBottom, 1));
			logQueue.push("(D ");
			movesQueue.push(std::make_pair(mRight, 1));
			logQueue.push("R ");
			movesQueue.push(std::make_pair(mBottom, 3));
			logQueue.push("D' ");
			movesQueue.push(std::make_pair(mRight, 3));
			logQueue.push("R') ");
			movesQueue.push(std::make_pair(mBottom, 1));
			logQueue.push("(D ");
			movesQueue.push(std::make_pair(mRight, 1));
			logQueue.push("R ");
			movesQueue.push(std::make_pair(mBottom, 3));
			logQueue.push("D' ");
			movesQueue.push(std::make_pair(mRight, 3));
			logQueue.push("R')    This cubicle has the white face upwards now! NEXT!\n");

		}
		movesQueue.push(std::make_pair(mTop, 1));
		logQueue.push("U ");
	}
	movesQueue.push(std::make_pair(mBreak, 0));
	logQueue.push("\n");
}

short RubikAlgorithms::TopRotateEdgeAlgorithm(short position, bool isClockwise)
{
	short ret = 0;
	switch (position)
	{
	case 3:
		ret = 1;
		movesQueue.push(std::make_pair(mTop, 3));
		logQueue.push("U' ");
		break;
	case 1:
		ret = 2;
		movesQueue.push(std::make_pair(mTop, 2));
		logQueue.push("2U ");
		break;
	case 5:
		ret = 3;
		movesQueue.push(std::make_pair(mTop, 1));
		logQueue.push("U ");
		break;
	case 7:
		ret = 0;
		break;
	}
	if (isClockwise)
	{
		movesQueue.push(std::make_pair(mRight, 2));
		logQueue.push("(2R ");
		movesQueue.push(std::make_pair(mTop, 1));
		logQueue.push("U ");
		movesQueue.push(std::make_pair(mRight, 1));
		logQueue.push("R ");
		movesQueue.push(std::make_pair(mTop, 1));
		logQueue.push("U ");
		movesQueue.push(std::make_pair(mRight, 3));
		logQueue.push("R' ");
		movesQueue.push(std::make_pair(mTop, 3));
		logQueue.push("U' ");
		movesQueue.push(std::make_pair(mRight, 3));
		logQueue.push("R' ");
		movesQueue.push(std::make_pair(mTop, 3));
		logQueue.push("U' ");
		movesQueue.push(std::make_pair(mRight, 3));
		logQueue.push("R' ");
		movesQueue.push(std::make_pair(mTop, 1));
		logQueue.push("U ");
		movesQueue.push(std::make_pair(mRight, 3));
		logQueue.push("R') ");
	} else {
		movesQueue.push(std::make_pair(mRight, 1));
		logQueue.push("(R ");
		movesQueue.push(std::make_pair(mTop, 3));
		logQueue.push("U' ");
		movesQueue.push(std::make_pair(mRight, 1));
		logQueue.push("R ");
		movesQueue.push(std::make_pair(mTop, 1));
		logQueue.push("U ");
		movesQueue.push(std::make_pair(mRight, 1));
		logQueue.push("R ");
		movesQueue.push(std::make_pair(mTop, 1));
		logQueue.push("U ");
		movesQueue.push(std::make_pair(mRight, 1));
		logQueue.push("R ");
		movesQueue.push(std::make_pair(mTop, 3));
		logQueue.push("U' ");
		movesQueue.push(std::make_pair(mRight, 3));
		logQueue.push("R' ");
		movesQueue.push(std::make_pair(mTop, 3));
		logQueue.push("U' ");
		movesQueue.push(std::make_pair(mRight, 2));
		logQueue.push("2R) ");
	}
	return ret;
}
void RubikAlgorithms::TopRotateCornerAlgorithm(bool isClockwise)
{
	if (isClockwise)
	{
		movesQueue.push(std::make_pair(mRight,3));
		logQueue.push("(R' ");
		movesQueue.push(std::make_pair(mFront, 1));
		logQueue.push("F ");
		movesQueue.push(std::make_pair(mRight, 3));
		logQueue.push("R'");
		movesQueue.push(std::make_pair(mBack, 2));
		logQueue.push("2B ");
		movesQueue.push(std::make_pair(mRight, 1));
		logQueue.push("R ");
		movesQueue.push(std::make_pair(mFront, 3));
		logQueue.push("F' ");
		movesQueue.push(std::make_pair(mRight, 3));
		logQueue.push("R' ");
		movesQueue.push(std::make_pair(mBack, 2));
		logQueue.push("2B ");
		movesQueue.push(std::make_pair(mRight, 2));
		logQueue.push("2R) ");
	}
	else {
		movesQueue.push(std::make_pair(mRight, 2));
		logQueue.push("(2R ");
		movesQueue.push(std::make_pair(mBack, 2));
		logQueue.push("2B ");
		movesQueue.push(std::make_pair(mRight, 1));
		logQueue.push("R ");
		movesQueue.push(std::make_pair(mFront, 1));
		logQueue.push("F ");
		movesQueue.push(std::make_pair(mRight, 3));
		logQueue.push("R' ");
		movesQueue.push(std::make_pair(mBack, 2));
		logQueue.push("2B ");
		movesQueue.push(std::make_pair(mRight, 1));
		logQueue.push("R ");
		movesQueue.push(std::make_pair(mFront, 3));
		logQueue.push("F' ");
		movesQueue.push(std::make_pair(mRight, 1));
		logQueue.push("R) ");
	}
}

void RubikAlgorithms::RotateTopAlgorithm(short rotationCode, cubicleColor faceNeeded)
{
	switch (faceNeeded) // Need to make sure we get to the right face to place the cubicle and calculate rotation
	{
		case ORANGE:
			rotationCode = (rotationCode - 3) % 4;
			movesQueue.push(std::make_pair(mOrange, 0));
			logQueue.push("Face-Orange: ");
			break;
		case GREEN:
			rotationCode = (rotationCode - 4) % 4;
			movesQueue.push(std::make_pair(mGreen, 0));
			logQueue.push("Face-Green: ");
			break;
		case BLUE:
			rotationCode = (rotationCode - 2) % 4;
			movesQueue.push(std::make_pair(mBlue, 0));
			logQueue.push("Face-Blue: ");
			break;
		case RED:
			rotationCode = (rotationCode - 1) % 4;
			movesQueue.push(std::make_pair(mRed, 0));
			logQueue.push("Face-Red: ");
			break;
	}
	switch (rotationCode) // Now we need to select the log for the rotation
	{
		case 1:
			movesQueue.push(std::make_pair(mTop, 1));
			logQueue.push("U ");
			break;
		case 2:
			movesQueue.push(std::make_pair(mTop, 2));
			logQueue.push("2U ");
			break;
		case 3:
			movesQueue.push(std::make_pair(mTop, 3));
			logQueue.push("U' ");
			break;
	}
}

short RubikAlgorithms::TopLookAround(short pos, short direction) // 1 == left, 0 == across, -1 = right
{
	switch (pos)
	{
		// These are for edges
	case 1:
		switch (direction)
		{
		case 1:
			return 5;
		case 0:
			return 7;
		case -1:
			return 3;
		}
	case 3:
		switch (direction)
		{
		case 1:
			return 1;
		case 0:
			return 5;
		case -1:
			return 7;
		}
	case 5:
		switch (direction)
		{
		case 1:
			return 7;
		case 0:
			return 3;
		case -1:
			return 1;
		}
	case 7:
		switch (direction)
		{
		case 1:
			return 3;
		case 0:
			return 1;
		case -1:
			return 5;
		}

		// These are for corners
	case 0:
		switch (direction)
		{
		case 1:
			return 2;
		case 0:
			return 8;
		case -1:
			return 6;
		}
	case 2:
		switch (direction)
		{
		case 1:
			return 8;
		case 0:
			return 6;
		case -1:
			return 0;
		}
	case 6:
		switch (direction)
		{
		case 1:
			return 0;
		case 0:
			return 2;
		case -1:
			return 8;
		}
	case 8:
		switch (direction)
		{
		case 1:
			return 6;
		case 0:
			return 0;
		case -1:
			return 2;
		}
	}
	return -1; // if error
}

void RubikAlgorithms::doNextMove()
{
	// Read moves from queue
	faceMove move = movesQueue.front().first;
	short modifier = movesQueue.front().second;
	std::string str = logQueue.front();
	movesQueue.pop();
	logQueue.pop();

	if (move >= mGreen && move <= mRed)
	{
		// We got a face-changing move
		forceFace = cubicleColor(move - mGreen);
	} else if (move >= mFront && move <= mBottom){
		// We got a rotation move
		rotateFace(move, modifier, forceFace);
	}
	if (move == mBreak)
	{
		freeze(!isFrozen());
	}
	if (movesQueue.empty())
	{
		freeze(false);
	}

	addHintsToScreen((char*) str.c_str());
}

void RubikAlgorithms::getHint(hintMessages hint)
{
	if (hintActivated)
	switch (hint)
	{
	case RubikAlgorithms::hintYellowCross:
		addHintsToScreen("The first thing you must do is the YELLOW cross.\n"
			"To do this you must take each yellow edge and place it to it's right position.\n"
			"You take the edge, you bring it to the TOP - with YELLOW up - "
			"rotate TOP (U) until you get it to it's corresponding colour, then rotate 2F.\n");
		break;
	case RubikAlgorithms::hintYellowCorners:
		addHintsToScreen("Next, you must place the YELLOW corners.\n"
			"You take the corner, you bring it to the TOP, rotate TOP (U) until you bring it "
			"above it's rightful position, then place them below using the corresponding algorithm.\n");
		break;
	case RubikAlgorithms::hintOuterEdges:
		addHintsToScreen("In order to finish the 2nd level, you need to place the LATERAL edges\n"
			"You bring the edges to the TOP, rotate TOP (U) until the edges matches the frontal center"
			"then place them below using the corresponding algorithm. Note that this algorithm got 2 phases:\n"
			"First you bring lowercorner near the edge, and then you place them both downwards.\n");
		break;
	case RubikAlgorithms::hintWhiteCross:
		addHintsToScreen("We now reached the final level, the top level. \n"
			"Just like on the bottom, we need to form a cross, the WHITE cross\n"
			"There are 3 situations to do this and only 2 algorithms : mirrored-L algorithm and line-algorithm"
			"To keep it simple: you see an L, you do 1st algorithm, you see a line you do the 2nd, you see a point, you do 1st and then the 2nd algorithm.\n"
			//"Note: If your Real-life cube has only 1, or 3 edges with white up, then one of your cubicles has been forcefully rotated!\n"
			);
		break;
	case RubikAlgorithms::hintWhiteUp:
		addHintsToScreen("You now need to finish the white face\n"
			"These 2 algorithms spins a corner clockwise or counterclockwise.\n"
			"After you spin the curent corner, to get to the next one rotate TOP(U) NOT whole cube.\n"
			"It is important that when you do this step that you DO NOT PANIC!\n"
			"The cube will look as if it scrambles, but after you spin all the corners, it will return to it's previous state.\n"
			//"Note: If you do all the corners right but your Real-life cube remains scrambled, then one of your cubicles has been forcefully rotated!\n"
			);
		break;
	case RubikAlgorithms::hintEdgeRotation:
		addHintsToScreen("Now that all the cubicles  have their white face on top, "
			"we need to rotate between them, until we place them in the correct spot."
			"Start with the edges. We have 2 algorithms: rotate edges clockwise or counter-clockwise\n"
	);
		break;
	case RubikAlgorithms::hintCornerRotation:
			addHintsToScreen("Only 1 step remaining to solve the cube: "
			"rotate the corners, until we place them in the correct spot.\n"
			"similar to the edges, we can rotate them clockwise or counter-clockwise\n"
	);
		break;
	default:
		break;
	}
}
void RubikAlgorithms::toggleHints(bool val)
{
	// This function can be used to deactivate hints. Not included in 1.0 :)
	hintActivated = val;
}
