#include "uiFormattableText.h"

#include "Game.h"
#include "stringHelpers.h"
#include "maths.h"

#include "resourceFont.h"
#include "resourceTexture.h"

#include "viewUi.h"

#include "inputKeyboard.h"

using namespace mage;

// magicText.
uiFormattableText::uiFormattableText(float x, float y, std::string textIn)
	: uiBasic(x, y, textureData(nullptr))
{
	text = textIn;
	lineLimit = -1;
	opacity = 255;
	alignment = LEFT;
	lineSpacing = 18.f;

	defaultFont = theGame()->resources->getAs<resourceFont>("__fontDef");
	defaultAnimationMode = NONE;
	defaultColorMode = SOLID;
	defaultColor = sf::Color::White;
}

void uiFormattableText::update(sf::Time elapsed)
{
	// ...
}

void uiFormattableText::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	int cOffset = 0;
	int lOffset = 0;
	unsigned int size = 16;

	animationMode curAnimMode = defaultAnimationMode;
	colorMode curColorMode = defaultColorMode;
	alignmentMode curAlignment = alignment;
	sf::Color curColor = defaultColor;

	curColor.a = opacity;

	std::shared_ptr<resourceFont> f = defaultFont;

	std::vector<std::shared_ptr<resourceTexture>> imagesToDraw;

	for (unsigned int i = 0; i < text.size(); i++) {
		int xOffset = 0;
		int yOffset = 0;
		int sizeOffset = 0;
		float rot = 0.f;

		sf::Color colorOffset = sf::Color::Transparent;

		std::string charString = { text[i] };

		float halfTextLength = findSizeX() / 2.f;

		// control string?
		if (charString == "[") {
			std::string commandString;
			for (unsigned int j = i + 1; j < text.size(); j++) {
				std::string charString2 = { text[j] };

				if (charString2 != "]")
					commandString += charString2;
				else {
					i = j;
					break;
				}
			}

			// read the string code
			std::vector<std::string> codeSplit = splitString(commandString, ':');

			if (codeSplit.size() == 2) {
				std::string op = codeSplit[0];
				std::string per = codeSplit[1];

				if (op == "c" || op == "color") { // hex color code/color mode
					if (per == "solid")
						curColorMode = SOLID;
					else if (per == "blink")
						curColorMode = BLINK;
					else if (per == "rainbow")
						curColorMode = RAINBOW;
					else if (per == "clear" || per == "cl") {
						curColorMode = SOLID;
						curColor = sf::Color::White;
					}
					else if (per.size() == 6) {
						// convert hex code to color
						unsigned int hexValue = std::stoul(per, nullptr, 16);

						sf::Color rgbColor;

						rgbColor.r = ((hexValue >> 16) & 0xFF);  // Extract the RR byte
						rgbColor.g = ((hexValue >> 8) & 0xFF);   // Extract the GG byte
						rgbColor.b = ((hexValue) & 0xFF);        // Extract the BB byte
						rgbColor.a = opacity;

						curColor = rgbColor;
					}
				}
				else if (op == "a" || op == "anim") { // animation name
					if (per == "none") {
						curAnimMode = NONE;
					}
					else if (per == "jitter") {
						curAnimMode = JITTER;
					}
					else if (per == "sleepy") {
						curAnimMode = SLEEPY;
					}
					else if (per == "scooby") {
						curAnimMode = SCOOBY;
					}
					else if (per == "bulge") {
						curAnimMode = BULGE;
					}
				}
				else if (op == "n" || op == "newline") {
					newLine(target, states, i + 1, curAnimMode, curColorMode, size, curColor, f, atoi(per.c_str()));

					break;
				}
				else if (op == "s" || op == "size")
					size = atoi(per.c_str());
				else if (op == "i" || op == "image") {
					imagesToDraw = { theGame()->resources->getAs<resourceTexture>(per) };
				}
				else if (op == "b" || op == "bind") {
					imagesToDraw = theGame()->getKeybindTextures(per);
				}
				else if (op == "f" || op == "font") {
					if (theGame()->resources->exists(per)) {
						f = theGame()->resources->getAs<resourceFont>(per);
					}
					else {
						f = theGame()->resources->getAs<resourceFont>("__fontDef");
					}
				}
			}

			continue;
		}

		if (curAnimMode == JITTER) {
			yOffset = randomInt(-1, 1);
		}
		else if (curAnimMode == SLEEPY) {
			yOffset = (int)floor(sin(theGame()->getSimTime().asSeconds() + cOffset) * 2.f);
		}
		else if (curAnimMode == SCOOBY) {
			yOffset = (int)floor(sin((theGame()->getSimTime().asSeconds() * 10) + (cOffset / 10)) * 4);
		}
		else if (curAnimMode == BULGE) {
			sizeOffset = (int)(sin(theGame()->getSimTime().asSeconds() + cOffset) * 4);
		}

		if (curColorMode == BLINK) {
			colorOffset.a = (sf::Uint8)floor(254.f * abs(sin(theGame()->getSimTime().asSeconds() * 10.f)));
		}
		else if (curColorMode == RAINBOW) {
			colorOffset.r = (sf::Uint8)floor(254.f * abs(sin(theGame()->getSimTime().asSeconds() + cOffset)));
			colorOffset.g = (sf::Uint8)floor(254.f * abs(sin(theGame()->getSimTime().asSeconds() + cOffset + 32.f)));
			colorOffset.b = (sf::Uint8)floor(254.f * abs(sin(theGame()->getSimTime().asSeconds() + cOffset + 64.f)));
		}

		auto charOffsetX = (float)cOffset + (float)xOffset;
		auto charOffsetY = (float)yOffset + (float)lOffset;

		if (curAlignment == LEFT) {
			// standard image drawing
			for (unsigned int img = 0; img < imagesToDraw.size(); img++) {
				sf::Sprite image(*imagesToDraw[img]->get());
				image.setPosition(getPosition() + sf::Vector2f(charOffsetX, charOffsetY - (image.getGlobalBounds().height / 2.f)));

				target.draw(image);

				cOffset += image.getGlobalBounds().width;

				charOffsetX = (float)cOffset + (float)xOffset;
			}

			// standard character drawing
			cOffset += drawInfo(target, states, getPosition() + sf::Vector2f(charOffsetX, charOffsetY), charString, curColor - colorOffset, false, size + sizeOffset, rot, f);
			cOffset += 1;
		}
		else {
			float centeredX = -halfTextLength + charOffsetX;

			// centered images
			for (unsigned int img = 0; img < imagesToDraw.size(); img++) {
				sf::Sprite image(*imagesToDraw[img]->get());
				image.setPosition(getPosition() + sf::Vector2f(centeredX, charOffsetY - (image.getGlobalBounds().height / 2.f)));

				target.draw(image);

				cOffset += image.getGlobalBounds().width;

				charOffsetX = (float)cOffset + (float)xOffset;
				centeredX = -halfTextLength + charOffsetX;
			}

			// centered characters
			cOffset += drawInfo(target, states, getPosition() + sf::Vector2f(centeredX, charOffsetY), charString, curColor - colorOffset, false, size + sizeOffset, rot, f);
			cOffset += 1;
		}

		imagesToDraw.clear();

		if (cOffset > theGame()->uiCamera->getSize().x - 128 && charString == " ") {
			newLine(target, states, i + 1, curAnimMode, curColorMode, size, curColor, f);

			break;
		}

		if (lineLimit != -1) {
			if (cOffset > lineLimit && charString == " ") {
				newLine(target, states, i + 1, curAnimMode, curColorMode, size, curColor, f);

				break;
			}
		}
	}
}

void uiFormattableText::newLine(sf::RenderTarget &target, sf::RenderStates states, unsigned int startPosition, animationMode am, colorMode cm, unsigned int s, sf::Color c, std::shared_ptr<resourceFont> f, int num) const
{
	// cheat here.
	// create MORE MAGICTEXT and draw it
	uiFormattableText mt(*this);
	mt.text = strSub(text, startPosition, text.size());
	mt.setPosition(getPosition() + sf::Vector2f(0.f, lineSpacing * num));

	mt.defaultAnimationMode = am;
	mt.defaultColorMode = cm;
	mt.defaultColor = c;
	mt.defaultFont = f;
	mt.defaultSize = s;

	target.draw(mt, states);
}

std::string uiFormattableText::stopAtScrollPoint(std::string text, unsigned int textPhase)
{
	// a scroll point = an individual character in the text, skipping codepoints
	std::string t;
	for (unsigned int i = 0; i < text.size() && i < textPhase; i++) {
		std::string charString = { text[i] };

		// skip any control string
		if (charString == "[") {
			std::string commandString;
			for (unsigned int j = i; j < text.size(); j++) {
				std::string charString2 = { text[j] };

				commandString += charString2;

				if (charString2 == "]") {
					textPhase += j - i; // add lost space
					i = j;
					break;
				}
			}

			t += commandString;
		}
		else {
			t += charString;
		}
	}

	return t;
}

unsigned int uiFormattableText::highestScrollPoint(std::string text)
{
	// a scroll point = an individual character in the text, skipping codepoints
	int c = 0;
	for (unsigned int i = 0; i < text.size(); i++) {
		std::string charString = { text[i] };

		// skip any control string
		if (charString == "[") {
			std::string commandString;
			for (unsigned int j = i; j < text.size(); j++) {
				std::string charString2 = { text[j] };

				commandString += charString2;

				if (charString2 == "]") {
					i = j;
					break;
				}
			}
		}

		c++;
	}

	return c;
}

float uiFormattableText::findSizeX() const
{
	int cOffset = 0;
	int lOffset = 0;
	float sizeX = 0.f;

	unsigned int size = 16;

	for (unsigned int i = 0; i < text.size(); i++) {
		int xOffset = 0;
		int yOffset = 0;
		int sizeOffset = 0;

		std::string charString = { text[i] };

		// control string?
		if (charString == "[") {
			std::string commandString;
			for (unsigned int j = i + 1; j < text.size(); j++) {
				std::string charString2 = { text[j] };

				if (charString2 != "]")
					commandString += charString2;
				else {
					i = j;
					break;
				}
			}

			// read the string code
			std::vector<std::string> codeSplit = splitString(commandString, ':');

			if (codeSplit.size() == 2) {
				std::string op = codeSplit[0];
				std::string per = codeSplit[1];

				if (op == "n" || op == "newline") {
					break;
				}
				else if (op == "s" || op == "size")
					size = atoi(per.c_str());
				else if (op == "i" || op == "image") {
					float imWidth = 0.f;

					if (theGame()->resources->exists(per)) {
						imWidth = theGame()->resources->getAs<resourceTexture>(per)->get()->getSize().x;
					}

					cOffset += imWidth;
				}
				else if (op == "b" || op == "bind") {
					float imWidth = 0.f;
					auto list = theGame()->keyboard->getBindTextures(per);

					for (unsigned int k = 0; k < list.size(); k++) {
						std::string bnd = list[k];

						if (theGame()->resources->exists(bnd)) {
							imWidth += theGame()->resources->getAs<resourceTexture>(bnd)->get()->getSize().x;
						}
					}

					cOffset += imWidth;
				}
			}

			continue;
		}

		// measure this character
		sf::Text charText(charString, *theGame()->resources->getAs<resourceFont>("__fontDef")->get(), size + sizeOffset);
		charText.setScale(.5f, .5f);

		cOffset += (int)charText.getGlobalBounds().width;
		cOffset += 1;

		if (cOffset > theGame()->uiCamera->getSize().x - 128 && charString == " ") {
			cOffset = 0;
			lOffset += size;
		}

		if (lineLimit != -1) {
			if (cOffset > lineLimit && charString == " ") {
				cOffset = 0;
				lOffset += size;
			}
		}

		if (cOffset > sizeX) {
			sizeX = (float)cOffset;
		}
	}

	return sizeX;
}

using namespace chaiscript;

DeclareScriptingCustom(user_type<uiFormattableText>(), "uiFormattableText");
DeclareScriptingCustom(base_class<uiBasic, uiFormattableText>());
DeclareScriptingBaseClass(basic, uiFormattableText);
DeclareScriptingBaseClass(sf::Transformable, uiFormattableText);
DeclareScriptingCustom(constructor<uiFormattableText(float, float, std::string)>(), "uiFormattableText");
DeclareScriptingCustom(fun<float, uiFormattableText>(&uiFormattableText::findSizeX), "findSizeX");
DeclareScriptingCustom(fun(&uiFormattableText::highestScrollPoint), "highestScrollPoint");
DeclareScriptingCustom(fun(&uiFormattableText::lineLimit), "lineLimit");
DeclareScriptingCustom(fun(&uiFormattableText::stopAtScrollPoint), "stopAtScrollPoint");
DeclareScriptingCustom(fun(&uiFormattableText::text), "text");