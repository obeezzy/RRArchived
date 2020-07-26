#ifndef REMOVEPRODUCT_H
#define REMOVEPRODUCT_H

#include "stockexecutor.h"

namespace Utility {
    namespace Stock {
    class Product;
}
}

namespace Query {
    namespace Stock {
    class RemoveProduct : public StockExecutor
    {
        Q_OBJECT
    public:
        static inline const QString COMMAND = QStringLiteral("remove_product");
        static inline const QString UNDO_COMMAND = QStringLiteral("undo_remove_product");

        explicit RemoveProduct(const Utility::Stock::Product &product,
                                    QObject *receiver);
        explicit RemoveProduct(const QueryRequest &request,
                                    QObject *receiver);
        QueryResult execute() override;
    private:
        QueryResult removeProduct();
        QueryResult undoRemoveProduct();
    };
}
}

#endif // REMOVEPRODUCT_H
