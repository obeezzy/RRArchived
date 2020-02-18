#include "imagetoolargeexception.h"

ImageTooLargeException::ImageTooLargeException(int size,
                                               int maxSize) :
    DatabaseException(DatabaseError::QueryErrorCode::ImageTooLarge,
                      QStringLiteral("Image too large (%1 bytes). Expected size should be less than %2")
                      .arg(size, maxSize))
{

}
