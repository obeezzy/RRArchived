#include "purchaseexecutor.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include "database/databaseexception.h"
#include "database/exceptions/exceptions.h"
#include "user/userprofile.h"

PurchaseExecutor::PurchaseExecutor(const QString& command,
                                   const QVariantMap& params, QObject* receiver)
    : TransactionExecutor(command, params, QueryRequest::QueryGroup::Purchase,
                          receiver)
{}

QueryResult PurchaseExecutor::addPurchaseTransaction(TransactionMode mode)
{
    QueryResult result{request()};
    result.setSuccessful(true);

    const QVariantMap& params{request().params()};
    const QString& clientPhoneNumber =
        params.value("client_phone_number").toString().trimmed();
    const double balance = params.value("balance").toDouble();
    const bool suspended = params.value("suspended", false).toBool();
    const bool overlookBalance = params.value("overlook_balance").toBool();
    int clientId = 0;
    int vendorId = 0;
    int purchaseTransactionId = 0;
    int debtorId = 0;
    int debtTransactionId = 0;
    int creditorId = 0;
    int creditTransactionId = 0;

    QSqlDatabase connection = QSqlDatabase::database(connectionName());
    QSqlQuery q(connection);

    try {
        QueryExecutor::enforceArguments({"action"}, params);

        if (mode == TransactionMode::UseSqlTransaction)
            QueryExecutor::beginTransaction(q);

        if (!clientPhoneNumber.isEmpty() && !suspended) {
            clientId = addClient();
            vendorId = addVendor(clientId);
        }

        purchaseTransactionId = addPurchaseTransactionToDatabase(clientId);
        addPurchasePayments(purchaseTransactionId);
        addPurchasedProducts(purchaseTransactionId);
        if (!overlookBalance && !suspended && balance > 0.0) {
            debtorId = TransactionExecutor::addDebtor(clientId);
            debtTransactionId = TransactionExecutor::addDebtTransaction(
                debtorId, purchaseTransactionId);
            TransactionExecutor::addDebtPayments(debtTransactionId);
        } else if (!overlookBalance && !suspended && balance < 0.0) {
            creditorId = TransactionExecutor::addCreditor(clientId);
            creditTransactionId = TransactionExecutor::addCreditTransaction(
                creditorId, purchaseTransactionId);
            TransactionExecutor::addCreditPayments(creditTransactionId);
        }

        if (mode == TransactionMode::UseSqlTransaction)
            QueryExecutor::commitTransaction(q);

        result.setOutcome(
            QVariantMap{{"client_id", clientId},
                        {"sale_transaction_id", purchaseTransactionId}});
        return result;
    } catch (const DatabaseException&) {
        if (mode == TransactionMode::UseSqlTransaction)
            QueryExecutor::rollbackTransaction(q);

        throw;
    }
}

int PurchaseExecutor::addClient()
{
    const QVariantMap& params{request().params()};
    const QString& preferredName =
        params.value("client_preferred_name").toString().trimmed();
    const QString& phoneNumber =
        params.value("client_phone_number").toString().trimmed();

    const auto& records(
        callProcedure("AddClientLite",
                      {ProcedureArgument{ProcedureArgument::Type::In,
                                         "preferred_name", preferredName},
                       ProcedureArgument{ProcedureArgument::Type::In,
                                         "phone_number", phoneNumber},
                       ProcedureArgument{ProcedureArgument::Type::In, "user_id",
                                         UserProfile::instance().userId()}}));

    if (records.isEmpty())
        throw UnexpectedResultException(
            QStringLiteral("Expected client ID but received nothing."));

    return records.first().value("client_id").toInt();
}

int PurchaseExecutor::addVendor(int clientId)
{
    const QVariantMap& params{request().params()};
    const QString& note = params.value("note").toString();
    const int noteId =
        QueryExecutor::addNote(note, QStringLiteral("sale_transaction"),
                               ExceptionPolicy::DisallowExceptions);

    const auto& records = callProcedure(
        "AddVendor",
        {ProcedureArgument{ProcedureArgument::Type::In, "client_id", clientId},
         ProcedureArgument{ProcedureArgument::Type::In, "note_id", noteId},
         ProcedureArgument{ProcedureArgument::Type::In, "user_id",
                           UserProfile::instance().userId()}});

    if (records.isEmpty())
        throw UnexpectedResultException(
            QStringLiteral("Expected vendor ID but received nothing."));

    return records.first().value("vendor_id").toInt();
}

int PurchaseExecutor::addPurchaseTransactionToDatabase(int clientId)
{
    const QVariantMap& params{request().params()};
    const bool shouldGiveChange =
        params.value("action").toString() == QStringLiteral("give_change");
    const double balance =
        shouldGiveChange ? 0.0 : qAbs(params.value("balance").toDouble());
    const QString& note = params.value("note").toString();
    const int noteId =
        QueryExecutor::addNote(note, QStringLiteral("purchase_transaction"),
                               ExceptionPolicy::DisallowExceptions);

    const auto& records(callProcedure(
        "AddPurchaseTransaction",
        {ProcedureArgument{ProcedureArgument::Type::In, "name",
                           params.value("customer_name")},
         ProcedureArgument{ProcedureArgument::Type::In, "client_id", clientId},
         ProcedureArgument{ProcedureArgument::Type::In, "balance", balance},
         ProcedureArgument{ProcedureArgument::Type::In, "discount",
                           params.value("discount", 0.0)},
         ProcedureArgument{ProcedureArgument::Type::In, "suspended",
                           params.value("suspended", false)},
         ProcedureArgument{ProcedureArgument::Type::In, "note_id", noteId},
         ProcedureArgument{ProcedureArgument::Type::In, "user_id",
                           UserProfile::instance().userId()}}));

    if (records.isEmpty())
        throw UnexpectedResultException(QStringLiteral(
            "Expected a purchase transaction ID, received nothing."));

    return records.first().value("purchase_transaction_id").toInt();
}

void PurchaseExecutor::addPurchasePayments(int purchaseTransactionId)
{
    const QVariantMap& params{request().params()};
    const QVariantList& payments = params.value("payments").toList();

    for (const auto& paymentAsVariant : payments) {
        callProcedure(
            "AddPurchasePayment",
            {ProcedureArgument{ProcedureArgument::Type::In,
                               "purchase_transaction_id",
                               purchaseTransactionId},
             ProcedureArgument{ProcedureArgument::Type::In, "amount",
                               paymentAsVariant.toMap().value("amount")},
             ProcedureArgument{ProcedureArgument::Type::In, "payment_method",
                               params.value("payment_method")},
             ProcedureArgument{ProcedureArgument::Type::In, "currency",
                               params.value("currency")},
             ProcedureArgument{ProcedureArgument::Type::In, "note",
                               params.value("note", QVariant::String)},
             ProcedureArgument{ProcedureArgument::Type::In, "user_id",
                               UserProfile::instance().userId()}});
    }
}

void PurchaseExecutor::addPurchasedProducts(int purchaseTransactionId)
{
    const QVariantMap& params{request().params()};
    const QVariantList& products = params.value("products").toList();
    const bool suspended = params.value("suspended").toBool();

    for (const QVariant& productAsVariant : products) {
        const QVariantMap& product = productAsVariant.toMap();
        if (!suspended)
            addToStockProductQuantity(product);

        addPurchasedProduct(purchaseTransactionId, product);
    }
}

void PurchaseExecutor::addToStockProductQuantity(const QVariantMap& product)
{
    // Deduct quantity if:
    // 1. This is a non-suspended transaction.
    // 2. This is a suspended transaction and you want to reserve the goods for
    // this customer.
    const QString& reason = request().command();

    callProcedure(
        "AddStockQuantity",
        {ProcedureArgument{ProcedureArgument::Type::In, "product_id",
                           product.value("product_id")},
         ProcedureArgument{ProcedureArgument::Type::In, "quantity",
                           product.value("quantity").toDouble()},
         ProcedureArgument{ProcedureArgument::Type::In, "unit_id",
                           product.value("product_unit_id")},
         ProcedureArgument{ProcedureArgument::Type::In, "reason", reason},
         ProcedureArgument{ProcedureArgument::Type::In, "user_id",
                           UserProfile::instance().userId()}});
}

void PurchaseExecutor::addPurchasedProduct(int purchaseTransactionId,
                                           const QVariantMap& product)
{
    callProcedure(
        "AddPurchaseProduct",
        {ProcedureArgument{ProcedureArgument::Type::In,
                           "purchase_transaction_id", purchaseTransactionId},
         ProcedureArgument{ProcedureArgument::Type::In, "product_id",
                           product.value("product_id")},
         ProcedureArgument{ProcedureArgument::Type::In, "product_unit_id",
                           product.value("unit_id")},
         ProcedureArgument{ProcedureArgument::Type::In, "unit_price",
                           product.value("unit_price")},
         ProcedureArgument{ProcedureArgument::Type::In, "quantity",
                           product.value("quantity")},
         ProcedureArgument{ProcedureArgument::Type::In, "cost",
                           product.value("cost")},
         ProcedureArgument{ProcedureArgument::Type::In, "discount",
                           product.value("discount")},
         ProcedureArgument{ProcedureArgument::Type::In, "currency",
                           product.value("currency")},
         ProcedureArgument{ProcedureArgument::Type::In, "user_id",
                           UserProfile::instance().userId()}});
}
