#include <QString>
#include <QtTest>
#include <QCoreApplication>

#include "qmlapi/qmldebttransactionmodel.h"
#include "qmlapi/qmldebtorpusher.h"
#include "databaseclient.h"

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
    void testSubmit();
private:
    QMLDebtTransactionModel *m_debtTransactionModel;
    DatabaseClient *m_client;
};

QMLDebtTransactionModelTest::QMLDebtTransactionModelTest()
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

}

void QMLDebtTransactionModelTest::testUpdateDebt()
{

}

void QMLDebtTransactionModelTest::testRemoveDebt()
{

}

void QMLDebtTransactionModelTest::testAddPayment()
{

}

void QMLDebtTransactionModelTest::testUpdatePayment()
{

}

void QMLDebtTransactionModelTest::testRemovePayment()
{

}

void QMLDebtTransactionModelTest::testSetDebtorId()
{
    QSignalSpy debtorIdChangedSpy(m_debtTransactionModel, &QMLDebtTransactionModel::debtorIdChanged);

    QVERIFY(m_client->initialize());

    // STEP: Ensure debtor ID is not set.
    QCOMPARE(m_debtTransactionModel->debtorId(), -1);

    // STEP: Set debtor ID.
    m_debtTransactionModel->setDebtorId(1);
    QVERIFY(QTest::qWaitFor([&]() { return !m_debtTransactionModel->isBusy(); }, 2000));

    // STEP: Ensure debtor ID is set and user is notified.
    QCOMPARE(m_debtTransactionModel->debtorId(), 1);
    QCOMPARE(debtorIdChangedSpy.count(), 1);
    debtorIdChangedSpy.clear();

    // STEP: Ensure user is not notified if debtor ID is set to the same value.
    m_debtTransactionModel->setDebtorId(1);
    QCOMPARE(debtorIdChangedSpy.count(), 0);
}

void QMLDebtTransactionModelTest::testSubmit()
{

}

QTEST_MAIN(QMLDebtTransactionModelTest)

#include "tst_qmldebttransactionmodeltest.moc"
