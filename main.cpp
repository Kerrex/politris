#include <iostream>
#include <vector>
#include <algorithm>

void print_vec(std::vector<std::vector<bool>> normalized)
{
	for (auto i = 0; i < normalized.size(); i++)
	{
		for (auto j = 0; j < normalized.at(i).size(); j++)
		{
			std::cout << (normalized.at(i).at(j) ? '1' : '0');
		}
		std::cout << std::endl;
	}
}

bool is_first_column_empty(std::vector<std::vector<bool>> vec)
{
	bool is_first_column_empty = true;
	for (auto i = 0; i < vec.size(); i++)
	{
		if (vec.at(i).at(0))
		{
			return false;
		}
	}
	return true;
}

bool is_last_column_empty(std::vector<std::vector<bool>> vec)
{
	bool is_first_column_empty = true;
	for (auto i = 0; i < vec.size(); i++)
	{
		auto size = vec.at(i).size();
		if (vec.at(i).at(size - 1))
		{
			return false;
		}
	}
	return true;
}

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
	bool operator==(const Polyomino &rhs);
	bool operator!=(const Polyomino &rhs);
	Polyomino rotated();
	std::vector<std::vector<bool>> get_normalized_vector();
};

Polyomino::Polyomino(int order) : shape(order * order, false),
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
				(temp.get(x - 1, y) ||
				 temp.get(x + 1, y) ||
				 temp.get(x, y - 1) ||
				 temp.get(x, y + 1)))
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
		for (auto j = std::next(i, 1); j != ret.end();)
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
			std::cout << (get(x, y) ? '1' : '0');
		}
		std::cout << std::endl;
	}
}

bool Polyomino::operator==(const Polyomino &rhs)
{
	if (this->order != rhs.order)
		return false;

	auto i = this->shape.begin();
	auto j = rhs.shape.begin();
	for (; i != this->shape.end() || j != rhs.shape.end();
		 ++i, ++j)
	{
		if (*i != *j)
			return false;
	}
	return true;
}

bool Polyomino::operator!=(const Polyomino &rhs)
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
		for (auto j = std::next(i, 1); j != ret.end();)
		{
			if (*i == *j)
				j = ret.erase(j);
			else
				++j;
		}

	return ret;
}

std::vector<std::vector<bool>> rotate(std::vector<std::vector<bool>> &to_rotate)
{
	std::vector<std::vector<bool>> new_polyomino_vec;
	//std::cout << to_rotate.at(0).size() << std::endl;
	for (auto i = 0; i < to_rotate.at(0).size(); i++)
	{
		std::vector<bool> row;
		// 	std::cout << i << std::endl;
		for (int j = to_rotate.size() - 1; j >= 0; j--)
		{
			//std::cout << "(" << j << ", " << i << ")" << std::endl;
			// 		std::cout << to_rotate.at(i).size()-1 << std::endl;
			row.push_back(to_rotate.at(j).at(i));
		}
		new_polyomino_vec.push_back(row);
	}

	return new_polyomino_vec;
}

std::vector<std::vector<bool>> Polyomino::get_normalized_vector()
{
	std::vector<std::vector<bool>> not_yet_normalized;

	//remove empty rows
	for (auto i = 0; i < order; i++)
	{
		std::vector<bool> row;
		bool should_add_row = false;
		for (auto j = 0; j < order; j++)
		{
			auto value = get(j, i);
			row.push_back(value);
			if (value)
			{
				should_add_row = true;
			}
		}
		if (should_add_row)
		{
			not_yet_normalized.push_back(row);
		}
	}

	//std::cout << "Removing empty starting columns" << std::endl;
	// remove empty starting columns
	bool is_st_empty = is_first_column_empty(not_yet_normalized);
	while (is_st_empty)
	{
		for (auto i = 0; i < not_yet_normalized.size(); i++)
		{
			auto &vec = not_yet_normalized.at(i);
			vec.erase(vec.begin());
		}
		is_st_empty = is_first_column_empty(not_yet_normalized);
	}

	//std::cout << "Removing empty ending columns" << std::endl;
	// remove empty ending columns
	bool is_en_empty = is_last_column_empty(not_yet_normalized);
	while (is_en_empty)
	{
		//std::cout << "Last column empty!" << std::endl;
		for (auto i = 0; i < not_yet_normalized.size(); i++)
		{
			auto &vec = not_yet_normalized.at(i);
			vec.erase(vec.begin() + vec.size() - 1);
		}
		is_en_empty = is_last_column_empty(not_yet_normalized);
	}

	return not_yet_normalized;
}

void print_one_sided(std::vector<Polyomino> &polys)
{
	int i = 0;
	std::vector<std::vector<std::vector<bool>>> without_duplicates;
	for (auto &e : polys)
	{
		auto normalized = e.get_normalized_vector();
		if (std::find(without_duplicates.begin(), without_duplicates.end(), normalized) == without_duplicates.end())
		{
			auto rotated90 = rotate(normalized);
			if (std::find(without_duplicates.begin(), without_duplicates.end(), rotated90) == without_duplicates.end())
			{
				auto rotated180 = rotate(rotated90);
				if (std::find(without_duplicates.begin(), without_duplicates.end(), rotated180) == without_duplicates.end())
				{
					auto rotated270 = rotate(rotated180);
					if (std::find(without_duplicates.begin(), without_duplicates.end(), rotated270) == without_duplicates.end())
					{
						without_duplicates.push_back(normalized);
						i++;
						std::cout << "=== " << i << " ===" << std::endl;
						print_vec(normalized);
					}
				}
			}
		}
	}
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
		std::cout << "Number " << i << ": " << vec.size() << std::endl;
	}
	
	print_one_sided(vec);

	return 0;
}