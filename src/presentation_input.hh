#ifndef SRC_PRESENTATION_INPUT_HH_
#define SRC_PRESENTATION_INPUT_HH_

#include <iosfwd>

class presentation_input {
public:
    presentation_input();
    presentation_input(std::istream& is);
    ~presentation_input();

    void sip();
    char peek() const;
    void putback(char ch);

private:
    struct impl;
    impl* pimpl;
};

#endif // SRC_PRESENTATION_INPUT_HH_
