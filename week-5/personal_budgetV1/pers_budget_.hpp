#include <iostream>
#include <vector>
#include <string>

constexpr int range = 36525;

std::pair<size_t, size_t> ConvertRangeToIndex(string_view start, string_view end)
{

	return {0, 0};
}

/*enum Command
{
	ComputeIncome,
	Earn,
	PayTax
};*/

class IBudget
{
	virtual ~IBudget();
	virtual void ComputeIncome(const std::string& start, const std::string& end) = 0;
	virtual void Earn(const std::string& start, const std::string& end, const double amount) = 0;
	virtual void PayTax(const std::string& start, const std::string& end) = 0;
};

class DesktopBudget : public IBudget
{
public:
	DesktopBudget() : account_(range, 0) {}

	void ComputeIncome(const std::string& start, const std::string& end) override
	{
		auto [startDate, endDate] = ConvertRangeToIndex(start, end);

		double earningsSum = 0;
		for (size_t i = startDate; i <= endDate; i++)
		{
			earningsSum += account_[i];
		}

		std::cout << earningsSum << "\n";
	}

	void Earn(const std::string& start, const std::string& end, const double amount) override
	{
		auto [startDate, endDate] = ConvertRangeToIndex(start, end);

		double earningPerDay = amount / (endDate - startDate);
		for (size_t i = startDate; i <= endDate; i++)
		{
			account_[i] += earningPerDay;
		}
	}

	void PayTax(const std::string& start, const std::string& end) override
	{
		auto [startDate, endDate] = ConvertRangeToIndex(start, end);

		double earningsSum = 0;
		for (size_t i = startDate; i <= endDate; i++)
		{
			account_[i] *= tax;
			earningsSum += account_[i];
		}

		std::cout << earningsSum << "\n";
	}
private:
	constexpr double tax = 0.87;
	std::vector<double> account_;
};

int main()
{
	DesktopBudget bud;

	int numberOfQueries;
	std::cin >> numberOfQueries;

	std::string command, start, end;
	double amount;
	while(numberOfQueries > 0)
	{	
		std::cin >> command >> start >> end;
		switch (command)
		{
			case "ComputeIncome":
				bud.ComputeIncome(start, end);
				break;
			case "Earn":
				std::cin >> amount;
				bud.Earn(start, end, amount);
				break;
			case "PayTax":
				bud.PayTax(start, end);
				break;
			default:
				std::cout << "Unknown command\n";
				break;
		}

		numberOfQueries--;
	}
	
	return 0;
}