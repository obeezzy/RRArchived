#include "purchasedproduct.h"

using namespace Utility::Purchase;

PurchasedProduct::PurchasedProduct(int id, int purchaseTransactionId)
    : id(id),
      transaction(PurchaseTransaction{purchaseTransactionId})
{}

PurchasedProduct::PurchasedProduct(const QVariantMap& map)
    : id(map.value("purchased_product_id").toInt()),
      transaction(
          PurchaseTransaction{map.value("purchase_transaction_id").toInt()}),
      category(Stock::ProductCategory{map}),
      product(Stock::Product{map}),
      monies(PurchaseMonies{map}),
      flags(map.value("archived").toBool() ? RecordGroup::Archived
                                           : RecordGroup::None),
      note(Note{map}),
      timestamp(RecordTimestamp{map}),
      user(User::User{map})
{}

QVariantMap PurchasedProduct::toVariantMap() const
{
    return {{"purchased_product_id", id},
            {"purchase_transaction_id", transaction.id},
            {"product_category_id", category.id},
            {"product_category", category.category},
            {"product_id", product.id},
            {"product", product.product},
            {"cost", monies.cost.toDouble()},
            {"discount", monies.discount.toDouble()},
            {"archived", flags.testFlag(RecordGroup::Archived)},
            {"note_id", note.id},
            {"note", note.note},
            {"user_id", user.id},
            {"user", user.user}};
}
