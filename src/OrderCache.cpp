#include <OrderCache.h>
#include <MatchingEngine.h>

bool Order::isValidOrder() const {

	if (0==qty()) {
		throw std::invalid_argument("cannot create object : Qty is zero");
	}

	if (orderId().empty()) {
		throw std::invalid_argument("cannot create object : OrderId is empty");
	}

	if (securityId().empty()) {
		throw std::invalid_argument("cannot create object : securityId is empty");
	}

	if (user().empty()) {
		throw std::invalid_argument("cannot create object : User is empty");
	}

	if (company().empty()) {
		throw std::invalid_argument("cannot create object : Company is empty");
	}

	if (side().empty()) {
		throw std::invalid_argument("cannot create object : Side is empty");
	}

	if (!(side()=="Buy" || side()=="Sell")) {
		throw std::invalid_argument("cannot create object : Invalid Side : " + side());
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////


OrderCache::OrderCache()
{
    mapIdToOrder.reserve(ORDER_CAPACITY);  				// Preallocate memory for ORDER_CAPACITY orders
}

void OrderCache::addOrder(Order order) {

	if (!order.isValidOrder()) {
		return;
	}

	// Check if an order with this ID already exists
	if (mapIdToOrder.find(order.orderId()) != mapIdToOrder.end()) {
		throw std::runtime_error("Cannot create object: Cannot replace existing order");
	}

	// Add order details
	mapSecurityToOrderIds[order.securityId()].emplace_back(order.orderId());
	mapUserToOrderIds[order.user()].emplace_back(order.orderId());
	mapIdToOrder.emplace(order.orderId(), order);
}


void OrderCache::cancelOrder(const std::string& orderId) {

	if (orderId.empty()) {
			throw std::invalid_argument("cannot cancelOrder : OrderId is empty");
	}

	auto itrOrderMap = mapIdToOrder.find(orderId);
	if (itrOrderMap == mapIdToOrder.end()) {
		throw std::runtime_error("Cannot cancelOrder: OrderId does not exist");
	}


	// Remove orderID from mapSecurityToOrderIds
	auto itrSecMap = mapSecurityToOrderIds.find(itrOrderMap->second.securityId());
	if (itrSecMap != mapSecurityToOrderIds.end()) {
		auto& vec = itrSecMap->second;
		vec.erase(std::remove(vec.begin(), vec.end(), orderId), vec.end());
		if (vec.empty()) {
			mapSecurityToOrderIds.erase(itrSecMap);
		}
	}

	// Remove orderID from mapUserToOrderIds
	auto itrUserMap = mapUserToOrderIds.find(itrOrderMap->second.user());
	if (itrUserMap != mapUserToOrderIds.end()) {
		auto& vec = itrUserMap->second;
		vec.erase(std::remove(vec.begin(), vec.end(), orderId), vec.end());
		if (vec.empty()) {
			mapUserToOrderIds.erase(itrUserMap);
		}
	}

	// Remove the order from mapIdToOrder
	mapIdToOrder.erase(itrOrderMap);
}


void OrderCache::cancelOrdersForUser(const std::string& user) {

	if (user.empty()) {
		throw std::invalid_argument("cannot cancel users order : User is empty");
	}

	auto itrUserMap = mapUserToOrderIds.find(user);
	if (itrUserMap == mapUserToOrderIds.end()) {
		std::cout << "Orders not found for user {" << user << "}" << std::endl;
		return;
	}

	// Iterate over orders for the specified user and remove them
	auto ordersToCancel = itrUserMap->second;
	for (const auto& orderId : ordersToCancel) {
		cancelOrder(orderId);
	}
}


void OrderCache::cancelOrdersForSecIdWithMinimumQty(const std::string& securityId, unsigned int minQty) {

	if (securityId.empty()) {
		throw std::invalid_argument("cannot cancelOrdersForSecIdWithMinimumQty: securityId is empty");
	}

	if (0 == minQty) {
		throw std::invalid_argument("cannot cancelOrdersForSecIdWithMinimumQty: minQty is zero");
	}

	auto itrSecMap = mapSecurityToOrderIds.find(securityId);
	if (itrSecMap == mapSecurityToOrderIds.end()) {
		std::cout << "No orders found for securityId {" << securityId << "}" << std::endl;
		return;
	}

	// Iterate over orders for the specified securityId and remove those with quantity above minQty
	auto ordersToCancel = itrSecMap->second;
	for (const auto& orderId : ordersToCancel) {
		auto itrOrder = mapIdToOrder.find(orderId);
		if (itrOrder != mapIdToOrder.end() && itrOrder->second.qty() >= minQty) {
            cancelOrder(orderId);
		}
	}
}


unsigned int OrderCache::getMatchingSizeForSecurity(const std::string& securityId) {

	if (securityId.empty()) {
		throw std::invalid_argument("cannot getMatchingSizeForSecurity : securityId is empty");
	}

	MatchingEngine  marketMachingEngine;

	// Iterate all orders for the given securityId and and process them sequentially in the Market Matching Engine
	auto itrSecMap = mapSecurityToOrderIds.find(securityId);
	if (itrSecMap != mapSecurityToOrderIds.end()) {
		for (const auto& orderId : itrSecMap->second) {
			auto itrOrder = mapIdToOrder.find(orderId);
			if (itrOrder != mapIdToOrder.end()) {
				marketMachingEngine.matchOrder(itrOrder->second);
			}
		}
	}

	return marketMachingEngine.getMatchedSize();
}


std::vector<Order> OrderCache::getAllOrders() const {

	std::vector<Order> allOrders;
	allOrders.reserve(mapIdToOrder.size());

	for (const auto& [orderId, order] : mapIdToOrder) {
 		allOrders.emplace_back(order);
 	}

	return allOrders;
}



