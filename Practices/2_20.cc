#include <cassert>
#include <cctype>
#include <deque>
#include <fstream>
#include <iostream>

using namespace std;


// Convert C comments to upper case.
// A C file is read from "is",
// And output to "os"
//
// For state transitions, run 2_20_DFA.py
//
// NB: This function requires "noskipws" is set for istream "is"
//
void cCommentToUpper( istream &is, ostream &os) {
    typedef enum {
        STATE_1 = 0,    // Start state
        STATE_2,
        STATE_3,
        STATE_4,
        STATE_5         // Finish state
    } State;

    // We need a buffer to store chars when going through the DFA.
    // When state 5 is hit, all chars in the buffer are outputed to upper case.
    // If other state is hit, all chars in the buffer are outputed unchanged.
    deque< char> buf;

    char c;

    while ( is >> c) {
        assert( buf.empty());
        buf.push_back( c);

        State state = STATE_1;
        bool mayInComment = true;

        while ( state < STATE_5) {
            bool advanceInput = false;

            switch ( state) {
                case STATE_1:
                    if ( c == '/') {
                        // 1 -> 2
                        state = STATE_2;
                        advanceInput = true;
                    }
                    else {
                        mayInComment = false;
                    }
                    break;

                case STATE_2:
                    if ( c == '*') {
                        // 2 -> 3
                        state = STATE_3;
                        advanceInput = true;
                    }
                    else {
                        mayInComment = false;
                    }
                    break;

                case STATE_3:
                    if ( c == '*') {
                        // 3 -> 4
                        state = STATE_4;
                        advanceInput = true;
                    }
                    else {
                        // 3 -> 3
                        // Do not change state, just input another char
                        advanceInput = true;
                    }
                    break;

                case STATE_4:
                    if ( c == '*') {
                        // 4 -> 4
                        // Do not change state, just input another char
                        advanceInput = true;
                    }
                    else if ( c == '/') {
                        // 4 -> 5
                        state = STATE_5;
                    }
                    else {
                        // 4 -> 3
                        state = STATE_3;
                        advanceInput = true;
                    }
                    break;

                default:
                    assert( 0 && "Never be here");
                    break;
            } // switch ( state)

            // Check if we are at outside of C comment
            if ( !mayInComment) {
                // break while ( state < STATE_5)
                break;
            }

            // Check if finish state is hit
            if ( state == STATE_5) {
                // finish state hit, break while ( state < STATE_5)
                assert( advanceInput == false);
                break;
            }

            if ( advanceInput) {
                if ( is >> c) {
                    buf.push_back( c);
                }
                else {
                    // No chars available, break while ( state < STATE_5)
                    break;
                }
            }
        }   // while ( state < STATE_5)

        // output contents in the buffer
        for ( deque< char>::const_iterator it = buf.begin(); it != buf.end(); ++it) {
            const char oc = *it;
            os << static_cast<char>( state == STATE_5 ? ::toupper( oc) : oc);
        }
        buf.clear();
    }   // End while ( is >> c)
}   // End cCommentToUpper

                


// Input is a C source file,
// And output to is standard out.
int main( int argc, char *argv[]) {
    if ( argc != 2) {
        cerr << "Usage: " << argv[ 0] << " <C file>" << endl;
        return 1;
    }

    // Use binary mode to open the file, because
    // we do not want to lose any information.
    ifstream cf( argv[1], ios_base::in | ios_base::binary);
    if ( !cf) {
        cerr << "Failed to open file \"" << argv[ 1] << "\"" << endl;
    }

    cf >> noskipws;
    cCommentToUpper( cf, cout);
    return 0;
}

