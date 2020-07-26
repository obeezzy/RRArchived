#include <QAbstractItemModelTester>
#include <QCoreApplication>
#include <QString>
#include <QtTest>
#include "mockdatabasethread.h"
#include "qmlapi/debtor/qmldebtpaymentmodel.h"
#include "utility/debtor/debttransaction.h"

class QMLDebtPaymentModelTest : public QObject
{
    Q_OBJECT
private slots:
    void init();
    void cleanup();

    void testModel();
    void testDebtTransactionId();
    void testDebtTransactionRef();
    void testTotalAmountPaid();
    void testAddPayment();
    void testRemovePayment();

    void testUpdateAmountPaidField();
    void testUpdateAmountPaidFieldOnlyIfDirty();
    void testUpdateDueDateField();
    void testReadonlyFields();

    void testError();

private:
    QMLDebtPaymentModel* m_debtPaymentModel;
    MockDatabaseThread* m_thread;
};

void QMLDebtPaymentModelTest::init()
{
    m_thread = new MockDatabaseThread(this);
    m_debtPaymentModel = new QMLDebtPaymentModel(*m_thread, this);
}

void QMLDebtPaymentModelTest::cleanup()
{
    m_debtPaymentModel->deleteLater();
    m_thread->deleteLater();
}

void QMLDebtPaymentModelTest::testModel()
{
    QAbstractItemModelTester(
        m_debtPaymentModel,
        QAbstractItemModelTester::FailureReportingMode::Fatal, this);
}

void QMLDebtPaymentModelTest::testDebtTransactionId()
{
    QSignalSpy debtTransactionIdSpy(
        m_debtPaymentModel, &QMLDebtPaymentModel::debtTransactionIdChanged);

    m_debtPaymentModel->componentComplete();
    QCOMPARE(m_debtPaymentModel->debtTransactionId(), 0);
    m_debtPaymentModel->setDebtTransactionId(1);

    QCOMPARE(m_debtPaymentModel->debtTransactionId(), 1);
    QCOMPARE(debtTransactionIdSpy.count(), 1);
    debtTransactionIdSpy.clear();

    m_debtPaymentModel->setDebtTransactionId(1);
    QCOMPARE(debtTransactionIdSpy.count(), 0);
}

void QMLDebtPaymentModelTest::testDebtTransactionRef()
{
    QSignalSpy debtTransactionRefSpy(
        m_debtPaymentModel, &QMLDebtPaymentModel::debtTransactionRefChanged);

    m_debtPaymentModel->componentComplete();
    QCOMPARE(m_debtPaymentModel->debtTransactionRef(), QVariant());
    m_debtPaymentModel->setDebtTransactionRef(QVariant("ref"));

    QCOMPARE(m_debtPaymentModel->debtTransactionRef(), QVariant("ref"));
    QCOMPARE(debtTransactionRefSpy.count(), 1);
    debtTransactionRefSpy.clear();

    m_debtPaymentModel->setDebtTransactionRef(QVariant("ref"));
    QCOMPARE(debtTransactionRefSpy.count(), 0);
}

void QMLDebtPaymentModelTest::testTotalAmountPaid()
{
    QSignalSpy totalAmountPaidSpy(m_debtPaymentModel,
                                  &QMLDebtPaymentModel::totalAmountPaidChanged);

    m_debtPaymentModel->componentComplete();
    QCOMPARE(m_debtPaymentModel->totalAmountPaid(), 0.0);
    m_debtPaymentModel->setTotalAmountPaid(747.23);

    QCOMPARE(m_debtPaymentModel->totalAmountPaid(), 747.23);
    QCOMPARE(totalAmountPaidSpy.count(), 1);
    totalAmountPaidSpy.clear();

    m_debtPaymentModel->setTotalAmountPaid(747.23);
    QCOMPARE(totalAmountPaidSpy.count(), 0);
}

void QMLDebtPaymentModelTest::testAddPayment()
{
    const auto debtTransaction = Utility::Debtor::DebtTransaction{
        1234.56, QDateTime::currentDateTime().addDays(2)};
    QVariantMap payment{{"debt", debtTransaction.totalDebt.toDouble()},
                        {"amount_paid", 100.44},
                        {"note", QStringLiteral("Test note")}};
    const auto& currentDateTime = QDateTime::currentDateTime();
    const auto balance =
        payment["debt"].toDouble() - payment["amount_paid"].toDouble();
    QSignalSpy rowsInsertedSpy(m_debtPaymentModel,
                               &QMLDebtPaymentModel::rowsInserted);

    m_debtPaymentModel->componentComplete();
    QCOMPARE(m_debtPaymentModel->rowCount(), 0);
    QCOMPARE(rowsInsertedSpy.count(), 0);

    m_debtPaymentModel->setDebtTransactionRef(
        QVariant::fromValue(&debtTransaction));
    m_debtPaymentModel->addPayment(payment["amount_paid"].toDouble(),
                                   payment["note"].toString());

    QCOMPARE(m_debtPaymentModel->rowCount(), 1);
    QCOMPARE(m_debtPaymentModel->totalAmountPaid(),
             payment["amount_paid"].toDouble());
    QCOMPARE(rowsInsertedSpy.count(), 1);
    rowsInsertedSpy.clear();
    QCOMPARE(
        m_debtPaymentModel
            ->data(m_debtPaymentModel->index(0), QMLDebtPaymentModel::DebtRole)
            .toDouble(),
        payment["debt"].toDouble());
    QCOMPARE(m_debtPaymentModel
                 ->data(m_debtPaymentModel->index(0),
                        QMLDebtPaymentModel::AmountPaidRole)
                 .toDouble(),
             payment["amount_paid"].toDouble());
    QCOMPARE(m_debtPaymentModel
                 ->data(m_debtPaymentModel->index(0),
                        QMLDebtPaymentModel::BalanceRole)
                 .toDouble(),
             balance);
    QCOMPARE(m_debtPaymentModel
                 ->data(m_debtPaymentModel->index(0),
                        QMLDebtPaymentModel::MaxPayableAmount)
                 .toDouble(),
             balance);
    QCOMPARE(m_debtPaymentModel
                 ->data(m_debtPaymentModel->index(0),
                        QMLDebtPaymentModel::CurrencyRole)
                 .toString(),
             debtTransaction.totalDebt.currency().isoCode());
    QCOMPARE(m_debtPaymentModel
                 ->data(m_debtPaymentModel->index(0),
                        QMLDebtPaymentModel::DueDateRole)
                 .toDateTime(),
             debtTransaction.dueDateTime);
    QCOMPARE(
        m_debtPaymentModel
            ->data(m_debtPaymentModel->index(0), QMLDebtPaymentModel::NoteRole)
            .toString(),
        payment["note"].toString());
    QCOMPARE(
        m_debtPaymentModel
            ->data(m_debtPaymentModel->index(0), QMLDebtPaymentModel::DirtyRole)
            .toBool(),
        false);
    QCOMPARE(
        m_debtPaymentModel
            ->data(m_debtPaymentModel->index(0), QMLDebtPaymentModel::FreshRole)
            .toBool(),
        true);
    QCOMPARE(m_debtPaymentModel
                 ->data(m_debtPaymentModel->index(0),
                        QMLDebtPaymentModel::ArchivedRole)
                 .toBool(),
             false);
    QCOMPARE(m_debtPaymentModel
                 ->data(m_debtPaymentModel->index(0),
                        QMLDebtPaymentModel::CreatedRole)
                 .toDate(),
             currentDateTime.date());
    QCOMPARE(m_debtPaymentModel
                 ->data(m_debtPaymentModel->index(0),
                        QMLDebtPaymentModel::LastEditedRole)
                 .toDate(),
             currentDateTime.date());

    m_debtPaymentModel->addPayment(payment["amount_paid"].toDouble(),
                                   payment["note"].toString());

    QCOMPARE(m_debtPaymentModel->rowCount(), 2);
    QCOMPARE(
        m_debtPaymentModel->totalAmountPaid(),
        payment["amount_paid"].toDouble() + payment["amount_paid"].toDouble());
    QCOMPARE(rowsInsertedSpy.count(), 1);
    QCOMPARE(
        m_debtPaymentModel
            ->data(m_debtPaymentModel->index(1), QMLDebtPaymentModel::DebtRole)
            .toDouble(),
        balance);
    QCOMPARE(m_debtPaymentModel
                 ->data(m_debtPaymentModel->index(1),
                        QMLDebtPaymentModel::AmountPaidRole)
                 .toDouble(),
             payment["amount_paid"].toDouble());
    QCOMPARE(m_debtPaymentModel
                 ->data(m_debtPaymentModel->index(1),
                        QMLDebtPaymentModel::BalanceRole)
                 .toDouble(),
             balance - payment["amount_paid"].toDouble());
    QCOMPARE(m_debtPaymentModel
                 ->data(m_debtPaymentModel->index(1),
                        QMLDebtPaymentModel::MaxPayableAmount)
                 .toDouble(),
             balance - payment["amount_paid"].toDouble());
    QCOMPARE(m_debtPaymentModel
                 ->data(m_debtPaymentModel->index(1),
                        QMLDebtPaymentModel::CurrencyRole)
                 .toString(),
             debtTransaction.totalDebt.currency().isoCode());
    QCOMPARE(m_debtPaymentModel
                 ->data(m_debtPaymentModel->index(1),
                        QMLDebtPaymentModel::DueDateRole)
                 .toDateTime(),
             debtTransaction.dueDateTime);
    QCOMPARE(
        m_debtPaymentModel
            ->data(m_debtPaymentModel->index(1), QMLDebtPaymentModel::NoteRole)
            .toString(),
        payment["note"].toString());
    QCOMPARE(
        m_debtPaymentModel
            ->data(m_debtPaymentModel->index(1), QMLDebtPaymentModel::DirtyRole)
            .toBool(),
        false);
    QCOMPARE(
        m_debtPaymentModel
            ->data(m_debtPaymentModel->index(1), QMLDebtPaymentModel::FreshRole)
            .toBool(),
        true);
    QCOMPARE(m_debtPaymentModel
                 ->data(m_debtPaymentModel->index(1),
                        QMLDebtPaymentModel::ArchivedRole)
                 .toBool(),
             false);
    QCOMPARE(m_debtPaymentModel
                 ->data(m_debtPaymentModel->index(1),
                        QMLDebtPaymentModel::CreatedRole)
                 .toDate(),
             currentDateTime.date());
    QCOMPARE(m_debtPaymentModel
                 ->data(m_debtPaymentModel->index(1),
                        QMLDebtPaymentModel::LastEditedRole)
                 .toDate(),
             currentDateTime.date());
}

void QMLDebtPaymentModelTest::testRemovePayment()
{
    const auto debtTransaction = Utility::Debtor::DebtTransaction{
        1234.56, QDateTime::currentDateTime().addDays(2)};
    QVariantMap payment{{"debt", debtTransaction.totalDebt.toDouble()},
                        {"amount_paid", 100.44},
                        {"note", QStringLiteral("Test note")}};
    QSignalSpy rowsRemovedSpy(m_debtPaymentModel,
                              &QMLDebtPaymentModel::rowsInserted);

    m_debtPaymentModel->componentComplete();
    QCOMPARE(m_debtPaymentModel->rowCount(), 0);
    QCOMPARE(rowsRemovedSpy.count(), 0);

    m_debtPaymentModel->setDebtTransactionRef(
        QVariant::fromValue(&debtTransaction));
    m_debtPaymentModel->addPayment(payment["amount_paid"].toDouble(),
                                   payment["note"].toString());
    QCOMPARE(m_debtPaymentModel->rowCount(), 1);
    QCOMPARE(m_debtPaymentModel->totalAmountPaid(),
             payment["amount_paid"].toDouble());

    m_debtPaymentModel->removePayment(0);
    QCOMPARE(rowsRemovedSpy.count(), 1);
    QCOMPARE(m_debtPaymentModel->rowCount(), 0);
    QCOMPARE(m_debtPaymentModel->totalAmountPaid(), 0.0);
}

void QMLDebtPaymentModelTest::testUpdateAmountPaidField()
{
    const auto updatedAmountPaid = 348.38;
    const auto debtTransaction = Utility::Debtor::DebtTransaction{
        1234.56, QDateTime::currentDateTime().addDays(2)};
    QVariantMap payment{{"debt", debtTransaction.totalDebt.toDouble()},
                        {"amount_paid", 100.44},
                        {"note", QStringLiteral("Test note")}};

    m_debtPaymentModel->componentComplete();
    QCOMPARE(m_debtPaymentModel->rowCount(), 0);

    m_debtPaymentModel->setDebtTransactionRef(
        QVariant::fromValue(&debtTransaction));
    m_debtPaymentModel->addPayment(payment["amount_paid"].toDouble(),
                                   payment["note"].toString());
    QCOMPARE(m_debtPaymentModel->rowCount(), 1);

    const auto successful = m_debtPaymentModel->setData(
        m_debtPaymentModel->index(0), updatedAmountPaid,
        QMLDebtPaymentModel::AmountPaidRole);
    QCOMPARE(successful, true);
    QCOMPARE(m_debtPaymentModel
                 ->data(m_debtPaymentModel->index(0),
                        QMLDebtPaymentModel::AmountPaidRole)
                 .toDouble(),
             updatedAmountPaid);
}

void QMLDebtPaymentModelTest::testUpdateAmountPaidFieldOnlyIfDirty()
{
    const auto updatedAmountPaid = 348.38;
    const QVariantMap payment{
        {"debt_transaction_id", 1},
        {"debt", 1343.23},
        {"amount_paid", 1000.03},
        {"balance", 343.20},
        {"due_date_time", QDateTime::currentDateTime().addDays(2)},
        {"note", QStringLiteral("Note")},
        {"archived", false},
        {"created", QDateTime::currentDateTime()},
        {"last_edited", QDateTime::currentDateTime()}};
    auto databaseWillReturn = [this](const QVariantList& payments) {
        m_thread->result().setSuccessful(true);
        m_thread->result().setOutcome(QVariantMap{{"payments", payments}});
    };

    databaseWillReturn({payment});

    m_debtPaymentModel->componentComplete();
    QCOMPARE(m_debtPaymentModel->rowCount(), 1);

    const auto successful = m_debtPaymentModel->setData(
        m_debtPaymentModel->index(0), updatedAmountPaid,
        QMLDebtPaymentModel::AmountPaidRole);
    QCOMPARE(successful, false);
    QCOMPARE(m_debtPaymentModel
                 ->data(m_debtPaymentModel->index(0),
                        QMLDebtPaymentModel::AmountPaidRole)
                 .toDouble(),
             payment["amount_paid"].toDouble());
}

void QMLDebtPaymentModelTest::testUpdateDueDateField()
{
    const auto updatedDueDate = QDateTime::currentDateTime().addDays(3);
    const auto debtTransaction = Utility::Debtor::DebtTransaction{
        1234.56, QDateTime::currentDateTime().addDays(2)};
    QVariantMap payment{{"debt", debtTransaction.totalDebt.toDouble()},
                        {"amount_paid", 100.44},
                        {"note", QStringLiteral("Test note")}};

    m_debtPaymentModel->componentComplete();
    QCOMPARE(m_debtPaymentModel->rowCount(), 0);

    m_debtPaymentModel->setDebtTransactionRef(
        QVariant::fromValue(&debtTransaction));
    m_debtPaymentModel->addPayment(payment["amount_paid"].toDouble(),
                                   payment["note"].toString());
    QCOMPARE(m_debtPaymentModel->rowCount(), 1);

    const auto successful = m_debtPaymentModel->setData(
        m_debtPaymentModel->index(0), updatedDueDate,
        QMLDebtPaymentModel::DueDateRole);
    QCOMPARE(successful, true);
    QCOMPARE(m_debtPaymentModel
                 ->data(m_debtPaymentModel->index(0),
                        QMLDebtPaymentModel::DueDateRole)
                 .toDateTime(),
             updatedDueDate);
}

void QMLDebtPaymentModelTest::testReadonlyFields()
{
    const auto updatedDebt = 832.18;
    const auto debtTransaction = Utility::Debtor::DebtTransaction{
        1234.56, QDateTime::currentDateTime().addDays(2)};
    QVariantMap payment{{"debt", debtTransaction.totalDebt.toDouble()},
                        {"amount_paid", 100.44},
                        {"note", QStringLiteral("Test note")}};

    m_debtPaymentModel->componentComplete();
    QCOMPARE(m_debtPaymentModel->rowCount(), 0);

    m_debtPaymentModel->setDebtTransactionRef(
        QVariant::fromValue(&debtTransaction));
    m_debtPaymentModel->addPayment(payment["amount_paid"].toDouble(),
                                   payment["note"].toString());
    QCOMPARE(m_debtPaymentModel->rowCount(), 1);

    const auto successful =
        m_debtPaymentModel->setData(m_debtPaymentModel->index(0), updatedDebt,
                                    QMLDebtPaymentModel::DebtRole);
    QCOMPARE(successful, false);
    QCOMPARE(
        m_debtPaymentModel
            ->data(m_debtPaymentModel->index(0), QMLDebtPaymentModel::DebtRole)
            .toDouble(),
        payment["debt"].toDouble());
}

void QMLDebtPaymentModelTest::testError()
{
    const auto debtTransaction = Utility::Debtor::DebtTransaction{
        1234.56, QDateTime::currentDateTime().addDays(2)};
    auto databaseWillReturnError = [this]() {
        m_thread->result().setSuccessful(false);
    };
    QSignalSpy busyChangedSpy(m_debtPaymentModel,
                              &QMLDebtPaymentModel::busyChanged);
    QSignalSpy errorSpy(m_debtPaymentModel, &QMLDebtPaymentModel::error);
    QSignalSpy successSpy(m_debtPaymentModel, &QMLDebtPaymentModel::success);

    databaseWillReturnError();

    m_debtPaymentModel->componentComplete();
    QCOMPARE(busyChangedSpy.count(), 2);
    QCOMPARE(errorSpy.count(), 1);
    QCOMPARE(successSpy.count(), 0);
}

QTEST_GUILESS_MAIN(QMLDebtPaymentModelTest)

#include "tst_qmldebtpaymentmodeltest.moc"
