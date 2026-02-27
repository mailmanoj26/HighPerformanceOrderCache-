#include <MatchingEngine.h>

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



