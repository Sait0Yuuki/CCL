#include <string>
#include <tuple>

using std::string;
using std::tuple;

// A ParserResult contains a string representation of the input and the
// number of characters processed.
typedef tuple<string, int> ParserResult;

// Return a string representation of the input RESP string.
string ParseRESP(string);

// Parse a RESP Simple String (starting with '+').
ParserResult ParseSimpleString(string);

// Parse a RESP Error (starting with '-').
ParserResult ParseError(string);

// Parse a RESP Integer (starting with ':').
ParserResult ParseInteger(string);

// Parse a RESP Bulk String (starting with '$').
ParserResult ParseBulkString(string);

// Parse a RESP Array (starting with '*').
ParserResult ParseArray(string);
