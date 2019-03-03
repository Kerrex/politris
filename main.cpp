#include <iostream>
#include <vector>

class Polyomino
{
private:
	int order;
	std::vector<bool> shape;
public:
	explicit Polyomino(int order);
	void set(int x, int y, bool val);
	bool get(int x, int y);
	std::vector<Polyomino> generate_higher_order(void);
	void print(void);
	bool operator==(const Polyomino& rhs);
	bool operator!=(const Polyomino& rhs);
};

Polyomino::Polyomino(int order) :
	shape(order * order, false),
	order(order)
{
}

void Polyomino::set(int x, int y, bool val)
{
	shape[y * order + x] = val;
}

bool Polyomino::get(int x, int y)
{
	if ((x < 0) || (y < 0) || (x >= order) || (y >= order))
		return false;
	return shape[y * order + x];
}

std::vector<Polyomino> Polyomino::generate_higher_order(void)
{
	std::vector<Polyomino> ret;
	std::vector<Polyomino> gen;
	Polyomino temp(order + 2);
	
	// copy to a bigger board
	for (int y = 0; y < order; ++y)
		for (int x = 0; x < order; ++x)
			if (this->get(x, y))
			{
				temp.set(x + 1, y + 1, true);
			}
			
	// for every neighbor generate a new polyomino
	for (int y = 0; y < temp.order; ++y)
		for (int x = 0; x < temp.order; ++x)
			if (!temp.get(x, y) &&
					(temp.get(x-1, y) ||
					temp.get(x+1, y) ||
					temp.get(x, y-1) ||
					temp.get(x, y+1))
				)
			{
				Polyomino np = temp;
				np.set(x, y, true);
				gen.push_back(np);
			}
	
	// for every generated one - normalize its position
	for (auto &e : gen)
	{
		int empty_hor = 0;
		for (int y = 0; y < e.order; ++y)
		{
			bool empty = true;
			for (int x = 0; x < e.order; ++x)
			{
				if (e.get(x, y))
				{
					empty = false;
					break;
				}
			}
			if (empty)
				empty_hor++;
			else
				break;
		}
		
		int empty_ver = 0;
		for (int x = 0; x < e.order; ++x)
		{
			bool empty = true;
			for (int y = 0; y < e.order; ++y)
			{
				if (e.get(x, y))
				{
					empty = false;
					break;
				}
			}
			if (empty)
				empty_ver++;
			else
				break;
		}
		
		// order can be normalized as well in this moment
		Polyomino cand(order + 1);
		for (int y = 0; y < cand.order; ++y)
			for (int x = 0; x < cand.order; ++x)
			{
				cand.set(x, y, e.get(x + empty_ver, y + empty_hor));
			}
		ret.push_back(cand);
	}
	
	// remove duplicates
	for (auto i = ret.begin(); i != ret.end(); ++i)
		for (auto j = std::next(i, 1); j != ret.end(); )
		{
			if (*i == *j)
				j = ret.erase(j);
			else
				++j;
		}
	
	return ret;
}

void Polyomino::print(void)
{
	for (int y = 0; y < order; ++y)
	{
		for (int x = 0; x < order; ++x)
		{
			std::cout << (get(x, y) ? '*' : ' ');
		}
		std::cout << std::endl;
	}
}

bool Polyomino::operator==(const Polyomino& rhs)
{
	if (this->order != rhs.order)
		return false;
	
	auto i = this->shape.begin();
	auto j = rhs.shape.begin();
	for ( ; i != this->shape.end() || j != rhs.shape.end();
			++i, ++j)
	{
		if (*i != *j)
			return false;
	}
	return true;
}

bool Polyomino::operator!=(const Polyomino& rhs)
{
	return !(*this == rhs);
}

std::vector<Polyomino> generate_order(int order)
{
	std::vector<Polyomino> ret;
	
	if (order < 1)
		return ret;
	if (1 == order)
	{
		Polyomino p(1);
		p.set(0, 0, true);
		ret.push_back(p);
		return ret;
	}
	
	for (auto &e : generate_order(order - 1))
	{
		auto n = e.generate_higher_order();
		ret.insert(ret.end(), n.begin(), n.end());
	}
	
	// remove duplicates
	for (auto i = ret.begin(); i != ret.end(); ++i)
		for (auto j = std::next(i, 1); j != ret.end(); )
		{
			if (*i == *j)
				j = ret.erase(j);
			else
				++j;
		}
	
	return ret;
}

int main(int argc, char *argv[])
{
	std::cout << "Politris" << std::endl;
	
	Polyomino p(1);
	p.set(0, 0, true);
	p.print();
	
	int max = argc > 1 ? std::stoi(argv[1]) : 7;
	std::vector<Polyomino> vec;
	
	for (int i = 1; i <= max; ++i)
	{
		vec = generate_order(i);
		std::cout << "Number " << i << ": " <<
			vec.size() << std::endl;
	}
	
	int i = 0;
	for (auto &e : vec)
	{
		i++;
		std::cout << "=== " << i << " ===" << std::endl;
		e.print();
	}
	
	return 0;
}
