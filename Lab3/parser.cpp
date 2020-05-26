#include "parser.h"

using std::get;
using std::make_tuple;

// Return the number of elements in an Array or Bulk String type.
static int GetLength(string);

// Parse the item according to its type.
static ParserResult ParseItem(string);

// Return a ParserResult containing the contents of the input up to the
// first "\r\n" (skipping the type specifier).
static ParserResult ParseString(string);


static int GetLength(string input) {
	int len = 0;
	auto p = input.cbegin();

	// Skip the '*' or '$'.
    p++;

    if (*p == '-') {
    	return -1;
    }

    while (*p != '\r') {
        len = (len * 10) + (*p - '0');
        p++;
    }
    return len;
}

static ParserResult ParseItem(string input) {
	ParserResult result;
	const char type = input[0];

	switch (type) {
		case '+':
			result = ParseSimpleString(input);
			break;
		case '-':
			result = ParseError(input);
			break;
		case ':':
			result = ParseInteger(input);
			break;
		case '$':
			result = ParseBulkString(input);
			break;
		case '*':
			result = ParseArray(input);
			break;
		default:
			result = make_tuple("", 0);
			break;
	}
	return result;
}

static ParserResult ParseString(string input) {
	int len = 0;
	auto p = input.cbegin();

	// Skip the type specifier.
	p++;
	while (*p != '\r') {
		len++;
		p++;
	}
	// Add 3 to len for the type specifier and the trailing "\r\n".
	return make_tuple(input.substr(1, len), len + 3);
}

string ParseRESP(string input) {
	return get<0>(ParseItem(input));
}

ParserResult ParseSimpleString(string input) {
	auto result = ParseString(input);
	return make_tuple('"' + get<0>(result) + '"', get<1>(result));
}

ParserResult ParseError(string input) {
	auto result = ParseSimpleString(input);
	return make_tuple("Error:" + get<0>(result), get<1>(result));
}

ParserResult ParseInteger(string input) {
	return ParseString(input);
}

ParserResult ParseBulkString(string input) {
	const int len = GetLength(input);
	if (len == -1) {
		// Null Bulk Strings always look like "$-1\r\n" so count is 5.
		return make_tuple("Null", 5);
	}

	const int start = input.find('\r') + 2;
	return make_tuple('"' + input.substr(start, len) + '"', start + len + 2);
}

ParserResult ParseArray(string input) {
	// count is the number of characters we have processed.
	int count = 0;
	string result = "[";

	const int len = GetLength(input);
	if (len == -1) {
		// Null arrays always look like "*-1\r\n", so count is 5.
		return make_tuple("Null", 5);
	}

	const int start = input.find('\r') + 2;
	input = &input[start];
	count += start;

    for (int i = 0; i < len; i++) {
    	const auto item = ParseItem(input);
 		const int cur_count = get<1>(item);

    	result += get<0>(item);
    	if (i < len - 1) {
    		result += ", ";
    	}

    	input = &input[cur_count];
    	count += cur_count;
    }

    return make_tuple(result + "]", count);
}
