#include <QString>
#include <QtTest>
#include <QCoreApplication>
#include <QSqlQuery>

#include "qmlapi/qmldebttransactionmodel.h"
#include "models/debtpaymentmodel.h"
#include "databaseclient.h"
#include "mockdatabasethread.h"

class QMLDebtTransactionModelTest : public QObject
{
    Q_OBJECT

public:
    QMLDebtTransactionModelTest();

private Q_SLOTS:
    void init();
    void cleanup();

    void testSetImageSource();
    void testSetFirstName();
    void testSetLastName();
    void testSetPreferredName();
    void testSetPrimaryPhoneNumber();
    void testSetNote();

    void testAddAlternatePhoneNumberModel();
    void testRemoveAlternatePhoneNumberModel();

    void testAddAddressModel();
    void testRemoveAddressModel();

    void testAddEmailAddressModel();
    void testRemoveEmailAddressModel();

    void testAddDebt();
    void testUpdateDebt();
    void testRemoveDebt();

    void testAddPayment();
    void testUpdatePayment();
    void testRemovePayment();

    // Long-running tests
    void testSetDebtorId();
    void testSubmitOneDebt();
    void testSubmitMultipleDebts();

    void testFetchDebtAndAddNewPayment();

private:
    QMLDebtTransactionModel *m_debtTransactionModel;
    QueryResult m_result;
    MockDatabaseThread m_thread;
    DatabaseClient *m_client;
};

QMLDebtTransactionModelTest::QMLDebtTransactionModelTest() :
    m_thread(&m_result)
{
    QLoggingCategory::setFilterRules(QStringLiteral("*.info=false"));
}

void QMLDebtTransactionModelTest::init()
{
    m_debtTransactionModel = new QMLDebtTransactionModel(this);
    m_client = new DatabaseClient;
}

void QMLDebtTransactionModelTest::cleanup()
{
    m_debtTransactionModel->deleteLater();
    delete m_client;
}

void QMLDebtTransactionModelTest::testSetImageSource()
{
    QSignalSpy imageSourceChangedSpy(m_debtTransactionModel, &QMLDebtTransactionModel::imageSourceChanged);

    // STEP: Ensure image source is not set.
    QCOMPARE(m_debtTransactionModel->imageSource(), QString());

    // STEP: Set image source.
    m_debtTransactionModel->setImageSource(QStringLiteral("image/source"));

    // STEP: Ensure image source is set and user is notified.
    QCOMPARE(m_debtTransactionModel->imageSource(), QStringLiteral("image/source"));
    QCOMPARE(imageSourceChangedSpy.count(), 1);
    imageSourceChangedSpy.clear();

    // STEP: Ensure user is not notified if image source is set to the same value.
    m_debtTransactionModel->setImageSource(QStringLiteral("image/source"));
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

void QMLDebtTransactionModelTest::testSetPrimaryPhoneNumber()
{
    QSignalSpy primaryPhoneNumberChanged(m_debtTransactionModel, &QMLDebtTransactionModel::primaryPhoneNumberChanged);

    // STEP: Ensure primary phone number is not set.
    QCOMPARE(m_debtTransactionModel->primaryPhoneNumber(), QString());

    // STEP: Set primary phone number.
    m_debtTransactionModel->setPrimaryPhoneNumber(QStringLiteral("123456789"));

    // STEP: Ensure primary phone number is set and user is notified.
    QCOMPARE(m_debtTransactionModel->primaryPhoneNumber(), QStringLiteral("123456789"));
    QCOMPARE(primaryPhoneNumberChanged.count(), 1);
    primaryPhoneNumberChanged.clear();

    // STEP: Ensure user is not notified if primary phone number is set to the same value.
    m_debtTransactionModel->setPrimaryPhoneNumber(QStringLiteral("123456789"));
    QCOMPARE(primaryPhoneNumberChanged.count(), 0);
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

void QMLDebtTransactionModelTest::testAddAlternatePhoneNumberModel()
{
    QSignalSpy alternatePhoneNumberModelChangedSpy(m_debtTransactionModel, &QMLDebtTransactionModel::alternatePhoneNumberModelChanged);

    // STEP: Ensure alternate phone number is not set.
    QCOMPARE(m_debtTransactionModel->alternatePhoneNumberModel(), QStringList());

    // STEP: Add alternate phone number.
    m_debtTransactionModel->addAlternatePhoneNumber(QStringLiteral("987654321"));

    // STEP: Ensure alternate phone number is set and user is notified.
    QCOMPARE(m_debtTransactionModel->alternatePhoneNumberModel().count(), 1);
    QCOMPARE(m_debtTransactionModel->alternatePhoneNumberModel().first(), QStringLiteral("987654321"));
    QCOMPARE(alternatePhoneNumberModelChangedSpy.count(), 1);
    alternatePhoneNumberModelChangedSpy.clear();

    // STEP: Add another alternate phone number.
    m_debtTransactionModel->addAlternatePhoneNumber(QStringLiteral("98765"));

    // STEP: Ensure alternate phone number is set and user is notified.
    QCOMPARE(m_debtTransactionModel->alternatePhoneNumberModel().count(), 2);
    QCOMPARE(m_debtTransactionModel->alternatePhoneNumberModel().last(), QStringLiteral("98765"));
    QCOMPARE(alternatePhoneNumberModelChangedSpy.count(), 1);
    alternatePhoneNumberModelChangedSpy.clear();

    // STEP: Add the same alternate phone number again and ensure it fails.
    m_debtTransactionModel->addAlternatePhoneNumber(QStringLiteral("98765"));
    QCOMPARE(m_debtTransactionModel->alternatePhoneNumberModel().count(), 2);
    QCOMPARE(alternatePhoneNumberModelChangedSpy.count(), 0);
}

void QMLDebtTransactionModelTest::testRemoveAlternatePhoneNumberModel()
{
    QSignalSpy alternatePhoneNumberModelChangedSpy(m_debtTransactionModel, &QMLDebtTransactionModel::alternatePhoneNumberModelChanged);

    // STEP: Ensure alternate phone number is not set.
    QCOMPARE(m_debtTransactionModel->alternatePhoneNumberModel(), QStringList());
    // STEP: Add alternate phone number.
    m_debtTransactionModel->addAlternatePhoneNumber(QStringLiteral("987654321"));
    m_debtTransactionModel->addAlternatePhoneNumber(QStringLiteral("98765"));
    QCOMPARE(m_debtTransactionModel->alternatePhoneNumberModel().count(), 2);
    QCOMPARE(alternatePhoneNumberModelChangedSpy.count(), 2);
    alternatePhoneNumberModelChangedSpy.clear();

    // STEP: Remove alternate phone number.
    m_debtTransactionModel->removeAlternatePhoneNumber(0);

    // STEP: Ensure alternate phone number is set and user is notified.
    QCOMPARE(m_debtTransactionModel->alternatePhoneNumberModel().count(), 1);
    QCOMPARE(m_debtTransactionModel->alternatePhoneNumberModel().first(), QStringLiteral("98765"));
    QCOMPARE(alternatePhoneNumberModelChangedSpy.count(), 1);
    alternatePhoneNumberModelChangedSpy.clear();

    // STEP: Remove another alternate phone number.
    m_debtTransactionModel->removeAlternatePhoneNumber(0);

    // STEP: Ensure alternate phone number is set and user is notified.
    QCOMPARE(m_debtTransactionModel->alternatePhoneNumberModel().count(), 0);
    QCOMPARE(alternatePhoneNumberModelChangedSpy.count(), 1);
    alternatePhoneNumberModelChangedSpy.clear();

    // STEP: Attempt to remove another alternate phone number.
    m_debtTransactionModel->removeAlternatePhoneNumber(0);

    // STEP: Ensure alternate phone number is set and user is notified.
    QCOMPARE(m_debtTransactionModel->alternatePhoneNumberModel().count(), 0);
    QCOMPARE(alternatePhoneNumberModelChangedSpy.count(), 0);
}

void QMLDebtTransactionModelTest::testAddAddressModel()
{
    QSignalSpy addressModelChangedSpy(m_debtTransactionModel, &QMLDebtTransactionModel::addressModelChanged);

    // STEP: Ensure address is not set.
    QCOMPARE(m_debtTransactionModel->addressModel(), QStringList());

    // STEP: Add address.
    m_debtTransactionModel->addAddress(QStringLiteral("Address1"));

    // STEP: Ensure address is set and user is notified.
    QCOMPARE(m_debtTransactionModel->addressModel().count(), 1);
    QCOMPARE(m_debtTransactionModel->addressModel().first(), QStringLiteral("Address1"));
    QCOMPARE(addressModelChangedSpy.count(), 1);
    addressModelChangedSpy.clear();

    // STEP: Add another address.
    m_debtTransactionModel->addAddress(QStringLiteral("Address2"));

    // STEP: Ensure address is set and user is notified.
    QCOMPARE(m_debtTransactionModel->addressModel().count(), 2);
    QCOMPARE(m_debtTransactionModel->addressModel().last(), QStringLiteral("Address2"));
    QCOMPARE(addressModelChangedSpy.count(), 1);
    addressModelChangedSpy.clear();

    // STEP: Add the same address again and ensure it fails.
    m_debtTransactionModel->addAddress(QStringLiteral("Address2"));
    QCOMPARE(m_debtTransactionModel->addressModel().count(), 2);
    QCOMPARE(addressModelChangedSpy.count(), 0);
}

void QMLDebtTransactionModelTest::testRemoveAddressModel()
{
    QSignalSpy addressModelChangedSpy(m_debtTransactionModel, &QMLDebtTransactionModel::addressModelChanged);

    // STEP: Ensure address is not set.
    QCOMPARE(m_debtTransactionModel->addressModel(), QStringList());
    // STEP: Add alternate phone number.
    m_debtTransactionModel->addAddress(QStringLiteral("Address1"));
    m_debtTransactionModel->addAddress(QStringLiteral("Address2"));
    QCOMPARE(m_debtTransactionModel->addressModel().count(), 2);
    QCOMPARE(addressModelChangedSpy.count(), 2);
    addressModelChangedSpy.clear();

    // STEP: Remove address.
    m_debtTransactionModel->removeAddress(0);

    // STEP: Ensure address is set and user is notified.
    QCOMPARE(m_debtTransactionModel->addressModel().count(), 1);
    QCOMPARE(m_debtTransactionModel->addressModel().first(), QStringLiteral("Address2"));
    QCOMPARE(addressModelChangedSpy.count(), 1);
    addressModelChangedSpy.clear();

    // STEP: Remove another address.
    m_debtTransactionModel->removeAddress(0);

    // STEP: Ensure address is set and user is notified.
    QCOMPARE(m_debtTransactionModel->addressModel().count(), 0);
    QCOMPARE(addressModelChangedSpy.count(), 1);
    addressModelChangedSpy.clear();

    // STEP: Attempt to remove another address.
    m_debtTransactionModel->removeAddress(0);

    // STEP: Ensure address is set and user is notified.
    QCOMPARE(m_debtTransactionModel->addressModel().count(), 0);
    QCOMPARE(addressModelChangedSpy.count(), 0);
}

void QMLDebtTransactionModelTest::testAddEmailAddressModel()
{
    QSignalSpy emailAddressModelChangedSpy(m_debtTransactionModel, &QMLDebtTransactionModel::emailAddressModelChanged);

    // STEP: Ensure email address is not set.
    QCOMPARE(m_debtTransactionModel->emailAddressModel(), QStringList());

    // STEP: Add email address.
    m_debtTransactionModel->addEmailAddress(QStringLiteral("email@email.com"));

    // STEP: Ensure email address is set and user is notified.
    QCOMPARE(m_debtTransactionModel->emailAddressModel().count(), 1);
    QCOMPARE(m_debtTransactionModel->emailAddressModel().first(), QStringLiteral("email@email.com"));
    QCOMPARE(emailAddressModelChangedSpy.count(), 1);
    emailAddressModelChangedSpy.clear();

    // STEP: Add another email address.
    m_debtTransactionModel->addEmailAddress(QStringLiteral("email2@email.com"));

    // STEP: Ensure email address is set and user is notified.
    QCOMPARE(m_debtTransactionModel->emailAddressModel().count(), 2);
    QCOMPARE(m_debtTransactionModel->emailAddressModel().last(), QStringLiteral("email2@email.com"));
    QCOMPARE(emailAddressModelChangedSpy.count(), 1);
    emailAddressModelChangedSpy.clear();

    // STEP: Add the same email address again and ensure it fails.
    m_debtTransactionModel->addEmailAddress(QStringLiteral("email2@email.com"));
    QCOMPARE(m_debtTransactionModel->emailAddressModel().count(), 2);
    QCOMPARE(emailAddressModelChangedSpy.count(), 0);
}

void QMLDebtTransactionModelTest::testRemoveEmailAddressModel()
{
    QSignalSpy emailAddressModelChangedSpy(m_debtTransactionModel, &QMLDebtTransactionModel::emailAddressModelChanged);

    // STEP: Ensure email address is not set.
    QCOMPARE(m_debtTransactionModel->emailAddressModel(), QStringList());
    // STEP: Add email address.
    m_debtTransactionModel->addEmailAddress(QStringLiteral("email@email.com"));
    m_debtTransactionModel->addEmailAddress(QStringLiteral("email2@email.com"));
    QCOMPARE(m_debtTransactionModel->emailAddressModel().count(), 2);
    QCOMPARE(emailAddressModelChangedSpy.count(), 2);
    emailAddressModelChangedSpy.clear();

    // STEP: Remove emailaddress.
    m_debtTransactionModel->removeEmailAddress(0);

    // STEP: Ensure email address is set and user is notified.
    QCOMPARE(m_debtTransactionModel->emailAddressModel().count(), 1);
    QCOMPARE(m_debtTransactionModel->emailAddressModel().first(), QStringLiteral("email2@email.com"));
    QCOMPARE(emailAddressModelChangedSpy.count(), 1);
    emailAddressModelChangedSpy.clear();

    // STEP: Remove another email address.
    m_debtTransactionModel->removeEmailAddress(0);

    // STEP: Ensure email address is set and user is notified.
    QCOMPARE(m_debtTransactionModel->emailAddressModel().count(), 0);
    QCOMPARE(emailAddressModelChangedSpy.count(), 1);
    emailAddressModelChangedSpy.clear();

    // STEP: Attempt to remove another email address.
    m_debtTransactionModel->removeEmailAddress(0);

    // STEP: Ensure address is set and user is notified.
    QCOMPARE(m_debtTransactionModel->emailAddressModel().count(), 0);
    QCOMPARE(emailAddressModelChangedSpy.count(), 0);
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
    QCOMPARE(m_debtTransactionModel->index(0).data(QMLDebtTransactionModel::TransactionIdRole).toInt(), -1);
    QCOMPARE(m_debtTransactionModel->index(0).data(QMLDebtTransactionModel::RelatedTransactionIdRole).toInt(), -1);
    QCOMPARE(m_debtTransactionModel->index(0).data(QMLDebtTransactionModel::RelatedTransactionRole).toString(), QString());
    QCOMPARE(m_debtTransactionModel->index(0).data(QMLDebtTransactionModel::DueDateRole).toDateTime(), currentDateTime);
    QCOMPARE(m_debtTransactionModel->index(0).data(QMLDebtTransactionModel::NoteRole).toString(), QStringLiteral("Note"));
//    QCOMPARE(m_debtTransactionModel->index(0).data(QMLDebtTransactionModel::CreatedRole).toDateTime(), QDateTime::currentDateTime());
    QCOMPARE(m_debtTransactionModel->index(0).data(QMLDebtTransactionModel::CurrentBalanceRole).toDouble(), 1234.56);

    // STEP: Ensure debt payment model is updated properly.
    DebtPaymentModel *debtPaymentModel = m_debtTransactionModel->index(0).data(QMLDebtTransactionModel::PaymentModelRole).value<DebtPaymentModel *>();
    QVERIFY(debtPaymentModel != nullptr);
    QCOMPARE(debtPaymentModel->rowCount(), 1);
    QCOMPARE(debtPaymentModel->index(0).data(DebtPaymentModel::AmountPaidRole).toDouble(), 0.0);
    QCOMPARE(debtPaymentModel->index(0).data(DebtPaymentModel::NoteRole).toString(), QString());
    QCOMPARE(debtPaymentModel->index(0).data(DebtPaymentModel::ArchivedRole).toDouble(), 0.0);
    QCOMPARE(debtPaymentModel->index(0).data(DebtPaymentModel::CreatedRole).toDateTime(), QDateTime::currentDateTime());
}

void QMLDebtTransactionModelTest::testUpdateDebt()
{
    QSignalSpy dataChangedSpy(m_debtTransactionModel, &QMLDebtTransactionModel::dataChanged);
    const QDateTime &currentDateTime = QDateTime::currentDateTime();

    // STEP: Add debt.
    m_debtTransactionModel->addDebt(1234.56, currentDateTime, QStringLiteral("Note"));
    QCOMPARE(m_debtTransactionModel->rowCount(), 1);

    // STEP: Add update debt.
    m_debtTransactionModel->updateDebt(0, currentDateTime.addDays(1), QStringLiteral("Note2"));

    // STEP: Ensure model is updated properly.
    QCOMPARE(dataChangedSpy.count(), 1);
    QCOMPARE(m_debtTransactionModel->rowCount(), 1);
    QCOMPARE(m_debtTransactionModel->index(0).data(QMLDebtTransactionModel::DueDateRole).toDateTime(), currentDateTime.addDays(1));
    QCOMPARE(m_debtTransactionModel->index(0).data(QMLDebtTransactionModel::NoteRole).toString(), QStringLiteral("Note2"));

    // STEP: Ensure debt payment model is updated properly.
    DebtPaymentModel *debtPaymentModel = m_debtTransactionModel->index(0).data(QMLDebtTransactionModel::PaymentModelRole).value<DebtPaymentModel *>();
    QVERIFY(debtPaymentModel != nullptr);
    QCOMPARE(debtPaymentModel->rowCount(), 1);
    QCOMPARE(debtPaymentModel->index(0).data(DebtPaymentModel::AmountPaidRole).toDouble(), 0.0);
    QCOMPARE(debtPaymentModel->index(0).data(DebtPaymentModel::NoteRole).toString(), QString());
    QCOMPARE(debtPaymentModel->index(0).data(DebtPaymentModel::ArchivedRole).toBool(), false);
    QCOMPARE(debtPaymentModel->index(0).data(DebtPaymentModel::CreatedRole).toDateTime(), QDateTime::currentDateTime());
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
    DebtPaymentModel *debtPaymentModel = m_debtTransactionModel->index(0).data(QMLDebtTransactionModel::PaymentModelRole).value<DebtPaymentModel *>();
    QVERIFY(debtPaymentModel != nullptr);
    QCOMPARE(debtPaymentModel->rowCount(), 1);

    QSignalSpy rowsInsertedSpy(debtPaymentModel, &DebtPaymentModel::rowsInserted);

    // STEP: Add payment.
    m_debtTransactionModel->addPayment(0, 12.34, QStringLiteral("Payment note"));
    QCOMPARE(rowsInsertedSpy.count(), 1);
    QCOMPARE(debtPaymentModel->index(1).data(DebtPaymentModel::AmountPaidRole).toDouble(), 12.34);
    QCOMPARE(debtPaymentModel->index(1).data(DebtPaymentModel::NoteRole).toString(), QStringLiteral("Payment note"));
    QCOMPARE(debtPaymentModel->index(1).data(DebtPaymentModel::ArchivedRole).toBool(), false);
    QCOMPARE(debtPaymentModel->index(1).data(DebtPaymentModel::CreatedRole).toDateTime(), QDateTime::currentDateTime());
}

void QMLDebtTransactionModelTest::testUpdatePayment()
{
    const QDateTime &currentDateTime = QDateTime::currentDateTime();

    // STEP: Add debt.
    m_debtTransactionModel->addDebt(1234.56, currentDateTime, QStringLiteral("Note"));

    // STEP: Ensure debt payment model is updated properly.
    DebtPaymentModel *debtPaymentModel = m_debtTransactionModel->index(0).data(QMLDebtTransactionModel::PaymentModelRole).value<DebtPaymentModel *>();
    QVERIFY(debtPaymentModel != nullptr);
    QCOMPARE(debtPaymentModel->rowCount(), 1);
    QCOMPARE(debtPaymentModel->index(0).data(DebtPaymentModel::AmountPaidRole).toDouble(), 0.0);
    QCOMPARE(debtPaymentModel->index(0).data(DebtPaymentModel::NoteRole).toString(), QString());
    QCOMPARE(debtPaymentModel->index(0).data(DebtPaymentModel::ArchivedRole).toBool(), false);
//    QCOMPARE(debtPaymentModel->index(0).data(DebtPaymentModel::CreatedRole).toDateTime(), QDateTime::currentDateTime());

    QSignalSpy dataChangedSpy(m_debtTransactionModel, &DebtPaymentModel::dataChanged);

    // STEP: Update payment.
    m_debtTransactionModel->updatePayment(0, 0, 12.34, QStringLiteral("Payment note"));
    QCOMPARE(dataChangedSpy.count(), 1);
    QCOMPARE(debtPaymentModel->index(0).data(DebtPaymentModel::AmountPaidRole).toDouble(), 12.34);
    QCOMPARE(debtPaymentModel->index(0).data(DebtPaymentModel::NoteRole).toString(), QStringLiteral("Payment note"));
    QCOMPARE(debtPaymentModel->index(0).data(DebtPaymentModel::ArchivedRole).toBool(), false);
//    QCOMPARE(debtPaymentModel->index(0).data(DebtPaymentModel::CreatedRole).toDateTime(), QDateTime::currentDateTime());
}

void QMLDebtTransactionModelTest::testRemovePayment()
{
    const QDateTime &currentDateTime = QDateTime::currentDateTime();

    // STEP: Add debt.
    m_debtTransactionModel->addDebt(1234.56, currentDateTime);
    QCOMPARE(m_debtTransactionModel->rowCount(), 1);

    DebtPaymentModel *debtPaymentModel = m_debtTransactionModel->index(0)
            .data(QMLDebtTransactionModel::PaymentModelRole).value<DebtPaymentModel *>();
    QSignalSpy rowsRemovedSpy(debtPaymentModel, &DebtPaymentModel::rowsRemoved);

    // STEP: Remove debt.
    m_debtTransactionModel->removePayment(0, 0);

    // STEP: Ensure model is updated properly.
    QCOMPARE(rowsRemovedSpy.count(), 1);
    QCOMPARE(debtPaymentModel->rowCount(), 0);
}

void QMLDebtTransactionModelTest::testSetDebtorId()
{
    QSignalSpy debtorIdChangedSpy(m_debtTransactionModel, &QMLDebtTransactionModel::debtorIdChanged);
    QSignalSpy successSpy(m_debtTransactionModel, &QMLDebtTransactionModel::success);

    QVERIFY(m_client->initialize());

    // STEP: Ensure debtor ID is not set.
    QCOMPARE(m_debtTransactionModel->debtorId(), -1);

    // STEP: Set debtor ID.
    m_debtTransactionModel->setDebtorId(1);
    QCOMPARE(debtorIdChangedSpy.count(), 1);
    debtorIdChangedSpy.clear();
    QVERIFY(QTest::qWaitFor([&]() { return !m_debtTransactionModel->isBusy(); }, 2000));

    // STEP: Ensure debtor ID is set and user is not notified again.
    QCOMPARE(m_debtTransactionModel->debtorId(), 1);
    QCOMPARE(debtorIdChangedSpy.count(), 0);
    debtorIdChangedSpy.clear();

    // STEP: Ensure user is not notified if debtor ID is set to the same value.
    m_debtTransactionModel->setDebtorId(1);
    QCOMPARE(debtorIdChangedSpy.count(), 0);

    // STEP: Ensure that the transaction succeeded.
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(successSpy.takeFirst().first().value<QMLDebtTransactionModel::SuccessCode>(), QMLDebtTransactionModel::ViewDebtorTransactionsSuccess);
    QCOMPARE(m_debtTransactionModel->rowCount(), 0);
}

void QMLDebtTransactionModelTest::testSubmitOneDebt()
{
    const QDateTime &dueDate = QDateTime::currentDateTime().addDays(2);
    QSignalSpy successSpy(m_debtTransactionModel, &QMLDebtTransactionModel::success);
    QVERIFY(m_client->initialize());

    // STEP: Provide mandatory fields.
    m_debtTransactionModel->setPreferredName(QStringLiteral("Mr. Okoro"));
    m_debtTransactionModel->setPrimaryPhoneNumber(QStringLiteral("123456789"));

    // STEP: Add debt.
    m_debtTransactionModel->addDebt(1234.56, dueDate, QStringLiteral("Note"));

    // STEP: Ensure debt payment model is updated properly.
    DebtPaymentModel *debtPaymentModel = m_debtTransactionModel->index(0).data(QMLDebtTransactionModel::PaymentModelRole).value<DebtPaymentModel *>();
    QVERIFY(debtPaymentModel != nullptr);
    QCOMPARE(debtPaymentModel->rowCount(), 1);

    // STEP: Submit debt info.
    QVERIFY(m_debtTransactionModel->submit());
    QVERIFY(QTest::qWaitFor([&]() { return !m_debtTransactionModel->isBusy(); }, 2000));
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(successSpy.takeFirst().first().value<QMLDebtTransactionModel::SuccessCode>(), QMLDebtTransactionModel::AddDebtorSuccess);

    // STEP: Ensure results are correct.
    QSqlQuery q(m_client->connection());
    QVERIFY(q.exec("SELECT preferred_name, phone_number FROM client WHERE id = 1"));
    QCOMPARE(q.size(), 1);
    QVERIFY(q.next());
    QCOMPARE(q.value("preferred_name").toString(), QStringLiteral("Mr. Okoro"));
    QCOMPARE(q.value("phone_number").toString(), QStringLiteral("123456789"));

    QVERIFY(q.exec("SELECT id FROM debtor WHERE id = 1"));
    QCOMPARE(q.size(), 1);
    QVERIFY(q.next());

    QVERIFY(q.exec("SELECT debtor_id, transaction_table FROM debt_transaction WHERE id = 1"));
    QCOMPARE(q.size(), 1);
    QVERIFY(q.next());
    QCOMPARE(q.value("debtor_id").toInt(), 1);
    QCOMPARE(q.value("transaction_table").toString(), QStringLiteral("debtor"));

    QVERIFY(q.exec("SELECT debt_transaction_id, total_amount, amount_paid, balance, "
                   "due_date, archived FROM debt_payment WHERE id = 1"));
    QCOMPARE(q.size(), 1);
    QVERIFY(q.next());
    QCOMPARE(q.value("debt_transaction_id").toInt(), 1);
    QCOMPARE(q.value("total_amount").toDouble(), 1234.56);
    QCOMPARE(q.value("amount_paid").toDouble(), 0.0);
    QCOMPARE(q.value("balance").toDouble(), 1234.56);
    QCOMPARE(q.value("due_date").toDateTime().secsTo(dueDate), 0);
    QCOMPARE(q.value("archived").toBool(), false);
}

void QMLDebtTransactionModelTest::testSubmitMultipleDebts()
{
    QVERIFY(true);
}

void QMLDebtTransactionModelTest::testFetchDebtAndAddNewPayment()
{
    QSignalSpy successSpy(m_debtTransactionModel, &QMLDebtTransactionModel::success);
    QSignalSpy errorSpy(m_debtTransactionModel, &QMLDebtTransactionModel::error);
    const QDateTime &dueDate = QDateTime::currentDateTime().addDays(2);

    QVERIFY(m_client->initialize());

    // STEP: Submit debt.
    m_debtTransactionModel->setPreferredName(QStringLiteral("Mr. Okoro"));
    m_debtTransactionModel->setPrimaryPhoneNumber(QStringLiteral("123456789"));
    m_debtTransactionModel->addDebt(1234.56, dueDate, QStringLiteral("Note"));
    QVERIFY(m_debtTransactionModel->submit());
    QVERIFY(QTest::qWaitFor([&]() { return !m_debtTransactionModel->isBusy(); }, 2000));
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(successSpy.takeFirst().first().value<QMLDebtTransactionModel::SuccessCode>(), QMLDebtTransactionModel::AddDebtorSuccess);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(m_debtTransactionModel->rowCount(), 0);

    // STEP: Retrieve debtor info.
    m_debtTransactionModel->setDebtorId(1);
    QVERIFY(QTest::qWaitFor([&]() { return !m_debtTransactionModel->isBusy(); }, 2000));
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(successSpy.takeFirst().first().value<QMLDebtTransactionModel::SuccessCode>(), QMLDebtTransactionModel::ViewDebtorTransactionsSuccess);
    QCOMPARE(m_debtTransactionModel->rowCount(), 1);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 0);

    // STEP: Add new payment.
    m_debtTransactionModel->addPayment(0, 1000.0);
    QCOMPARE(m_debtTransactionModel->index(0).data(QMLDebtTransactionModel::CurrentBalanceRole).toDouble(), 234.56);
    QVERIFY(m_debtTransactionModel->submit());
    QVERIFY(QTest::qWaitFor([&]() { return !m_debtTransactionModel->isBusy(); }, 2000));
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(successSpy.takeFirst().first().value<QMLDebtTransactionModel::SuccessCode>(), QMLDebtTransactionModel::UpdateDebtorSuccess);
    QCOMPARE(m_debtTransactionModel->rowCount(), 0);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 0);

    QSqlQuery q(m_client->connection());
    QVERIFY(q.exec(QStringLiteral("SELECT amount_paid FROM debt_payment WHERE debt_transaction_id = 1")));
    QCOMPARE(q.size(), 2);
    QVERIFY(q.first());
    QCOMPARE(q.value("amount_paid").toDouble(), 0.0);
    QVERIFY(q.next());
    QCOMPARE(q.value("amount_paid").toDouble(), 1000.0);
}

QTEST_MAIN(QMLDebtTransactionModelTest)

#include "tst_qmldebttransactionmodeltest.moc"
