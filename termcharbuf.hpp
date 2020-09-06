#ifndef __TERMCHARBUFF_HPP__
#define __TERMCHARBUFF_HPP__

#include <iostream>
#include <termios.h>

#define KEY_TAB    (0x09)
#define KEY_BK     (0x7f)
#define KEY_ENTER  (0x0a)
#define KEY_ESC    (0x1b)
#define KEY_UP     (0x41)
#define KEY_DOWN   (0x42)
#define KEY_RIGHT  (0x43)
#define KEY_LEFT   (0x44)
#define CLR_LINE  "\r\e[K" // i.e.: \r + clr_eol

// key sequences
const char keyDown[]  {0x1B, 0x5B, 0x42};
const char keyLeft[]  {0x1B, 0x5B, 0x44};
const char keyRight[] {0x1B, 0x5B, 0x43};
const char keyUp[]    {0x1B, 0x5B, 0x41};

// callback function to handle up, down, etc keys
typedef void (*KeyPressedCallback)(std::string&, int, void*);

static struct termios TermSaved;

// Set terminal to single character mode
bool terminalCharBufInit() {
	struct termios termCurrent;
	tcgetattr(fileno(stdin), &termCurrent);
	TermSaved = termCurrent;
	termCurrent.c_lflag &= (~ICANON & ~ECHO);
	termCurrent.c_cc[VTIME] = 0;
	termCurrent.c_cc[VMIN] = 1;

	return (tcsetattr(fileno(stdin), TCSANOW, &termCurrent) == 0);
}

// restore saved settings
bool terminalRestore() {
	return (tcsetattr(fileno(stdin), TCSANOW, &TermSaved) == 0);
}

// Read single characters from cin while checking for arrow keys sequences
std::string wsdGetline(KeyPressedCallback cb, void* pPassThrough) {
	const char DelBuf[] = "\b \b";
	std::string res;
	res.reserve(50);
	std::streambuf *pbuf = std::cin.rdbuf();

	char c;
	bool esc = false;
	bool enter = false;

	// lambda for convenience:
	auto handleArrowKeys = [pPassThrough, &cb, &esc, &res] (int key) {
		std::string strValue = res;
		esc = false;
		std::cout << CLR_LINE << std::flush;
		cb(strValue, key, pPassThrough);
		res = strValue;
		std::cout << strValue;
	};

	// res holds the string until enter key is pressed
	while (!enter && ((c = pbuf->sbumpc()) != EOF)) {
		switch (c) {
			case KEY_TAB:
				handleArrowKeys(KEY_TAB);
				break;
			case KEY_BK:
				if (res.size()) {
					std::cout << DelBuf;
					res.pop_back();
				}
				break;
			case KEY_ENTER:
				enter = true;
				std::cout << std::endl;
				break;
			case KEY_ESC:
				esc = true;
				res += c; // will be reset when the sequence complete
				break;
			default:
				if (!esc) std::cout << c;
				res += c;
				// check key sequencies
				if (res.size() > 2) {
					if (std::equal(res.end()-3, res.end(), keyUp)) {
						handleArrowKeys(KEY_UP);
					}
					else
					if (std::equal(res.end()-3, res.end(), keyDown)) {
						handleArrowKeys(KEY_DOWN);
					}
					else
					if (std::equal(res.end()-3, res.end(), keyLeft)) {
						esc = false;
						res.erase(res.end()-3, res.end());
						// std::cout << keyLeft;
					}
					else
					if (std::equal(res.end()-3, res.end(), keyRight)) {
						esc = false;
						res.erase(res.end()-3, res.end());
						// std::cout << keyRight;
					}
				}
				break;
		}
	}

	return res;
}

#endif