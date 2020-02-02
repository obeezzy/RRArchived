#ifndef INDETERMINATEDESTINATIONURLEXCEPTION_H
#define INDETERMINATEDESTINATIONURLEXCEPTION_H

#include "network/networkexception.h"

class IndeterminateDestinationUrlException : public NetworkException
{
public:
    explicit IndeterminateDestinationUrlException(const QString &command);
};

#endif // INDETERMINATEDESTINATIONURLEXCEPTION_H
