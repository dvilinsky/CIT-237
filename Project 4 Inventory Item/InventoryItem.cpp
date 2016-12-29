#include "InventoryItem.h"

bool InventoryItem::operator==(const InventoryItem &i2) const {
	return (this->getDescription() == i2.getDescription());
}