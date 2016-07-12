#include <cstdio>

#include "Cube.hpp"

RubikCube RubikCube::mainCube;

// This function calculates the ID of a cubicle
short getIDFromColor(cubicleColor** colors)
{
    short ID = 13; // Center cubicle

    for (short i = 0; i < 3; ++i)
    {
        switch(*colors[i])
        {
            case GREEN:
                --ID;
            break;
            case BLUE:
                ++ID;
            break;
            case ORANGE:
                ID -= 3;
            break;
            case RED:
                ID += 3;
            break;
            case YELLOW:
                ID -= 9;
            break;
            case WHITE:
                ID += 9;
            break;
        }
    }
    return ID;
}

short getIDFromColor(cubicleColor a, cubicleColor b, cubicleColor c)
{
    cubicleColor* colors[3];
    colors[0] = &a;
    colors[1] = &b;
    colors[2] = &c;
    return getIDFromColor(colors);
}

RubikCube::RubikCube()
{
    for (short i = 0; i < 27; ++i)
    {
        cubicles.push_back(new Cubicle(i));
    }
    initFaces();
}

RubikCube::~RubikCube()
{
    Cubicle *ptr;
    while (!cubicles.empty())
    {
        ptr = cubicles.back();
        cubicles.pop_back();
        delete ptr;
    }
}

// This function calculates the rotation
void RubikCube::rotateLogic(cubicleColor face, short times)
{
    short ID_temp;
    cubicleColor color_temp;

    while (times--)
    {
        //rotate 2-color cubicles
        ID_temp				= facesID[face][1];
        facesID[face][1]	= facesID[face][3];
        facesID[face][3]	= facesID[face][7];
        facesID[face][7]	= facesID[face][5];
        facesID[face][5]	= ID_temp;
        color_temp				= facesColor[face][1];
        facesColor[face][1]		= facesColor[face][3];
        facesColor[face][3]		= facesColor[face][7];
        facesColor[face][7]		= facesColor[face][5];
        facesColor[face][5]		= color_temp;

        //rotate 3-color cubicles
        ID_temp				= facesID[face][0];
        facesID[face][0]	= facesID[face][6];
        facesID[face][6]	= facesID[face][8];
        facesID[face][8]	= facesID[face][2];
        facesID[face][2]	= ID_temp;
        color_temp				= facesColor[face][0];
        facesColor[face][0]		= facesColor[face][6];
        facesColor[face][6]		= facesColor[face][8];
        facesColor[face][8]		= facesColor[face][2];
        facesColor[face][2]		= color_temp;

        short j, k, i[2][3];
        cubicleColor lCol, rCol;
        switch(face)//rotate nearby faces
        {
            case GREEN:
                i[0][0] = 0;    i[1][0] = 0;
                i[0][1] = 3;    i[1][1] = 3;
                i[0][2] = 6;    i[1][2] = 6;
                lCol = ORANGE;
                rCol = RED;
                break;
            case BLUE:
                i[0][0] = 8;    i[1][0] = 8;
                i[0][1] = 5;    i[1][1] = 5;
                i[0][2] = 2;    i[1][2] = 2;
                lCol = RED;
                rCol = ORANGE;
                break;
            case ORANGE:
                i[0][0] = 2;    i[1][0] = 6;
                i[0][1] = 1;    i[1][1] = 7;
                i[0][2] = 0;    i[1][2] = 8;
                lCol = BLUE;
                rCol = GREEN;
                break;
            case RED:
                i[0][0] = 6;    i[1][0] = 2;
                i[0][1] = 7;    i[1][1] = 1;
                i[0][2] = 8;    i[1][2] = 0;
                lCol = GREEN;
                rCol = BLUE;
                break;
            case YELLOW:
                k = 6;
                break;
            case WHITE:
                k = 0;
                break;
        }

        if(face < YELLOW) // lateral faces
        {
            for (j = 0; j < 3; ++j)
            {
                ID_temp						= facesID[WHITE][i[0][j]];
                facesID[WHITE][i[0][j]]		= facesID[lCol][8 - j*3];
                facesID[lCol][8 - j*3]		= facesID[YELLOW][i[1][j]];
                facesID[YELLOW][i[1][j]]	= facesID[rCol][j * 3];
                facesID[rCol][j * 3]		= ID_temp;
                color_temp						= facesColor[WHITE][i[0][j]];
                facesColor[WHITE][i[0][j]]		= facesColor[lCol][8 - j*3];
                facesColor[lCol][8 - j*3]		= facesColor[YELLOW][i[1][j]];
                facesColor[YELLOW][i[1][j]]		= facesColor[rCol][j * 3];
                facesColor[rCol][j * 3]			= color_temp;
            }
        } else if(face == YELLOW) {
            for (j = 0; j < 3; ++j)
            {
                ID_temp					= facesID[RED][j+k];
                facesID[RED][j+k]		= facesID[GREEN][j+k];
                facesID[GREEN][j+k]		= facesID[ORANGE][j+k];
                facesID[ORANGE][j+k]    = facesID[BLUE][j+k];
                facesID[BLUE][j+k]		= ID_temp;
                color_temp					= facesColor[RED][j+k];
                facesColor[RED][j+k]		= facesColor[GREEN][j+k];
                facesColor[GREEN][j+k]		= facesColor[ORANGE][j+k];
                facesColor[ORANGE][j+k]		= facesColor[BLUE][j+k];
                facesColor[BLUE][j+k]		= color_temp;
            }
		} else {//if (face == WHITE)
			for (j = 0; j < 3; ++j)
			{
				ID_temp = facesID[RED][j + k];
				facesID[RED][j + k] = facesID[BLUE][j + k];
				facesID[BLUE][j + k] = facesID[ORANGE][j + k];
				facesID[ORANGE][j + k] = facesID[GREEN][j + k];
				facesID[GREEN][j + k] = ID_temp;
				color_temp = facesColor[RED][j + k];
				facesColor[RED][j + k] = facesColor[BLUE][j + k];
				facesColor[BLUE][j + k] = facesColor[ORANGE][j + k];
				facesColor[ORANGE][j + k] = facesColor[GREEN][j + k];
				facesColor[GREEN][j + k] = color_temp;
			}
		}
    }

}

// This function commands the rotation
void RubikCube::rotateGraphic(cubicleColor face, short times)
{
    for (short i = 0; i < 9; ++i)
    {
        cubicles[facesID[face][i]]->rotate(times, face);
    }
}

// This function executes the rotation
void RubikCube::animate(float dt)
{
	for (short i = 0; i < 27; ++i)
	{
		cubicles[i]->rotateAnimation(dt);
	}
}

void RubikCube::initFaces() {
    initFacesSolved();
    //TODO: add some condition to figure out how the cube will start
    //initFacesFromInput
}

void RubikCube::initFacesSolved() {
    short ID[9];
    cubicleColor col[9];

    // Looks wierd but is right :p

    // Green
    for (short j, i = 0; i < 27; i+=3)
    {
        j = ((i % 9) / 3) + 3 *(2 -(i / 9));
        ID[j] = i;
        col[j] = GREEN;
    }
    setFace(GREEN, ID, col);

    // Blue
    for (short j, i = 2; i < 27; i+=3)
    {
        j = (2 - (i % 9) / 3) + 3 *(2 -(i / 9));
        ID[j] = i;
        col[j] = BLUE;
    }
    setFace(BLUE, ID, col);

    //Orange
    for (short k, j, i = 0; i < 27; i+=6)
    for (k = i + 3; i < k; ++i)
    {
        j = (2 - (i % 3)) + 3 *(2 -(i / 9));
        ID[j] = i;
        col[j] = ORANGE;
    }
    setFace(ORANGE, ID, col);

    //Red
    for (short k, j, i = 6; i < 27; i+=6)
    for (k = i + 3; i < k; ++i)
    {
        j = ((i % 3)) + 3 *(2 -(i / 9));
        ID[j] = i;
        col[j] = RED;
    }
    setFace(RED, ID, col);

    //Yellow
    for (short j, i = 0; i < 9; ++i)
    {
        j = ((i % 3)) + 3 *(2 -(i / 3));
        ID[j] = i;
        col[j] = YELLOW;
    }
    setFace(YELLOW, ID, col);

    //White
    for (short i = 0; i < 9; ++i)
    {
        ID[i] = i + 18;
        col[i] = WHITE;
    }
    setFace(WHITE, ID, col);
}

// This function will initiate the cube with an user-inputed configuration. Not included in 1.0 :)
void RubikCube::initFacesFromInput() {
	FILE * cubeInput;
	fopen_s(&cubeInput, "cubeConfig.dat", "r");
    if (cubeInput)
    {
        int color;
        for (int i = 0; i < 9; ++i)
        {
			fscanf_s(cubeInput, "%d", &color);
            //facesID[color][i] = ID[i];
            facesColor[WHITE][i] = (cubicleColor)color;
        }
		for (int i = 0; i < 9; ++i)
		{
			fscanf_s(cubeInput, "%d", &color);
            //facesID[color][i] = ID[i];
            facesColor[RED][i] = (cubicleColor)color;
        }
		for (int i = 0; i < 9; ++i)
		{
			fscanf_s(cubeInput, "%d", &color);
            //facesID[color][i] = ID[i];
            facesColor[BLUE][i] = (cubicleColor)color;
        }
		for (int i = 0; i < 9; ++i)
		{
			fscanf_s(cubeInput, "%d", &color);
            //facesID[color][i] = ID[i];
            facesColor[ORANGE][i] = (cubicleColor)color;
        }
		for (int i = 0; i < 9; ++i)
		{
			fscanf_s(cubeInput, "%d", &color);
            //facesID[color][i] = ID[i];
            facesColor[GREEN][i] = (cubicleColor)color;
        }
		for (int i = 0; i < 9; ++i)
		{
			fscanf_s(cubeInput, "%d", &color);
            //facesID[color][i] = ID[i];
            facesColor[YELLOW][i] = (cubicleColor)color;
        }

        cubicleColor _colors[3];
        cubicleColor* colors[3];
        colors[0] = &_colors[0];
        colors[1] = &_colors[1];
        colors[2] = &_colors[2];
        int ID;



        for (int i = 0; i < 27; ++i) // Location
        {
            _colors[0] = NOCOLOR;
            _colors[1] = NOCOLOR;
            _colors[2] = NOCOLOR;
            int k = 0;
            if (i % 3 == 2)
            {
                *colors[k++] = BLUE;
            }
            else if (i % 3 == 0)
            {
                *colors[k++] = GREEN;
            }
            if (i / 9 == 2)
            {
                *colors[k++] = RED;
            }
            else if (i / 9 == 0)
            {
                *colors[k++] = ORANGE;
            }
            if ((i / 3) % 3 == 2)
            {
                *colors[k++] = WHITE;
            }
            else if ((i / 3) % 3 == 0)
            {
                *colors[k++] = YELLOW;
            }
            ID = getIDFromColor(colors); //actual cube ID
            repositionCubiclesByLogic();
            while (k)
            {
                facesID[_colors[k--]][i] = ID;
            }
        }
        return;
    }
    fprintf(stderr, "There is no saved configuration for the cube!\n Please use the setup first!\n");

}

// Function that repositions the cubicles by specific colors. See RubikCube::initFacesFromInput()
void RubikCube::repositionCubiclesByLogic()
{
    /*if (i % 3 == 2)
    {
        j = (2 - (i % 9) / 3) + 3 *(2 -(i / 9));
        *colors[k++] = facesColor[BLUE][j];
    }
    else if (i % 3 == 0)
    {
        j = ((i % 9) / 3) + 3 *(2 -(i / 9));
        *colors[k++] = facesColor[GREEN][j];
    }
    if (i / 9 == 2)
    {
        j = ((i % 3)) + 3 *(2 -(i / 9));
        *colors[k++] = facesColor[RED][j];
    }
    else if (i / 9 == 0)
    {
        j = (2 - (i % 3)) + 3 *(2 -(i / 9));
        *colors[k++] = facesColor[ORANGE][j];
    }
    if ((i / 3) % 3 == 2)
    {
        *colors[k++] = facesColor[WHITE][i-18];
    }
    else if ((i / 3) % 3 == 0)
    {
        j = ((i % 3)) + 3 *(2 -(i / 3));
        *colors[k++] = facesColor[YELLOW][j];
    }*/
    return;
}


void RubikCube::setFace(cubicleColor color, short *ID, cubicleColor *col)
{
    for (short i = 0; i < 9; ++i)
    {
        facesID[color][i] = ID[i];
        facesColor[color][i] = col[i];
    }
}

float* RubikCube::getCubicleModel(short ID)
{
	return cubicles[ID]->getModel();
}

glm::vec3 RubikCube::getCubiclePosition(short ID)
{
	return cubicles[ID]->getPosition();
}

// This function reads a color of a cubicle at a specific location on a face
cubicleColor RubikCube::getCubicleColorByPosition(glm::vec3 pos, cubicleColor face)
{
	short falseID = 3 * (3 * ((int)pos.y + 1) + (int)pos.z + 1) + (int)pos.x + 1;
	short j;
	switch (face)
	{
		case GREEN:
			if (falseID % 3) return NOCOLOR;
			j = ((falseID % 9) / 3) + 3 * (2 - (falseID / 9));
			return facesColor[face][j];
		case BLUE:
			if ((falseID + 1) % 3) return NOCOLOR;
			j = (2 - (falseID % 9) / 3) + 3 * (2 - (falseID / 9));
			return facesColor[face][j];
		case ORANGE:
			if ((falseID / 3) % 3) return NOCOLOR;
			j = (2 - (falseID % 3)) + 3 * (2 - (falseID / 9));
			return facesColor[face][j];
		case RED:
			if ((falseID / 3 + 1) % 3) return NOCOLOR;
			j = ((falseID % 3)) + 3 * (2 - (falseID / 9));
			return facesColor[face][j];
		case YELLOW:
			if (falseID >= 9) return NOCOLOR;
			j = ((falseID % 3)) + 3 * (2 - (falseID / 3));
			return facesColor[face][j];
		case WHITE:
			if (falseID < 18) return NOCOLOR;
			return facesColor[face][falseID - 18];
		default:
			fprintf(stderr, "RubikCube::getCubicleColorByPosition says: face does not have this cubicle! %d\n", face);
			break;
	}
	return NOCOLOR;

}

bool RubikCube::isCubicleRotated(glm::vec3 pos, cubicleColor face)
{
	return !(face == getCubicleColorByPosition(pos, face));
}
bool RubikCube::isCubicleOK(short ID, cubicleColor faceToCheck)
{
	return (cubicles[ID]->isOK() && !isCubicleRotated(cubicles[ID]->getPosition(), faceToCheck));
}

// This function reads the Up-face for the Top Layer Algorithms
std::vector<short> RubikCube::getWhiteUpPositions()
{
	std::vector<short> wUp;
	for (int i = 0; i < 9; ++i)
		if (facesColor[WHITE][i] == WHITE) {
			wUp.push_back(1);
		}
		else {
			wUp.push_back(0);
		}
		return wUp;
}

// This function reads the Up-face for the Top Layer Algorithms
std::vector<short> RubikCube::getWhiteCubiclesPositions()
{
	std::vector<short> wUp;
	for (int i = 0; i < 9; ++i)
		wUp.push_back(0);
	for (int i = 0; i < 9; ++i)
		wUp[facesID[WHITE][i]-18] = i;
	return wUp;
}

std::vector<short> RubikCube::getRotationsNeededForWhite()
{
	std::vector<short> noRotations;

	if (facesColor[WHITE][2] == WHITE)
		noRotations.push_back(0);
	else if (facesColor[BLUE][2] == WHITE)
		noRotations.push_back(1);
	else if (facesColor[ORANGE][0] == WHITE)
		noRotations.push_back(2);

	if (facesColor[WHITE][0] == WHITE)
		noRotations.push_back(0);
	else if (facesColor[ORANGE][2] == WHITE)
		noRotations.push_back(1);
	else if (facesColor[GREEN][0] == WHITE)
		noRotations.push_back(2);

	if (facesColor[WHITE][6] == WHITE)
		noRotations.push_back(0);
	else if (facesColor[GREEN][2] == WHITE)
		noRotations.push_back(1);
	else if (facesColor[RED][0] == WHITE)
		noRotations.push_back(2);

	if (facesColor[WHITE][8] == WHITE)
		noRotations.push_back(0);
	else if (facesColor[RED][2] == WHITE)
		noRotations.push_back(1);
	else if (facesColor[BLUE][0] == WHITE)
		noRotations.push_back(2);

	return noRotations;
}
