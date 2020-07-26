#include "product.h"
#include <QDateTime>
#include <QVariantMap>

using namespace Utility::Stock;

Product::Product(int id, const QString& product, const Note& note)
    : id(id),
      product(product),
      note(note)
{}

Product::Product(const ProductCategory& category, const QString& product,
                 const QString& description, const QUrl& imageUrl,
                 const ProductQuantity& quantity, const ProductUnit& unit,
                 const Utility::RecordGroup::Flags& flags,
                 const StockMonies& monies, const Utility::Note& note)
    : category(category),
      product(product),
      description(description),
      imageUrl(imageUrl),
      quantity(quantity),
      unit(unit),
      flags(flags),
      monies(monies),
      note(note)
{}

Product::Product(int id, const ProductCategory& category,
                 const QString& product, const QString& description,
                 const QUrl& imageUrl, const ProductUnit& unit,
                 const Utility::RecordGroup::Flags& flags,
                 const StockMonies& monies, const Utility::Note& note)
    : id(id),
      category(category),
      product(product),
      description(description),
      imageUrl(imageUrl),
      unit(unit),
      flags(flags),
      monies(monies),
      note(note)
{}

Product::Product(int id, const ProductCategory& category,
                 const ProductQuantity& quantity, const ProductUnit& unit,
                 const StockMonies& monies, const Note& note)
    : id(id),
      category(category),
      quantity(quantity),
      unit(unit),
      monies(monies),
      note(note)
{}

Product::Product(const QVariantMap& map)
    : id(map.value("product_id").toInt()),
      category(
          ProductCategory{map.value("product_category_id").toInt(),
                          map.value("product_category").toString(),
                          Note{map.value("product_category_note_id").toInt(),
                               map.value("product_category_note").toString()}}),
      product(map.value("product").toString()),
      description(map.value("description").toString()),
      imageUrl(map.value("image_url").toUrl()),
      quantity(map.value("quantity").toDouble()),
      unit(ProductUnit{map}),
      flags((map.value("tracked").toBool() ? RecordGroup::Tracked
                                           : RecordGroup::None) |
                    map.value("divisible").toBool()
                ? RecordGroup::Divisible
                : RecordGroup::None),
      monies(StockMonies{map}),
      note(Note{map}),
      timestamp(RecordTimestamp{map}),
      user(User::User{map})
{}

QVariantMap Product::toVariantMap() const
{
    return {{"product_id", id},
            {"product_category_id", category.id},
            {"product_category", category.category},
            {"quantity", quantity.toDouble()},
            {"product_unit_id", unit.id},
            {"product_unit", unit.unit},
            {"cost_price", monies.costPrice.toDouble()},
            {"retail_price", monies.retailPrice.toDouble()},
            {"unit_price", monies.unitPrice.toDouble()},
            {"divisible", flags.testFlag(RecordGroup::Divisible)},
            {"note_id", note.id},
            {"note", note.note}};
}
