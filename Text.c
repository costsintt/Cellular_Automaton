#include "Text.h"

#define FONT_SIZE 25
#define FONT_MAX 2
#define NUM_GLYPHS 128
#define FONT_TEXTURE_SIZE 250
#define MAX_WORD_LENGTH 50
#define MAX_LINE_LENGTH 500

enum
{
	FONT_BASIC,
	FONT_BASIC_ITALIC
};

enum {
	TEXT_ALIGN_LEFT,
	TEXT_ALIGN_CENTER,
	TEXT_ALIGN_RIGHT
};

static TTF_Font* fonts[FONT_MAX];
static SDL_Rect glyphs[FONT_MAX][NUM_GLYPHS];
static SDL_Texture* fontTextures[FONT_MAX];


static void initFont(struct App* app, int fontType, char *filename);
static void initFonts(struct App* app);
static SDL_Texture* toTexture(struct App* app, SDL_Surface *surface, int destroySurface);
static void drawTextLine(struct App* app, char* text, int x, int y, int r, int g, int b, int fontType,
                  int align);
static int drawTextWrapped(struct App* app, char *text, int x, int y, int r, int g, int b,
                           int fontType, int align, int maxWidth, int doDraw);

static void calcTextDimensions(char *text, int fontType, int *w, int *h);

static int getWrappedTextHeight(struct App* app, char *text, int fontType, int maxWidth);

static void calcTextDimensions(char *text, int fontType, int *w, int *h)
{
	int i, character;
	SDL_Rect *g;

	*w = *h = 0;

	i = 0;

	character = text[i++];

	while (character)
	{
		g = &glyphs[fontType][character];

		*w += g->w;
		*h = g->h > *h ? g->h : *h;//MAX(g->h, *h);

		character = text[i++];
	}
}

static void initFont(struct App* app, int fontType, char *filename)
{
	SDL_Color white; white.a = 255; white.b = 255; white.g = 255; white.r = 255;
	SDL_Surface *surface, *text;
	SDL_Rect dest;
	int i;
	char c[2];
	SDL_Rect *g;

	memset(&glyphs[fontType], 0, sizeof(SDL_Rect) * NUM_GLYPHS);

	fonts[fontType] = TTF_OpenFont(filename, FONT_SIZE);

	surface = SDL_CreateRGBSurface(0, FONT_TEXTURE_SIZE, FONT_TEXTURE_SIZE, 32, 0, 0, 0, 0xff);

	SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGBA(surface->format, 0, 0, 0, 0));

	dest.x = dest.y = 0;

	for (i = ' ' ; i <= 'z' ; i++)
	{
		c[0] = i;
		c[1] = 0;

		text = TTF_RenderUTF8_Blended(fonts[fontType], c, white);

		TTF_SizeText(fonts[fontType], c, &dest.w, &dest.h);

		if (dest.x + dest.w >= FONT_TEXTURE_SIZE)
		{
			dest.x = 0;

			dest.y += dest.h + 1;

			if (dest.y + dest.h >= FONT_TEXTURE_SIZE)
			{
				SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_CRITICAL, "Out of glyph space in %dx%d font atlas texture map.", FONT_TEXTURE_SIZE, FONT_TEXTURE_SIZE);
				exit(1);
			}
		}

		SDL_BlitSurface(text, NULL, surface, &dest);

		g = &glyphs[fontType][i];

		g->x = dest.x;
		g->y = dest.y;
		g->w = dest.w;
		g->h = dest.h;

		SDL_FreeSurface(text);

		dest.x += dest.w;
	}

	fontTextures[fontType] = toTexture(app, surface, 1);
}

static void initFonts(struct App* app)
{
	initFont(app, FONT_BASIC, "../fonts/VeraMono.ttf");
	initFont(app, FONT_BASIC_ITALIC, "../fonts/VeraMono-Italic.ttf");
}

static SDL_Texture* toTexture(struct App* app, SDL_Surface *surface, int destroySurface)
{
	SDL_Texture *texture;

	texture = SDL_CreateTextureFromSurface(app->renderer, surface);

	if (destroySurface)
	{
		SDL_FreeSurface(surface);
	}

	return texture;
}

static int getWrappedTextHeight(struct App* app, char *text, int fontType, int maxWidth)
{
	return drawTextWrapped(app, text, 0, 0, 255, 255, 255, fontType, TEXT_ALIGN_LEFT, maxWidth, 0);
}

void Text_init(struct App* app)
{
    if (TTF_Init() < 0)
    {
	    printf("Couldn't initialize SDL TTF: %s\n", SDL_GetError());
	    exit(1);
    }

	initFonts(app);
}

void Text_quit()
{
    TTF_Quit();
}

static int drawTextWrapped(struct App* app, char *text, int x, int y, int r, int g, int b,
                           int fontType, int align, int maxWidth, int doDraw)
{
	char word[MAX_WORD_LENGTH], line[MAX_LINE_LENGTH];
	int i, n, wordWidth, lineWidth, character, len;

	i = 0;

	memset(word, 0, MAX_WORD_LENGTH);
	memset(line, 0, MAX_LINE_LENGTH);

	character = text[i++];

	n = 0;

	lineWidth = wordWidth = 0;

	len = strlen(text);

	while (character)
	{
		wordWidth += glyphs[fontType][character].w;

		if (character != ' ')
		{
			word[n++] = character;
		}

		if (character == ' ' || i == len)
		{
			if (lineWidth + wordWidth >= maxWidth)
			{
				if (doDraw)
				{
					drawTextLine(app, line, x, y, r, g, b, fontType, align);
				}

				memset(line, 0, MAX_LINE_LENGTH);

				y += glyphs[fontType][' '].h;

				lineWidth = 0;
			}
			else if (lineWidth != 0)
			{
				strcat(line, " ");
			}

			strcat(line, word);

			lineWidth += wordWidth;

			memset(word, 0, MAX_WORD_LENGTH);

			wordWidth = 0;

			n = 0;
		}

		character = text[i++];
	}

	if (doDraw)
	{
		drawTextLine(app, line, x, y, r, g, b, fontType, align);
	}

	return y + glyphs[fontType][' '].h;
}

void drawText(struct App* app, char *text, int x, int y, int r, int g, int b, int fontType,
              int align, int maxWidth)
{
	if (maxWidth > 0)
	{
		drawTextWrapped(app, text, x, y, r, g, b, fontType, align, maxWidth, 1);
	}
	else
	{
		drawTextLine(app, text, x, y, r, g, b, fontType, align);
	}
}

static void drawTextLine(struct App* app, char* text, int x, int y, int r, int g, int b, int fontType,
                  int align)
{
	int i, character, w, h;
	SDL_Rect *glyph, dest;

	if (align != TEXT_ALIGN_LEFT)
	{
		calcTextDimensions(text, fontType, &w, &h);

		if (align == TEXT_ALIGN_CENTER)
		{
			x -= (w / 2);
		}
		else if (align == TEXT_ALIGN_RIGHT)
		{
			x -= w;
		}
	}

	SDL_SetTextureColorMod(fontTextures[fontType], r, g, b);

	i = 0;

	character = text[i++];

	while (character)
	{
		glyph = &glyphs[fontType][character];

		dest.x = x;
		dest.y = y;
		dest.w = glyph->w;
		dest.h = glyph->h;

		SDL_RenderCopy(app->renderer, fontTextures[fontType], glyph, &dest);

		x += glyph->w;

		character = text[i++];
	}
}


// SDL_Texture *getTextTexture(struct App* app, char *text)
// {
// 	SDL_Surface *surface;
//     SDL_Color white; white.a = 255; white.b = 255; white.g = 255; white.r = 100;

// 	surface = TTF_RenderUTF8_Blended(font, text, white);

// 	return toTexture(app, surface, 1);
// }

// void App_blit(struct App* self, SDL_Texture* texture)
// {
//     SDL_Rect rect;
//     rect.x = 50;
//     rect.y = 50;
//     rect.w = 200;
//     rect.h = 200;
// 	SDL_QueryTexture(texture, NULL, NULL, NULL, NULL);
// 	SDL_RenderCopyEx(self->renderer, texture, NULL, &rect, 0.0, NULL,
//     SDL_FLIP_NONE);
// }