#include "indeterminatedestinationurlexception.h"
#include "network/networkerror.h"

IndeterminateDestinationUrlException::IndeterminateDestinationUrlException(
    const QString& command)
    : NetworkException(
          NetworkError::ServerErrorCode::UnableToDetermineDestinationUrl,
          QStringLiteral(
              "Unable to determine destination URL for action or command '%1'")
              .arg(command))
{}
