#include "token_scanner.hh"

void token_scanner::previous(const token& previously_scanned_token)
{
    this->previously_scanned_token = previously_scanned_token;
}
