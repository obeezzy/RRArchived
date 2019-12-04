#ifndef PURCHASEUTILS_H
#define PURCHASEUTILS_H

#include <QString>
#include <QList>
#include <QDateTime>

struct PurchasePayment {
    enum class PaymentMethod {
        Cash,
        DebitCard,
        CreditCard
    };

    double amount;
    PaymentMethod method;
    QString note;
    QString currency;

    QString paymentMethodAsString() const {
        switch (method) {
        case PaymentMethod::Cash:
            return QStringLiteral("cash");
        case PaymentMethod::DebitCard:
            return QStringLiteral("debit_card");
        case PaymentMethod::CreditCard:
            return QStringLiteral("credit_card");
        }

        return QString();
    }

    QVariantMap toVariantMap() const {
        return {
            { "amount", amount },
            { "payment_method", paymentMethodAsString(), },
            { "note", note }
        };
    }
}; Q_DECLARE_TYPEINFO(PurchasePayment, Q_PRIMITIVE_TYPE);

class PurchasePaymentList : public QList<PurchasePayment> {
public:
    QVariantList toVariantList() const {
        QVariantList list;
        for (const auto &payment : *this)
            list.append(payment.toVariantMap());
        return list;
    }
};

struct PurchaseTransaction {
    qint64 id;
    int clientId;
    QString customerName;
    qreal totalCost;
    qreal amountPaid;
    qreal balance;
    qreal discount;
    int noteId;
    QString note;
    bool suspended;
    bool archived;
    QDateTime created;
    QDateTime lastEdited;
    int userId;

    PurchaseTransaction(const QVariantMap &transaction) :
        id(transaction.value("purchase_transaction_id").toLongLong()),
        clientId(transaction.value("client_id").toInt()),
        customerName(transaction.value("customer_name").toString()),
        totalCost(transaction.value("total_cost").toDouble()),
        amountPaid(transaction.value("amount_paid").toDouble()),
        balance(transaction.value("balance").toDouble()),
        discount(transaction.value("discount").toDouble()),
        noteId(transaction.value("note_id").toInt()),
        note(transaction.value("note").toString()),
        suspended(transaction.value("suspended").toBool()),
        archived(transaction.value("archived").toBool()),
        created(transaction.value("created").toDateTime()),
        lastEdited(transaction.value("last_edited").toDateTime()),
        userId(transaction.value("user_id").toInt())
    {}

    QVariantMap toVariantMap() const {
        return {
            { "purchase_transaction_id", id },
            { "client_id", clientId },
            { "customer_name", customerName },
            { "total_cost", totalCost },
            { "amount_paid", amountPaid },
            { "balance", balance },
            { "discount", discount },
            { "note_id", noteId },
            { "note", note },
            { "suspended", suspended },
            { "archived", archived },
            { "created", created },
            { "last_edited", lastEdited },
            { "user_id", userId }
        };
    }
}; Q_DECLARE_TYPEINFO(PurchaseTransaction, Q_PRIMITIVE_TYPE);

class PurchaseTransactionList : public QList<PurchaseTransaction> {
public:
    QVariantList toVariantList() const {
        QVariantList list;
        for (const auto &transaction : *this)
            list.append(transaction.toVariantMap());
        return list;
    }
};

#endif // PURCHASEUTILS_H
