#include "qmlapi/qmldebttransactionmodel.h"
#include "mockdatabasethread.h"
#include <QString>
#include <QtTest>
#include <QCoreApplication>

class QMLDebtTransactionModelTest : public QObject
{
    Q_OBJECT
private slots:
    void init();
    void cleanup();

    void testSetImageUrl();
    void testSetFirstName();
    void testSetLastName();
    void testSetPreferredName();
    void testSetPhoneNumber();
    void testSetNote();

    void testAddDebt();
    void testUpdateDebt();
    void testRemoveDebt();

    void testAddPayment();
    void testUpdatePayment();
    void testRemovePayment();

    void testSetDebtorId();
    void testSubmitDebt();
    void testSubmitPayment();

private:
    QMLDebtTransactionModel *m_debtTransactionModel;
    MockDatabaseThread *m_thread;
};

void QMLDebtTransactionModelTest::init()
{
    m_thread = new MockDatabaseThread(this);
    m_debtTransactionModel = new QMLDebtTransactionModel(*m_thread, this);
}

void QMLDebtTransactionModelTest::cleanup()
{
    m_debtTransactionModel->deleteLater();
    m_thread->deleteLater();
}

void QMLDebtTransactionModelTest::testSetImageUrl()
{
    QSignalSpy imageSourceChangedSpy(m_debtTransactionModel, &QMLDebtTransactionModel::imageUrlChanged);

    // STEP: Ensure image source is not set.
    QCOMPARE(m_debtTransactionModel->imageUrl(), QUrl());

    // STEP: Set image source.
    m_debtTransactionModel->setImageUrl(QStringLiteral("image/source"));

    // STEP: Ensure image source is set and user is notified.
    QCOMPARE(m_debtTransactionModel->imageUrl(), QStringLiteral("image/source"));
    QCOMPARE(imageSourceChangedSpy.count(), 1);
    imageSourceChangedSpy.clear();

    // STEP: Ensure user is not notified if image source is set to the same value.
    m_debtTransactionModel->setImageUrl(QStringLiteral("image/source"));
    QCOMPARE(imageSourceChangedSpy.count(), 0);
}

void QMLDebtTransactionModelTest::testSetFirstName()
{
    QSignalSpy firstNameChangedSpy(m_debtTransactionModel, &QMLDebtTransactionModel::firstNameChanged);

    // STEP: Ensure first name is not set.
    QCOMPARE(m_debtTransactionModel->firstName(), QString());

    // STEP: Set first name.
    m_debtTransactionModel->setFirstName(QStringLiteral("First"));

    // STEP: Ensure first name is set and user is notified.
    QCOMPARE(m_debtTransactionModel->firstName(), QStringLiteral("First"));
    QCOMPARE(firstNameChangedSpy.count(), 1);
    firstNameChangedSpy.clear();

    // STEP: Ensure user is not notified if first name is set to the same value.
    m_debtTransactionModel->setFirstName(QStringLiteral("First"));
    QCOMPARE(firstNameChangedSpy.count(), 0);
}

void QMLDebtTransactionModelTest::testSetLastName()
{
    QSignalSpy lastNameChangedSpy(m_debtTransactionModel, &QMLDebtTransactionModel::lastNameChanged);

    // STEP: Ensure last name is not set.
    QCOMPARE(m_debtTransactionModel->lastName(), QString());

    // STEP: Set last name.
    m_debtTransactionModel->setLastName(QStringLiteral("Last"));

    // STEP: Ensure last name is set and user is notified.
    QCOMPARE(m_debtTransactionModel->lastName(), QStringLiteral("Last"));
    QCOMPARE(lastNameChangedSpy.count(), 1);
    lastNameChangedSpy.clear();

    // STEP: Ensure user is not notified if last name is set to the same value.
    m_debtTransactionModel->setLastName(QStringLiteral("Last"));
    QCOMPARE(lastNameChangedSpy.count(), 0);
}

void QMLDebtTransactionModelTest::testSetPreferredName()
{
    QSignalSpy preferredNameChangedSpy(m_debtTransactionModel, &QMLDebtTransactionModel::preferredNameChanged);

    // STEP: Ensure preferred name is not set.
    QCOMPARE(m_debtTransactionModel->preferredName(), QString());

    // STEP: Set preferred name.
    m_debtTransactionModel->setPreferredName(QStringLiteral("Preferred"));

    // STEP: Ensure preferred name is set and user is notified.
    QCOMPARE(m_debtTransactionModel->preferredName(), QStringLiteral("Preferred"));
    QCOMPARE(preferredNameChangedSpy.count(), 1);
    preferredNameChangedSpy.clear();

    // STEP: Ensure user is not notified if preferred name is set to the same value.
    m_debtTransactionModel->setPreferredName(QStringLiteral("Preferred"));
    QCOMPARE(preferredNameChangedSpy.count(), 0);
}

void QMLDebtTransactionModelTest::testSetPhoneNumber()
{
    QSignalSpy phoneNumberChanged(m_debtTransactionModel, &QMLDebtTransactionModel::phoneNumberChanged);

    // STEP: Ensure primary phone number is not set.
    QCOMPARE(m_debtTransactionModel->phoneNumber(), QString());

    // STEP: Set primary phone number.
    m_debtTransactionModel->setPhoneNumber(QStringLiteral("123456789"));

    // STEP: Ensure primary phone number is set and user is notified.
    QCOMPARE(m_debtTransactionModel->phoneNumber(), QStringLiteral("123456789"));
    QCOMPARE(phoneNumberChanged.count(), 1);
    phoneNumberChanged.clear();

    // STEP: Ensure user is not notified if primary phone number is set to the same value.
    m_debtTransactionModel->setPhoneNumber(QStringLiteral("123456789"));
    QCOMPARE(phoneNumberChanged.count(), 0);
}

void QMLDebtTransactionModelTest::testSetNote()
{
    QSignalSpy noteChangedSpy(m_debtTransactionModel, &QMLDebtTransactionModel::noteChanged);

    // STEP: Ensure note is not set.
    QCOMPARE(m_debtTransactionModel->note(), QString());

    // STEP: Set note.
    m_debtTransactionModel->setNote(QStringLiteral("Note"));

    // STEP: Ensure note is set and user is notified.
    QCOMPARE(m_debtTransactionModel->note(), QStringLiteral("Note"));
    QCOMPARE(noteChangedSpy.count(), 1);
    noteChangedSpy.clear();

    // STEP: Ensure user is not notified if note is set to the same value.
    m_debtTransactionModel->setNote(QStringLiteral("Note"));
    QCOMPARE(noteChangedSpy.count(), 0);
}

void QMLDebtTransactionModelTest::testAddDebt()
{
    QSignalSpy rowsInsertedSpy(m_debtTransactionModel, &QMLDebtTransactionModel::rowsInserted);
    const QDateTime &currentDateTime = QDateTime::currentDateTime();

    // STEP: Add debt.
    m_debtTransactionModel->addDebt(1234.56, currentDateTime, QStringLiteral("Note"));

    // STEP: Ensure model is updated properly.
    QCOMPARE(rowsInsertedSpy.count(), 1);
    QCOMPARE(m_debtTransactionModel->rowCount(), 1);
    QCOMPARE(m_debtTransactionModel->index(0).data(QMLDebtTransactionModel::TransactionIdRole).toInt(), 0);
    QCOMPARE(m_debtTransactionModel->index(0).data(QMLDebtTransactionModel::RelatedTransactionIdRole).toInt(), 0);
    QCOMPARE(m_debtTransactionModel->index(0).data(QMLDebtTransactionModel::RelatedTransactionRole).toString(), QString());
    QCOMPARE(m_debtTransactionModel->index(0).data(QMLDebtTransactionModel::DueDateRole).toDateTime(), currentDateTime);
    QCOMPARE(m_debtTransactionModel->index(0).data(QMLDebtTransactionModel::NoteRole).toString(), QStringLiteral("Note"));
    QCOMPARE(m_debtTransactionModel->index(0).data(QMLDebtTransactionModel::TotalDebtRole).toDouble(), 1234.56);

    // STEP: Ensure debt payment model is updated properly.
//    DebtPaymentModel *debtPaymentModel = m_debtTransactionModel->index(0).data(QMLDebtTransactionModel::PaymentModelRole).value<DebtPaymentModel *>();
//    QVERIFY(debtPaymentModel != nullptr);
//    QCOMPARE(debtPaymentModel->rowCount(), 1);
//    QCOMPARE(debtPaymentModel->index(0).data(DebtPaymentModel::AmountPaidRole).toDouble(), 0.0);
//    QCOMPARE(debtPaymentModel->index(0).data(DebtPaymentModel::NoteRole).toString(), QString());
//    QCOMPARE(debtPaymentModel->index(0).data(DebtPaymentModel::ArchivedRole).toDouble(), 0.0);
}

void QMLDebtTransactionModelTest::testUpdateDebt()
{
    QSignalSpy dataChangedSpy(m_debtTransactionModel, &QMLDebtTransactionModel::dataChanged);
    const QDateTime &currentDateTime = QDateTime::currentDateTime();

    // STEP: Add debt.
    m_debtTransactionModel->addDebt(1234.56, currentDateTime, QStringLiteral("Note"));
    QCOMPARE(m_debtTransactionModel->rowCount(), 1);

    // STEP: Add update debt.
//    m_debtTransactionModel->updateDebt(0, currentDateTime.addDays(1), QStringLiteral("Note2"));

//    // STEP: Ensure model is updated properly.
//    QCOMPARE(dataChangedSpy.count(), 1);
//    QCOMPARE(m_debtTransactionModel->rowCount(), 1);
//    QCOMPARE(m_debtTransactionModel->index(0).data(QMLDebtTransactionModel::DueDateRole).toDateTime(), currentDateTime.addDays(1));
//    QCOMPARE(m_debtTransactionModel->index(0).data(QMLDebtTransactionModel::NoteRole).toString(), QStringLiteral("Note2"));

//    // STEP: Ensure debt payment model is updated properly.
//    DebtPaymentModel *debtPaymentModel = m_debtTransactionModel->index(0).data(QMLDebtTransactionModel::PaymentModelRole).value<DebtPaymentModel *>();
//    QVERIFY(debtPaymentModel != nullptr);
//    QCOMPARE(debtPaymentModel->rowCount(), 1);
//    QCOMPARE(debtPaymentModel->index(0).data(DebtPaymentModel::AmountPaidRole).toDouble(), 0.0);
//    QCOMPARE(debtPaymentModel->index(0).data(DebtPaymentModel::NoteRole).toString(), QString());
//    QCOMPARE(debtPaymentModel->index(0).data(DebtPaymentModel::ArchivedRole).toBool(), false);
//    QCOMPARE(debtPaymentModel->index(0).data(DebtPaymentModel::CreatedRole).toDateTime(), QDateTime::currentDateTime());
}

void QMLDebtTransactionModelTest::testRemoveDebt()
{
    QSignalSpy rowsRemovedSpy(m_debtTransactionModel, &QMLDebtTransactionModel::rowsRemoved);
    const QDateTime &currentDateTime = QDateTime::currentDateTime();

    // STEP: Add debt.
    m_debtTransactionModel->addDebt(1234.56, currentDateTime);
    QCOMPARE(m_debtTransactionModel->rowCount(), 1);

    // STEP: Remove debt.
    m_debtTransactionModel->removeDebt(0);

    // STEP: Ensure model is updated properly.
    QCOMPARE(rowsRemovedSpy.count(), 1);
    QCOMPARE(m_debtTransactionModel->rowCount(), 0);
}

void QMLDebtTransactionModelTest::testAddPayment()
{
    const QDateTime &currentDateTime = QDateTime::currentDateTime();

    // STEP: Add debt.
    m_debtTransactionModel->addDebt(1234.56, currentDateTime, QStringLiteral("Note"));

    // STEP: Ensure debt payment model is updated properly.
//    DebtPaymentModel *debtPaymentModel = m_debtTransactionModel->index(0).data(QMLDebtTransactionModel::PaymentModelRole).value<DebtPaymentModel *>();
//    QVERIFY(debtPaymentModel != nullptr);
//    QCOMPARE(debtPaymentModel->rowCount(), 1);

//    QSignalSpy rowsInsertedSpy(debtPaymentModel, &DebtPaymentModel::rowsInserted);

//    // STEP: Add payment.
//    m_debtTransactionModel->addPayment(0, 12.34, QStringLiteral("Payment note"));
//    QCOMPARE(rowsInsertedSpy.count(), 1);
//    QCOMPARE(debtPaymentModel->index(1).data(DebtPaymentModel::AmountPaidRole).toDouble(), 12.34);
//    QCOMPARE(debtPaymentModel->index(1).data(DebtPaymentModel::NoteRole).toString(), QStringLiteral("Payment note"));
//    QCOMPARE(debtPaymentModel->index(1).data(DebtPaymentModel::ArchivedRole).toBool(), false);
//    QCOMPARE(debtPaymentModel->index(1).data(DebtPaymentModel::CreatedRole).toDateTime(), QDateTime::currentDateTime());
}

void QMLDebtTransactionModelTest::testUpdatePayment()
{
    const QDateTime &currentDateTime = QDateTime::currentDateTime();

    // STEP: Add debt.
    m_debtTransactionModel->addDebt(1234.56, currentDateTime, QStringLiteral("Note"));

    // STEP: Ensure debt payment model is updated properly.
//    DebtPaymentModel *debtPaymentModel = m_debtTransactionModel->index(0).data(QMLDebtTransactionModel::PaymentModelRole).value<DebtPaymentModel *>();
//    QVERIFY(debtPaymentModel != nullptr);
//    QCOMPARE(debtPaymentModel->rowCount(), 1);
//    QCOMPARE(debtPaymentModel->index(0).data(DebtPaymentModel::AmountPaidRole).toDouble(), 0.0);
//    QCOMPARE(debtPaymentModel->index(0).data(DebtPaymentModel::NoteRole).toString(), QString());
//    QCOMPARE(debtPaymentModel->index(0).data(DebtPaymentModel::ArchivedRole).toBool(), false);
//    QCOMPARE(debtPaymentModel->index(0).data(DebtPaymentModel::CreatedRole).toDateTime(), currentDateTime);

//    QSignalSpy dataChangedSpy(m_debtTransactionModel, &DebtPaymentModel::dataChanged);

//    // STEP: Update payment.
//    m_debtTransactionModel->updatePayment(0, 0, 12.34, QStringLiteral("Payment note"));
//    QCOMPARE(dataChangedSpy.count(), 1);
//    QCOMPARE(debtPaymentModel->index(0).data(DebtPaymentModel::AmountPaidRole).toDouble(), 12.34);
//    QCOMPARE(debtPaymentModel->index(0).data(DebtPaymentModel::NoteRole).toString(), QStringLiteral("Payment note"));
//    QCOMPARE(debtPaymentModel->index(0).data(DebtPaymentModel::ArchivedRole).toBool(), false);
}

void QMLDebtTransactionModelTest::testRemovePayment()
{
    const QDateTime &currentDateTime = QDateTime::currentDateTime();

    // STEP: Add debt.
//    m_debtTransactionModel->addDebt(1234.56, currentDateTime);
//    QCOMPARE(m_debtTransactionModel->rowCount(), 1);

//    DebtPaymentModel *debtPaymentModel = m_debtTransactionModel->index(0)
//            .data(QMLDebtTransactionModel::PaymentModelRole).value<DebtPaymentModel *>();
//    QSignalSpy rowsRemovedSpy(debtPaymentModel, &DebtPaymentModel::rowsRemoved);

//    // STEP: Remove debt.
//    m_debtTransactionModel->removePayment(0, 0);

//    // STEP: Ensure model is updated properly.
//    QCOMPARE(rowsRemovedSpy.count(), 1);
//    QCOMPARE(debtPaymentModel->rowCount(), 0);
}

void QMLDebtTransactionModelTest::testSetDebtorId()
{
    auto databaseWillReturnEmptyResult = [this]() {
        m_thread->result().setSuccessful(true);
    };

    QSignalSpy debtorIdChangedSpy(m_debtTransactionModel, &QMLDebtTransactionModel::debtorIdChanged);
    QSignalSpy successSpy(m_debtTransactionModel, &QMLDebtTransactionModel::success);

    // STEP: Ensure debtor ID is not set.
    QCOMPARE(m_debtTransactionModel->debtorId(), 0);

    databaseWillReturnEmptyResult();

    // STEP: Set debtor ID.
    m_debtTransactionModel->setDebtorId(1);
    QCOMPARE(debtorIdChangedSpy.count(), 1);
    debtorIdChangedSpy.clear();

    // STEP: Ensure debtor ID is set and user is not notified again.
    QCOMPARE(m_debtTransactionModel->debtorId(), 1);
    QCOMPARE(debtorIdChangedSpy.count(), 0);
    debtorIdChangedSpy.clear();

    databaseWillReturnEmptyResult();

    // STEP: Ensure user is not notified if debtor ID is set to the same value.
    m_debtTransactionModel->setDebtorId(1);
    QCOMPARE(debtorIdChangedSpy.count(), 0);

    // STEP: Ensure the transaction succeeded.
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(successSpy.takeFirst().first().value<ModelResult>().code(),
             QMLDebtTransactionModel::ViewDebtorTransactionsSuccess);
    QCOMPARE(m_debtTransactionModel->rowCount(), 0);
}

void QMLDebtTransactionModelTest::testSubmitDebt()
{
    auto databaseWillReturnSingleDebt = [this]() {
        m_thread->result().setSuccessful(true);
        const QueryRequest &request = m_thread->result().request();
        const QVariantList paymentGroups {
            QVariantMap {
                { "debt_transaction_id", 1 },
                { "debt_payment_id", 1 },
                { "total_amount", request.params().value("total_debt") },
                { "amount_paid", request.params().value("amount_paid") },
                { "balance", request.params().value("total_debt") },
                { "currency", "NGN" },
                { "archived", request.params().value("archived") }
            }
        };
        const QVariantList transactions {
            QVariantMap {
                { "debt_transaction_id", 1 },
                { "related_transaction_table", "debtor" },
                { "related_transaction_id", 1 },
                { "total_debt", request.params().value("total_debt") },
                { "note_id", 1 }
            }
        };
        m_thread->result().setOutcome(QVariantMap {
                                { "client_id", 0 },
                                { "debtor_id", 1 },
                                { "preferred_name", request.params().value("preferred_name") },
                                { "primary_phone_number", request.params().value("primary_phone_number") },
                                { "note", "" },
                                { "transactions", transactions },
                                { "payment_groups", paymentGroups },
                                { "record_count", transactions.count() }
                            });
    };

    QSignalSpy successSpy(m_debtTransactionModel, &QMLDebtTransactionModel::success);
    const QDateTime &dueDate = QDateTime::currentDateTime().addDays(2);

    // STEP: Provide mandatory fields.
    m_debtTransactionModel->setPreferredName(QStringLiteral("Mr. Okoro"));
    m_debtTransactionModel->setPhoneNumber(QStringLiteral("123456789"));

    // STEP: Add debt.
    m_debtTransactionModel->addDebt(1234.56, dueDate, QStringLiteral("Note"));
    QCOMPARE(m_debtTransactionModel->index(0).data(QMLDebtTransactionModel::TotalDebtRole).toDouble(), 1234.56);
    QCOMPARE(m_debtTransactionModel->index(0).data(QMLDebtTransactionModel::NoteRole).toString(), QStringLiteral("Note"));

    databaseWillReturnSingleDebt();

    // STEP: Submit debt info.
    QVERIFY(m_debtTransactionModel->submit());
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(successSpy.takeFirst().first().value<ModelResult>().code(), QMLDebtTransactionModel::AddDebtorSuccess);
    QCOMPARE(m_debtTransactionModel->rowCount(), 0);
}

void QMLDebtTransactionModelTest::testSubmitPayment()
{
    auto databaseWillReturnSingleDebtWithSinglePayment = [this]() {
        m_thread->result().setSuccessful(true);
        const QueryRequest &request = m_thread->result().request();
        const QVariantList paymentGroups {
            QVariantMap {
                { "debt_transaction_id", 1 },
                { "debt_payment_id", 1 },
                { "total_amount", request.params().value("total_debt") },
                { "amount_paid", request.params().value("amount_paid") },
                { "balance", request.params().value("total_debt") },
                { "currency", "NGN" },
                { "archived", request.params().value("archived") }
            }
        };
        const QVariantList transactions {
            QVariantMap {
                { "debt_transaction_id", 1 },
                { "related_transaction_table", "debtor" },
                { "related_transaction_id", 1 },
                { "total_debt", request.params().value("total_debt") },
                { "note_id", 1 }
            }
        };
        m_thread->result().setOutcome(QVariantMap {
                                { "client_id", 0 },
                                { "debtor_id", 1 },
                                { "preferred_name", request.params().value("preferred_name") },
                                { "primary_phone_number", request.params().value("primary_phone_number") },
                                { "note", "" },
                                { "transactions", transactions },
                                { "payment_groups", paymentGroups },
                                { "record_count", transactions.count() }
                            });
    };

    const QDateTime &dueDate = QDateTime::currentDateTime().addDays(2);
    QSignalSpy successSpy(m_debtTransactionModel, &QMLDebtTransactionModel::success);

    // STEP: Provide mandatory fields.
    m_debtTransactionModel->setPreferredName(QStringLiteral("Mr. Okoro"));
    m_debtTransactionModel->setPhoneNumber(QStringLiteral("123456789"));

    // STEP: Add debt.
    m_debtTransactionModel->addDebt(1234.56, dueDate, QStringLiteral("Note"));
//    m_debtTransactionModel->addPayment(0, 1234.56, QStringLiteral("Payment Note"));

//    // STEP: Ensure debt payment model is updated properly.
//    DebtPaymentModel *debtPaymentModel = m_debtTransactionModel->index(0).data(QMLDebtTransactionModel::PaymentModelRole).value<DebtPaymentModel *>();
//    QVERIFY(debtPaymentModel != nullptr);
//    QCOMPARE(debtPaymentModel->rowCount(), 2);

//    // STEP: Check the first row of the debt payment table
//    QCOMPARE(debtPaymentModel->index(0).data(DebtPaymentModel::AmountPaidRole).toDouble(), 0.0);
//    QCOMPARE(debtPaymentModel->index(0).data(DebtPaymentModel::NoteRole).toString(), QString());

//    // STEP: Check the second row of the debt payment table
//    QCOMPARE(debtPaymentModel->index(1).data(DebtPaymentModel::AmountPaidRole).toDouble(), 1234.56);
//    QCOMPARE(debtPaymentModel->index(1).data(DebtPaymentModel::NoteRole).toString(), QStringLiteral("Payment Note"));

//    databaseWillReturnSingleDebtWithSinglePayment();

    // STEP: Submit debt info.
//    QVERIFY(m_debtTransactionModel->submit());
//    QCOMPARE(successSpy.count(), 1);
//    QCOMPARE(successSpy.takeFirst().first().value<QMLDebtTransactionModel::SuccessCode>(), QMLDebtTransactionModel::AddDebtorSuccess);
//    QCOMPARE(m_debtTransactionModel->rowCount(), 0);
}

QTEST_GUILESS_MAIN(QMLDebtTransactionModelTest)

#include "tst_qmldebttransactionmodeltest.moc"
