#include "token_scanner.hh"

token_scanner::token_scanner()
{
}

token_scanner::token_scanner(token_scanner* successor_scanner) :
    successor_scanner(successor_scanner)
{
}

void token_scanner::previous(const token& previously_scanned_token)
{
    this->previously_scanned_token = previously_scanned_token;
}

/// Get this scanner's successor in the scanners chain.
///
/// The successor of this scanner will be the scanner that will attempt to
/// recognize a token if this scanner can't recognize the token.
///
/// \return The successor of this scanner if there is one;  a null pointer
/// otherwise.
token_scanner* token_scanner::successor() const
{
    return successor_scanner.get();
}

/// Set this scanner's successor in the scanners chain.
///
/// The successor of this scanner will be the scanner that will attempt to
/// recognize a token if this scanner can't recognize the token.
///
/// \param successor_scanner The new successor of this scanner.
void token_scanner::successor(token_scanner* successor_scanner)
{
    this->successor_scanner.reset(successor_scanner);
}

/// Scan for a single token from a YAML input stream.
///
/// Do not override this function;  it is a helper function on which concrete
/// scanners rely.  Specifically, it provides the engine for forwarding
/// requests down the scanners chain.  If a scanner doesn't recognize the
/// upcoming characters and therefore can't resolve them into a token it calls
/// this function to have the request forwarded to the next scanner in the
/// chain.
///
/// \return true a token was scanned, false if not concrete scanner could
/// recognize the upcoming characters as a token.
bool token_scanner::scan()
{
    if( recognize() )
        return true;
    else if( successor() )
        return successor()->scan(); // non-virtual;  recurse on another object
    return false;
}
