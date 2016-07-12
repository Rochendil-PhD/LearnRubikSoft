#include <vector>
#include <cstring>
#include <string>
#include <iostream>

#include "..\lib\glew\glew.h"
#include "..\lib\freeglut\freeglut.h"
#include "..\lib\glfw\glfw3.h"
#include "..\lib\glm\glm.hpp"

#include "Common.hpp"

unsigned int Common::vertexBuffer;
unsigned int Common::UVBuffer;
unsigned int Common::textUniformID;
unsigned int Common::textShaderID;
unsigned int Common::textTexture;
unsigned int Common::idxTxtBuffer;
std::string Common::hintText;

unsigned int printText2D(const char* str, float x, float y, float textsize)
{
    std::vector<glm::vec2> pos;
	std::vector<glm::vec2> UVs;
	std::vector<int> index;

	float textsizex = textsize / 1400.0f;
	float textsizey = textsize / 800.0f;

    int length = strlen(str);
    for (int i = 0, j = 0, _i = 0; i < length; ++i, ++_i)
    {
		if (str[i] == '\n')
		{
			_i = -1;
			++j;
			continue;
		}

		// Position for each letter
		glm::vec2 LD = glm::vec2(x + _i * textsizex - textsizex*0, y - j * textsizey);
        glm::vec2 RD = glm::vec2(x + _i * textsizex + textsizex*1, y - j * textsizey);
		glm::vec2 LU = glm::vec2(x + _i * textsizex - textsizex*0, y - j * textsizey + textsizey);
		glm::vec2 RU = glm::vec2(x + _i * textsizex + textsizex*1, y - j * textsizey + textsizey);

		pos.push_back(LU);
		pos.push_back(LD);
		pos.push_back(RU);

		pos.push_back(RD);
		pos.push_back(RU);
		pos.push_back(LD);

        float uv_x = (str[i] % 16) / 16.0f;
        float uv_y = (str[i] / 16) / 16.0f;

		// Where to read for texture of the letter
        LD = glm::vec2(uv_x,				1.0f - (uv_y + 1.0f/16.0f));
        RD = glm::vec2(uv_x + 1.0f/16.001f,	1.0f - (uv_y + 1.0f/16.0f));
        LU = glm::vec2(uv_x,				1.0f - uv_y);
        RU = glm::vec2(uv_x + 1.0f/16.001f,	1.0f - uv_y);

		UVs.push_back(LU);
		UVs.push_back(LD);
		UVs.push_back(RU);

		UVs.push_back(RD);
		UVs.push_back(RU);
		UVs.push_back(LD);
    }

	glBindBuffer(GL_ARRAY_BUFFER, Common::vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, pos.size() *sizeof(glm::vec2), &pos[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, Common::UVBuffer);
	glBufferData(GL_ARRAY_BUFFER, UVs.size() *sizeof(glm::vec2), &UVs[0], GL_STATIC_DRAW);

	glUseProgram(Common::textShaderID);

	// Bind texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Common::textTexture);
	glUniform1i(Common::textUniformID, 0);

    glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, Common::vertexBuffer);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, Common::UVBuffer);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glDrawArrays(GL_TRIANGLES, 0, pos.size());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	return 0;
}

void addHintsToScreen(char* str)
{
	const int maxCharPerLine = 35;
	const int maxNoLines = 30;

	// Add the new hints to current text
	Common::hintText += str;

	fprintf(stdout, str);

	int endlines = 0;
	int lastEndline = 0;
	int lastSpace = 0;
	int textsize = Common::hintText.size();
	
	// Delete extra text in order to make space for new text
	for (int i = 0; i < textsize; ++i)
	{
		// Count the spaces
		if (Common::hintText[i] == ' ')
		{
			// If too many, we add a newline
			if (i - lastEndline >= maxCharPerLine)
			{
				Common::hintText[lastSpace] = '\n';
				lastEndline = lastSpace;
				++endlines;
			}
			lastSpace = i;
		}

		// Count the newlines
		if (Common::hintText[i] == '\n')
		{
			lastEndline = i;
			++endlines;
		}
	}

	// If we have too many lines, we delete some
	while (endlines > maxNoLines)
	{
		int x = Common::hintText.find_first_of("\n") + 1;
		Common::hintText.erase(Common::hintText.begin(), Common::hintText.begin() + x);
		--endlines;
	}

}
