#ifndef _COMMON_HPP_
#define _COMMON_HPP_

#include <string>


enum cubicleColor { GREEN, BLUE, ORANGE, RED, YELLOW, WHITE, NOCOLOR };
/**
*   Note on ID <-> Colors:
*   ID : 3 digits, (0,1,2); 0 <= ID < 3*3*3
*   ID = xyz;
*   x :(0 = yellow, 1 = nocolor, 2 = white)
*   y :(0 = orange, 1 = nocolor, 2 = red)
*   z :(0 = green,  1 = nocolor, 2 = blue)
**/

void frontColChange(short);
unsigned int printText2D(const char* str, float x, float y, float textsize);
void addHintsToScreen(char* str);

class Common
{
public:
	static unsigned int UVBuffer;
	static unsigned int vertexBuffer;
	static unsigned int textUniformID;
	static unsigned int textShaderID;
	static unsigned int textTexture;
	static unsigned int idxTxtBuffer;
	static std::string hintText;
};

static const float idx_vertex_buffer_data[] = {
	-1.010f,	-0.900f,	-0.900f, // Green : begin
	-1.010f,	-0.900f,	 0.900f,
	-1.010f,	 0.900f,	-0.900f,
	-1.010f,	 0.900f,	 0.900f, // Green : end
	-0.900f,	-0.900f,	-1.010f, // Orange : begin
	-0.900f,	 0.900f,	-1.010f,
	 0.900f,	-0.900f,	-1.010f,
	 0.900f,	 0.900f,	-1.010f, // Orange : end
	-0.900f,	-1.010f,	-0.900f, // Yellow : begin
	-0.900f,	-1.010f,	 0.900f,
	 0.900f,	-1.010f,	-0.900f,
	 0.900f,	-1.010f,	 0.900f, // Yellow : end
	-0.900f,	-0.900f,	 1.010f, // Red : begin
	-0.900f,	 0.900f,	 1.010f,
	 0.900f,	-0.900f,	 1.010f,
	 0.900f,	 0.900f,	 1.010f, // Red : end
	 1.010f,	-0.900f,	-0.900f, // Blue : begin
	 1.010f,	-0.900f,	 0.900f,
	 1.010f,	 0.900f,	-0.900f,
	 1.010f,	 0.900f,	 0.900f, // Blue : end
	-0.900f,	 1.010f,	-0.900f, // White : begin
	-0.900f,	 1.010f,	 0.900f,
	 0.900f,	 1.010f,	-0.900f,
	 0.900f,	 1.010f,	 0.900f, // White : end
	//and now some black edge
	-1.0f,	-1.0f,	-1.0f,
	-1.0f,	-1.0f,	 1.0f,
	-1.0f,	 1.0f,	-1.0f,
	-1.0f,	 1.0f,	 1.0f,
	 1.0f,	-1.0f,	-1.0f,
	 1.0f,	-1.0f,	 1.0f,
	 1.0f,	 1.0f,	-1.0f,
	 1.0f,	 1.0f,	 1.0f
};
static const float idx_color_buffer_data[] = {
    0.000f,	0.700f, 0.000f, // Green : begin
    0.000f, 0.700f, 0.000f,
	0.000f, 0.700f, 0.000f,
	0.000f, 0.700f, 0.000f, // Green : end
	1.000f, 0.600f, 0.000f, // Orange : begin
	1.000f, 0.600f, 0.000f,
    1.000f, 0.600f, 0.000f,
	1.000f, 0.600f, 0.000f, // Orange : end
	1.000f, 1.000f, 0.000f, // Yellow : begin
	1.000f, 1.000f, 0.000f,
	1.000f, 1.000f, 0.000f,
	1.000f, 1.000f, 0.000f, // Yellow : end
	0.800f, 0.000f, 0.000f, // Red : begin
	0.800f, 0.000f, 0.000f,
	0.800f, 0.000f, 0.000f,
	0.800f, 0.000f, 0.000f, // Red : end
	0.000f, 0.000f, 0.800f, // Blue : begin
    0.000f, 0.000f, 0.800f,
    0.000f, 0.000f, 0.800f,
	0.000f, 0.000f, 0.800f, // Blue : end
	1.000f, 1.000f, 1.000f, // White : begin
    1.000f, 1.000f, 1.000f,
    1.000f, 1.000f, 1.000f,
	1.000f, 1.000f, 1.000f, // White : end
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0,
    0, 0, 0
};
static const unsigned short idx_buffer_data[] = {
	0,	1,	2,
	1,	2,	3,	//Green
	4,	5,	6,
	5,	6,	7,	//Orange
	8,	9,	10,
	9,	10,	11,	//Yellow
	12,	13,	14,
	13,	14,	15,	//Red
	16, 17, 18,
	17, 18,	19,	//Blue
	20, 21, 22,
	21, 22,	23,	//White
	//black edges
	24, 25, 26,
	25, 26,	27,
	28, 29, 30,
	29, 30, 31,
	24, 25, 28,
	25, 28, 29,
	26, 27, 30,
	27, 30, 31,
	24, 26, 28,
	26, 28, 30,
	25, 27, 29,
	27, 29, 31
};


#endif // _COMMON_HPP_
