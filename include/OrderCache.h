#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <list>
#include <map>
#include <unordered_map>
#include <stdexcept>

// Estimates for optimal performance
constexpr size_t ORDER_CAPACITY = 1000000;		//to efficiently process up to 1 million orders

class Order
{

 public:
  Order() = default;

  // Constructor for creating an order with all required fields.
  // Each order is uniquely identified by orderId.
  Order(
      const std::string& ordId,
      const std::string& secId,
      const std::string& side,
      const unsigned int qty,
      const std::string& user,
      const std::string& company)
      : m_orderId(ordId),
        m_securityId(secId),
        m_side(side),
        m_qty(qty),
        m_user(user),
        m_company(company) { }


    // Copy constructor
    Order(const Order& other) {
        m_orderId = other.m_orderId;
        m_securityId = other.m_securityId;
        m_side = other.m_side;
        m_qty = other.m_qty;
        m_user = other.m_user;
        m_company = other.m_company;
    }

  // Accessor methods
  std::string orderId() const    { return m_orderId; }
  std::string securityId() const { return m_securityId; }
  std::string side() const       { return m_side; }
  std::string user() const       { return m_user; }
  std::string company() const    { return m_company; }
  unsigned int qty() const       { return m_qty; }

  bool isValidOrder() const;

 private:

  // Core attributes of an order. These fields define identity, ownership, and trade details.
  std::string m_orderId;     // Unique identifier for the order
  std::string m_securityId;  // security identifier
  std::string m_side;        // side of the order, eg Buy or Sell
  unsigned int m_qty;        // qty for this order
  std::string m_user;        // user name who owns this order
  std::string m_company;     // company for user

};


class OrderCacheInterface
{
 public:

  // add order to the cache
  virtual void addOrder(Order order) = 0;

  // remove order with this unique order id from the cache
  virtual void cancelOrder(const std::string& orderId) = 0;

  // remove all orders in the cache for this user
  virtual void cancelOrdersForUser(const std::string& user) = 0;

  // remove all orders in the cache for this security with qty >= minQty
  virtual void cancelOrdersForSecIdWithMinimumQty(const std::string& securityId, unsigned int minQty) = 0;

  // return the total qty that can match for the security id
  virtual unsigned int getMatchingSizeForSecurity(const std::string& securityId) = 0;

  // return all orders in cache in a vector
  virtual std::vector<Order> getAllOrders() const = 0;

};

// The OrderCache maintains multiple indexes for efficient lookups:
// - mapIdToOrder: direct access by order ID
// - mapSecurityToOrderIds: grouping by securityId
// - mapUserToOrderIds: grouping by user
class OrderCache : public OrderCacheInterface
{

 public:

  OrderCache();

  void addOrder(Order order) override;

  void cancelOrder(const std::string& orderId) override;

  void cancelOrdersForUser(const std::string& user) override;

  void cancelOrdersForSecIdWithMinimumQty(const std::string& securityId, unsigned int minQty) override;

  unsigned int getMatchingSizeForSecurity(const std::string& securityId) override;

  std::vector<Order> getAllOrders() const override;

 private:

	// Primary storage: mapIdToOrder provides O(1) access to orders by unique ID.
	std::unordered_map<std::string, Order> mapIdToOrder;

	// Secondary indexes support efficient queries by security and user.
	std::unordered_map<std::string, std::vector<std::string>> mapSecurityToOrderIds;
	std::unordered_map<std::string, std::vector<std::string>> mapUserToOrderIds;
};
