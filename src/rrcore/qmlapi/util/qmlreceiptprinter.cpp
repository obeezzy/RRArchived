#include "qmlreceiptprinter.h"
#include <QDateTime>
#include <QDebug>
#include <QPainter>
#include <QPrintDialog>
#include <QPrinter>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQmlProperty>
#include <QQuickItem>
#include <QQuickItemGrabResult>
#include <QQuickWindow>
#include <QSettings>
#include <QSharedPointer>
#include <QStandardPaths>
#include <QVariantMap>
#include "singletons/settings.h"
#include "user/businessdetails.h"
#include "user/userprofile.h"

Q_LOGGING_CATEGORY(lcqmlreceiptprinter, "rrcore.qmlapi.qmlreceiptprinter");

QMLReceiptPrinter::QMLReceiptPrinter(QObject* parent) : QObject(parent) {}

bool QMLReceiptPrinter::isBusy() const
{
    return m_busy;
}

void QMLReceiptPrinter::setBusy(bool busy)
{
    if (m_busy == busy)
        return;

    m_busy = busy;
    emit busyChanged();
}

void QMLReceiptPrinter::print(const QString& job)
{
    if (job.trimmed().isEmpty() || !qmlEngine(this))
        return;

    setBusy(true);
    QPrinter* printer = new QPrinter;
    QPrintDialog dialog(printer);
    if (dialog.exec() == QDialog::Accepted) {
        QQuickItem* receipt = createReceipt(job);
        if (!receipt) {
            qWarning() << "Failed to create receipt";
            setBusy(false);
            return;
        }

        m_result = receipt->grabToImage(QSize(printer->width() * 2, 400));
        connect(m_result.data(), &QQuickItemGrabResult::ready, this,
                [this, receipt, printer]() {
                    QPainter painter(printer);
                    m_result->saveToFile("/tmp/receipt.png");
                    painter.drawImage(QPoint(0, 0), m_result->image());
                    painter.end();

                    setBusy(false);
                    receipt->deleteLater();
                    delete printer;
                });
    }
}

QQuickItem* QMLReceiptPrinter::createReceipt(const QString& job)
{
    Q_UNUSED(job)
    QSettings settings;
    QQmlComponent component(
        qmlEngine(this),
        settings
            .value("settings/receipt/template",
                   Settings::instance().defaultReceiptTemplateUrl())
            .toUrl());
    QQuickItem* receiptTemplate = static_cast<QQuickItem*>(component.create());
    if (!receiptTemplate) {
        qCWarning(lcqmlreceiptprinter) << "Failed to create receipt...";
        return nullptr;
    }

    QQmlApplicationEngine* engine =
        static_cast<QQmlApplicationEngine*>(qmlEngine(this));
    if (!engine || engine->rootObjects().isEmpty()) {
        qCWarning(lcqmlreceiptprinter)
            << "Invalid engine. Root objects:" << engine->rootObjects().count();
        return nullptr;
    }
    QQuickWindow* rootItem =
        static_cast<QQuickWindow*>(engine->rootObjects().first());
    receiptTemplate->setParentItem(rootItem->contentItem());
    receiptTemplate->setVisible(false);

    // QQmlProperty::write(receiptTemplate, "businessLogoUrl",
    // UserProfile::instance().businessDetails()->logoUrl());
    QQmlProperty::write(receiptTemplate, "businessName",
                        UserProfile::instance().businessDetails()->name());
    QQmlProperty::write(receiptTemplate, "businessAddress",
                        UserProfile::instance().businessDetails()->address());
    QQmlProperty::write(
        receiptTemplate, "businessPhoneNumber",
        UserProfile::instance().businessDetails()->phoneNumber());
    QQmlProperty::write(
        receiptTemplate, "businessEstablishmentYear",
        UserProfile::instance().businessDetails()->establishmentYear());

    QQmlProperty::write(receiptTemplate, "leftMargin", 0.0);
    QQmlProperty::write(receiptTemplate, "rightMargin", 0.0);
    QQmlProperty::write(receiptTemplate, "topMargin", 0.0);
    QQmlProperty::write(receiptTemplate, "bottomMargin", 0.0);

    QQmlProperty::write(receiptTemplate, "barcodeUrl",
                        "" /* Generate barcode */);
    QQmlProperty::write(receiptTemplate, "receiptNumber",
                        1 /* Generate receipt number */);
    QQmlProperty::write(receiptTemplate, "date", QDateTime::currentDateTime());
    QQmlProperty::write(receiptTemplate, "cashier",
                        UserProfile::instance().userName());
    // QQmlProperty::write(receiptTemplate, "model",
    // QVariant::fromValue(model));

    return receiptTemplate;
}
