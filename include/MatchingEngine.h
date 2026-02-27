#include "OrderCache.h"

//Order Matching Engine for qequential processing and maching of Orders
class MatchingEngine {
  private:
	std::unordered_map<std::string, int>  buyToBeExecuted;		// Buy mapping 	=> company to orderSize
	std::unordered_map<std::string, int>  sellToBeExecuted;		// Sell mapping	=> company to orderSize
	int matchedSize =0;

  public:
	int getMatchedSize() const { return matchedSize; }
	void matchOrder(const Order& order);
};
/*

void MatchingEngine::matchOrder(const Order& order) {

	const bool isBuyOrder = (order.side()=="Buy");
	int receivedQty(order.qty());
	const std::string& receivedCompany = order.company();

	auto& oppositeSide = isBuyOrder ? sellToBeExecuted : buyToBeExecuted;
	auto& sameSide = isBuyOrder ? buyToBeExecuted : sellToBeExecuted;

	for (auto itr = oppositeSide.begin(); itr != oppositeSide.end(); )
	{
		if (itr->first != receivedCompany)
		{
			int availableQty = itr->second;
			if (receivedQty >= availableQty)
			{
				receivedQty -= availableQty;
				matchedSize += availableQty;
				itr = oppositeSide.erase(itr);
			}
			else
			{
				itr->second -= receivedQty;
				matchedSize += receivedQty;
				receivedQty = 0;
				break;
			}
		}
		else
		{
			++itr;
		}
	}

	if (receivedQty) {
		sameSide[receivedCompany] += receivedQty;
	}
}

*/

