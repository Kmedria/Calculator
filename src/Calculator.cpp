#include <iostream>

// text calculator
// can calculate any sum that uses floating or whole numbers. 
// Floating numbers must use '.'
// So it would like 1.1
// The Calculator can process * as multiply and / as divide.
// The Calculator can tell where your equation has a mistake (somewhere where the calculator detects a char that it does not recognize), but the user must figure out the mistake at given location
// The calculator understands brackets and is able to process them follwing BODMASS.
// The calculator can process sin, cos and tan. It requires the value to be in a bracket like sin(x) and x is assumed to be in radians.

// Trig equations are caluculated using Maclaurin series approximation. The accuracy is determined by the trigAccuracy variable.

int getInput(char* text, int maxSize);
double solve(char* text, int size, bool* valid);
bool findText(char* source, int sourceSize, const char* search, int searchTextSize);
int pos(char* source, int sourceSize, const char* search, int searchTextSize);

int trigAccuracy = 10;

float fac(int input) {
	if (input < 0) {
		return -1;
	}
	if (input == 0) {
		return 1;
	}
	float val = 1;
	for (int index = 1; index < input+1; index++) {
		val *= index;
	}
	return val;
}

// calculated using Maclaurin series approximation, up to trigAccuracy number of terms.
float customSinFunc(float input) {
	float val = 0;
	int sign = 1;
	for (int index = 1; index < trigAccuracy*2; index += 2) {
		val += sign * (pow(input, index))/fac(index);
		sign *= -1;
	}
	return val;
}

// calculated using Maclaurin series approximation, up to trigAccuracy number of terms.
float customCosFunc(float input) {
	float val = 0;
	int sign = 1;
	for (int index = 0; index < trigAccuracy * 2; index += 2) {
		val += sign * (pow(input, index)) / fac(index);
		sign *= -1;
	}
	return val;
}

// tan is calculated from sin/cos

int main() {

	const char* promptText = "Type the equation to solve and hit enter (max 127 characters).\nType in <help> for help on how to use the calculator\nType in <exit> to close the program";

	std::cout << "Starting the text caluculator\n" << std::endl << promptText << std::endl;

	int maxInputTextSize = 128; // maximum char limit for input

	char* input = new char[maxInputTextSize*2];

	int counter = 0;
	for (counter = 0; counter < maxInputTextSize*2; counter++) {
		input[counter] = '\0';
	}
	
	int inputSize = getInput(input, maxInputTextSize);

	int exitCodeSize = 4;
	char* exitCode = new char[exitCodeSize];
	exitCode[0] = 'e';
	exitCode[1] = 'x';
	exitCode[2] = 'i';
	exitCode[3] = 't';

	int helpCodeSize = 4;
	char* helpCode = new char[helpCodeSize];
	helpCode[0] = 'h';
	helpCode[1] = 'e';
	helpCode[2] = 'l';
	helpCode[3] = 'p';

	while (!findText(input, inputSize, exitCode, exitCodeSize)) {

		if (findText(input, inputSize, helpCode, helpCodeSize)) {
			std::cout << "\n Help code Entered. Enter a number + or - or * or / another number." <<
				"You can enter how ever many numbers you want as long as you don't have more than 128 chars." << std::endl;
		}
		else {
			bool* valid = new bool(true);
			double ans = solve(input, inputSize, valid);
			if (*valid) {
				std::cout << ans << std::endl;
			}
			else {
				std::cout << "The input Equation is invalid" << std::endl;
			}

			std::cout << "\n" << promptText << std::endl;
			delete valid;
		}
		
		for (int counter = 0; counter < maxInputTextSize * 2; counter++) {
			input[counter] = '\0';
		}

		std::cin.clear();
		std::cin.ignore(10000, '\n');

		inputSize = getInput(input, maxInputTextSize);

	}

	std::cout << "Exit code found. terminating.";

	delete[] input;
	delete[] exitCode;
	delete[] helpCode;

	return 0;
}

bool findText(char* source, const char* search, int searchTextSize, int start, int end) {
	int searchCounter = 0;
	for (int sourceCounter = start; sourceCounter < end; sourceCounter++) {
		if (source[sourceCounter] == search[searchCounter]) {
			searchCounter++;
		}
		else {
			searchCounter = 0;
		}
		if (searchCounter == searchTextSize) {
			return true;
		}
	}
	return false;
}

bool findText(char* source, int sourceSize, const char* search, int searchTextSize) {
	return findText(source, search, searchTextSize, 0, sourceSize);
}

int pos(char* source, const char* search, int searchTextSize, int start, int end) {
	int searchCounter = 0;
	for (int sourceCounter = start; sourceCounter < end; sourceCounter++) {
		if (source[sourceCounter] == search[searchCounter]) {
			searchCounter++;
		}
		else {
			searchCounter = 0;
		}
		if (searchCounter == searchTextSize) {
			return sourceCounter;
		}
	}
	return -1;
}

int pos(char* source, int sourceSize, const char* search, int searchTextSize) {
	return findText(source, search, searchTextSize, 0, sourceSize);
}

bool isDigit(char input) {
	return 47 < input && 58 > input;
}

double solve(char* text, int startingPoint, int end, bool* validEqn) {

	if (!*validEqn) {
		return 0;
	}

	if (end < startingPoint) {
		return 0;
	}

	int counter = startingPoint;
	int depth = 0;
	bool keepSearching = true;
	//counter = counter;
	while (keepSearching) {
		if (depth < 0) {
			*validEqn = false;
			std::cerr << " You may not have more ) brackets than (. There must one ( bracket that come before for each )." << std::endl;
			return 0;
		}
		if ('(' == text[counter]) {
			depth++;
		}
		if (')' == text[counter]) {
			depth--;
		}
		if (0 == depth && '+' == text[counter] && counter < end) {
			keepSearching = false;
		} else {
			counter++;
		}
		if (!(counter < end)) {
			keepSearching = false;
		}
	}

	if (depth != 0) {
		*validEqn = false;
		std::cerr << "The equation does not have an equal number of brackets.";
		if (depth > 0) {
			std::cerr << " There are too many ( brackets." << std::endl;
			return 0;
		}
		std::cerr << " There are too many ) brackets." << std::endl;
		return 0;
	}

	if (counter < end ) {
		return solve(text, startingPoint, counter, validEqn) + solve(text, counter + 1, end, validEqn);
	}
	else {

		if (')' == text[startingPoint]) {
			*validEqn = false;
			std::cerr << " You may not start with a ) bracket. There must be a ( bracket that comes before it." << std::endl;
			return 0;
		}

		if ('(' == text[startingPoint]) {
			depth = 1;
			int count = startingPoint+1;
			while (depth > 0) {
				if ('(' == text[count]) {
					depth++;
				}
				if (')' == text[count]) {
					depth--;
				}
				if (0 != depth) {
					count++;
				}
			}
			if (count + 1 < end) {
				if ('*' == text[count + 1]) {
					return solve(text, startingPoint, count+1, validEqn) * solve(text, count + 2, end, validEqn);
				} else if ('/' == text[count + 1]) {

					float denom = solve(text, count + 2, end, validEqn);
					if (0 == denom) {
						*validEqn = false;
						std::cerr << "Division by 0 has occured." << std::endl;
						return 0;
					}
					return solve(text, startingPoint, count+1, validEqn) / denom;
				} else return solve(text, startingPoint, count+1, validEqn) + solve(text, count + 2, end, validEqn);

			} else return solve(text, startingPoint + 1, count, validEqn);
		} else {

			char* tempChar = new char[1];
			tempChar[0] = '*';

			if (findText(text, tempChar, 1, startingPoint, end)) {
				int temp = pos(text, tempChar, 1, startingPoint, end);
				delete[] tempChar;
				return solve(text, startingPoint, temp, validEqn) * solve(text, temp + 1, end, validEqn);
			}
			
			tempChar[0] = '/';

			if (findText(text, tempChar, 1, startingPoint, end)) {
				int temp = pos(text, tempChar, 1, startingPoint, end);
				delete[] tempChar;
				float denom = solve(text, temp + 1, end, validEqn);
				if (0 == denom) {
					*validEqn = false;
					std::cerr << "Division by 0 has occured." << std::endl;
					return 0;
				}
				return solve(text, startingPoint, temp, validEqn) / denom;
			}

			float value = 0;
			int sign = 1;
			while ('-' == text[startingPoint]) {
				sign = -sign;
				startingPoint++;
			}

			if ('(' == text[startingPoint]) {
				delete[] tempChar;
				return sign*solve(text, startingPoint+1, end-1, validEqn);
			}

			tempChar[0] = '.';

			int positionOfDot = pos(text, tempChar, 1, startingPoint, end);

			delete[] tempChar;

			char* sinChar = new char[3];
			sinChar[0] = 's';
			sinChar[1] = 'i';
			sinChar[2] = 'n';

			char* cosChar = new char[3];
			cosChar[0] = 'c';
			cosChar[1] = 'o';
			cosChar[2] = 's';

			char* tanChar = new char[3];
			tanChar[0] = 't';
			tanChar[1] = 'a';
			tanChar[2] = 'n';

			if (findText(text, sinChar, 3, startingPoint, end)) {
				float val = solve(text, startingPoint + 3, end, validEqn);
				delete[] sinChar;
				delete[] cosChar;
				delete[] tanChar;
				return sign * customSinFunc(val);
			}

			if (findText(text, cosChar, 3, startingPoint, end)) {
				float val = solve(text, startingPoint + 3, end, validEqn);
				delete[] sinChar;
				delete[] cosChar;
				delete[] tanChar;
				return sign * customCosFunc(val);
			}

			if (findText(text, tanChar, 3, startingPoint, end)) {
				float val = solve(text, startingPoint + 3, end, validEqn);
				float valNum = customSinFunc(val);
				float valDenum = customCosFunc(val);
				delete[] sinChar;
				delete[] cosChar;
				delete[] tanChar;
				if (0 == valDenum) {
					*validEqn = false;
					std::cerr << "Division by 0 has occured when calculating tan(" << val << "). " << std::endl;
					return 0;
				}
				return sign * valNum/valDenum;
			}

			delete[] sinChar;
			delete[] cosChar;
			delete[] tanChar;

			if (positionOfDot>0) {
				float fractionPart = solve(text, positionOfDot+1, end, validEqn);
				float wholePart = solve(text, startingPoint, positionOfDot, validEqn);
				while (fractionPart > 1) {
					fractionPart /= 10;
				}
				return sign * (wholePart + fractionPart);
			}

			for (int index = startingPoint; index < end; index++) {
				if (!isDigit(text[index])) {

					std::cerr << std::endl << text << std::endl;

					for (int count = 0; count < index; count++) {
						std::cerr << "-";
					}
					std::cerr << "^\n";
					std::cerr << "A non digit was entered where it was not supposed to be at character position " << index << std::endl;
					*validEqn = false;
				} 
				value *= 10;
				value += text[index]-48;
			}
			return value*sign;
		}
	}
}

double solve(char* text, int size, bool* validEqn) {
	return solve(text, 0, size, validEqn);
}

int getInput(char* text, int maxSize) {
	int size = 0;
	while (10 != std::cin.peek()) {

		if ('-' == std::cin.peek() && (!(0 == size) && !('(' == text[size-1]) && !('-' == text[size-1 ]))) {
			text[size] = '+';
			size++;
		} else if ('(' == std::cin.peek() && size > 0 && isDigit(text[size - 1])) {
			text[size] = '*';
			size++;
		} else if (isDigit(std::cin.peek()) && size > 0 && ')' == text[size - 1]) {
			text[size] = '*';
			size++;
		} else if ('s' == std::cin.peek() && size > 0 && ((isDigit(text[size - 1]) || ')' == text[size - 1]))) {
			text[size] = '*';
			size++;
		} else if ('c' == std::cin.peek() && size > 0 && (isDigit(text[size - 1]) || ')' == text[size - 1])) {
			text[size] = '*';
			size++;
		} else if ('t' == std::cin.peek() && size > 0 && (isDigit(text[size - 1]) || ')' == text[size - 1])) {
			text[size] = '*';
			size++;
		}

		text[size] = std::cin.get();
		if (32 != text[size]) {
			size++;
			if (size == maxSize) {
				return size;
			}
		}
	}
	return size;
}
