#include<iostream>
#include<vector>
#include<string>
#include<utility>
#include<fstream>
#include<queue>
#include<stack>	
#include<bitset>
using namespace std;

using Costs = vector<vector<pair<int,int>>>;
using Matrix = vector<vector<int>>;
using PCType = vector<int>;
using counter_type = Costs::size_type;
using char_map = vector<vector<char>>;
using std::cout;



template<typename T>
class Adapter
{
private:
	T& object;
private:
	template<typename U>
	class Proxy
	{
	private:
		U& subobjeсе;
	public:
		Proxy() = delete;
		Proxy(U& other) :subobjeсе(other) {}
		Proxy(U&& other) :subobjeсе(other) {};
		decltype(auto) operator[](size_t index)
		{
			return (subobjeсе[index].second);
		}
	};
public:
	Adapter() = delete;
	Adapter(T& _other) :object(_other) {}
	decltype(auto) operator[](size_t index)
	{
		return Proxy<T::value_type>(object[index]);
	}
};

struct cell
{
	int row;
	int col;
	cell() = default;
	cell(int r, int c) :row(r), col(c) {}
};

struct e_cell
{
	int row;
	int col;
	char d;
	e_cell() = default;
	e_cell(int r, int c, char ds) :row(r), col(c), d(ds) {}
};

void way_back(stack<e_cell>& st, e_cell& t, Matrix& counters, int s_r, int s_c)
{
	bool flag = true;
	char dis = t.d;
	e_cell next = t;
	e_cell last;
	while (true)
	{
		while (dis == next.d)
		{
			if (st.empty())
				break;
			last = st.top();
			st.pop();
			counters[next.row][next.col] -= 1;
			next = st.top();


		}
		if (last.d != next.d)
			{
				e_cell buff;
				if (last.d == 'U')
				{
					buff = last;
					buff.row -= 1;
				}
				else if (last.d == 'D')
				{
					buff = last;
					buff.row += 1;
				}
				else if (last.d == 'L')
				{
					buff = last;
					buff.col -= 1;
				}
				else if (last.d == 'R')
				{
					buff = last;
					buff.col += 1;
				}
				if ((buff.row == next.row) && (buff.col == next.col))
				{
					
						counters[buff.row][buff.col] -= 1;
						dis = next.d;
						last = next;
						st.pop();
						next = st.top();
				}
				else
				{
					counters[buff.row][buff.col] -= 1;
					break;

				}

			}
	}

}
	
template<typename T>
void found_cycle(T& m, Matrix& counters, char_map& map, vector<e_cell>& nodes, int s_r, int s_c, e_cell& st)
{
	stack<e_cell> way;
	way.push(st);
	if (st.row + 1 < s_r)
	{
		way.emplace(st.row + 1, st.col, 'U');
		counters[st.row][st.col] += 1;
	}
	if (0 <= st.row - 1)
	{
		way.emplace(st.row - 1, st.col, 'D');
		counters[st.row][st.col] += 1;
	}
	if (st.col + 1 < s_c)
	{
		way.emplace(st.row, st.col + 1, 'L');
		counters[st.row][st.col] += 1;
	}
	if (0 <= st.col - 1)
	{
		way.emplace(st.row, st.col - 1, 'R');
		counters[st.row][st.col] += 1;
	}
	e_cell current;
	while (true)
	{
		if (way.empty())
		{
			cerr << "empty stack" << endl;
			break;
		}
		current = way.top();
		if (current.row == st.row&&current.col == st.col)
		{
			//cout << "found cycle" << endl;
			break;
		}
		if (m[current.row][current.col] == -1)
		{
			if (current.d == 'L')
			{

				if (current.col + 1 >= s_c)
				{
					counters[current.row][current.col] += 1;
					way_back(way, current, counters, s_r, s_c); continue;
				}
				way.emplace(current.row, current.col + 1, 'L');
				counters[current.row][current.col] += 1;
				continue;
			}
			if (current.d == 'R')
			{

				if (current.col - 1 < 0)
				{
					counters[current.row][current.col] += 1;
					way_back(way, current, counters, s_r, s_c); continue;
				}
				way.emplace(current.row, current.col - 1, 'R');
				counters[current.row][current.col] += 1;
				continue;
			}
			if (current.d == 'U')
			{

				if (current.row + 1 >= s_r)
				{
					counters[current.row][current.col] += 1;
					way_back(way, current, counters, s_r, s_c); continue;
				}
				way.emplace(current.row + 1, current.col, 'U');
				counters[current.row][current.col] += 1;
				continue;
			}
			if (current.d == 'D')
			{
				if (current.row - 1 < 0)
				{
					counters[current.row][current.col] += 1;
					way_back(way, current, counters, s_r, s_c); continue;
				}
				way.emplace(current.row - 1, current.col, 'D');
				counters[current.row][current.col] += 1;
				continue;
			}
		}
		if (m[current.row][current.col] != -1)
		{
			if (current.row + 1 < s_r)
			{
				if (current.d != 'D')
				{
					way.emplace(current.row + 1, current.col, 'U');
					counters[current.row][current.col] += 1;
				}
			}
			if (0 <= current.row - 1)
			{
				if (current.d != 'U')
				{
					way.emplace(current.row - 1, current.col, 'D');
					counters[current.row][current.col] += 1;
				}
			}
			if (current.col + 1 < s_c)
			{
				if (current.d != 'R')
				{
					way.emplace(current.row, current.col + 1, 'L');
					counters[current.row][current.col] += 1;
				}
			}
			if (current.col - 1 >= 0)
			{
				if (current.d != 'L')
				{
					way.emplace(current.row, current.col - 1, 'R');
					counters[current.row][current.col] += 1;
				}
			}
		}

	}

	vector<e_cell>buffer;
	e_cell temp = way.top();
	way.pop();
	nodes.push_back(temp);
	while (!way.empty())
	{
		buffer.push_back(way.top());
		way.pop();
	}
	char dis = temp.d;
	
	while (true)
	{
		if (temp.d == 'D')
		{
			temp.row += 1;
		}
		if (temp.d == 'U')
		{
			temp.row -= 1;
		}
		if (temp.d == 'R')
		{
			temp.col += 1;
		}
		if (temp.d == 'L')
		{
			temp.col -= 1;
		}
		//char temp;
		auto it = find_if(buffer.begin(), buffer.end(), [&temp](const e_cell&oth) {return (temp.row == oth.row) && (temp.col == oth.col)&&(temp.d==oth.d); });
		if (it == buffer.end())
		{
			it = find_if(buffer.begin(), buffer.end(), [&temp](const e_cell&oth) {return (temp.row == oth.row) && (temp.col == oth.col); });
			if (it == buffer.end())
			{
				cerr << "bad1" << endl;
				system("pause");
			}
		}
		e_cell buff = *it;
		if (buff.row == st.row&&buff.col == st.col)
			break;
		if (dis != buff.d)
		{
			dis = buff.d;
			nodes.push_back(buff);
			buffer.erase(it);
			temp = nodes.back();
		}
		
	}
	
}

void upgrade_plan(Costs& _costs,vector<e_cell>& _nodes, e_cell& _start)
{

	bool flag = true;
	for (auto& x : _nodes)
	{
		if (flag)
		{
			x.d = '+';
			flag = false;
			continue;
		}
		x.d = '-';
		flag = true;
	}
	// поиск наименьшего значения с '-'
	e_cell min_elem;
	int value_min=INT_MAX;
	for (auto& x : _nodes)
	{
		if (x.d == '-')
		{
			if (_costs[x.row][x.col].second < value_min)
			{
				value_min=_costs[x.row][x.col].second;
				min_elem.row = x.row;
				min_elem.col = x.col;
				min_elem.d = x.d;
			}
		}
	}
	
	// улучшение плана
	bool zero = true;
	_costs[_start.row][_start.col].second += 1;
	for (auto& x : _nodes)
	{
		if (x.d == '+')
		{
			_costs[x.row][x.col].second += value_min;
		}
		else
		{
			_costs[x.row][x.col].second -= value_min;
			if (_costs[x.row][x.col].second == 0)
			{
				if (zero)
				{
					_costs[x.row][x.col].second = -1;
					zero = false;
				}
				
			}
		}
	}
}

void search_basic_plan(Costs& _costs,PCType& _producers, PCType& _consumers)
{
	auto count_prod=_costs.size();
	auto count_cons = _costs[0].size();
	auto index = 0;
	for (counter_type i = 0; i < count_prod; i++)
	{
		for (counter_type j = index; j < count_cons; j++)
		{
			if (_consumers[j] >= _producers[i])
			{
				_costs[i][j].second =_producers[i];
				_consumers[j] = _consumers[j] - _producers[i];
				_producers[i] = 0;
			}
			else
			{
				_costs[i][j].second = _consumers[j];
				_producers[i] = _producers[i] - _consumers[j];
				_consumers[j] = 0;
			}
			if (_producers[i] == 0)
			{
				index = j;
				break;
			}
		}
	}

}

void opt_plan(Costs& _costs, PCType& _producers, PCType& _consumers)
{
	int qwe = 0;
	while (true)
	{
		qwe++;
		PCType poten_V(_consumers.size(), INT_MAX);
		PCType poten_U(_producers.size(), INT_MAX);
		Matrix values(_consumers.size());
		counter_type index_row = 0;
		counter_type index_colum = 0;
		int min_prev_value = 0;
		poten_U[0] = 0;
		bool is_all = true;
		while (true)
		{
			
			for (counter_type i = 0; i < poten_U.size(); i++)
			{
				for (counter_type j = 0; j < poten_V.size(); j++)
				{
					if (_costs[i][j].second == -1)
						continue;
					if ((poten_V[j] == INT_MAX) && (poten_U[i] == INT_MAX))
					{
						is_all = false;
						continue;
					}
					if (poten_V[j] == INT_MAX)
					{
						poten_V[j] = poten_U[i] + _costs[i][j].first;
						continue;
					}
					if (poten_U[i] == INT_MAX)
					{
						poten_U[i] = poten_V[j] - _costs[i][j].first;
						continue;
					}
				}
			}

			if (is_all)
			{
				break;
			}
			is_all = true;

		}
		

		//поиск минимального элемента
		for (counter_type i = 0; i < _producers.size(); i++)
		{
			for (counter_type j = 0; j < _consumers.size(); j++)
			{
				int delt = poten_U[i] + _costs[i][j].first - poten_V[j];
				if (delt < min_prev_value)
				{
					min_prev_value = delt;
					index_row = i;
					index_colum = j;
				}
				values[i].push_back(delt);
			}
		}

		//выход из главного цикла
		if (min_prev_value >= 0)
		{
			break;
		}
		// подготовка и преобразвоание вспомогательных данных
		Adapter<Costs> def(_costs);
		vector<e_cell> nodes;
		char_map map;
		Matrix counters;
		e_cell start(index_row, index_colum, 'S');
		int s_r = _costs.size();
		int s_c = _costs[0].size();
		for (int i = 0; i < s_r; i++)
		{
			map.emplace_back(vector<char>());
			counters.emplace_back(vector<int>());
			for (int j = 0; j < s_c; j++)
			{
				map[i].push_back(0);
				counters[i].push_back(0);
			}
		}

		// тут поиск цикла
		found_cycle(def, counters, map, nodes, s_r, s_c, start);
		
		upgrade_plan(_costs, nodes, start);

		cout << "Итерация: " << qwe << endl;

		cout.setf(ios_base::right,ios_base::adjustfield);

		for (int i = 0; i < s_r; i++)
		{
			for (int j = 0; j < s_c; j++)
			{
				cout.width(4);
				if (_costs[i][j].second == -1)
				{
					cout << 0 << " ";
					continue;
				}
				cout << _costs[i][j].second << " ";
			}
			cout << endl;
		}
		cout << endl;
	}
}

double func_value(Costs& _costs,int s_r,int s_c)
{
	double result=0;
	for (int i = 0; i < s_r; i++)
	{
		for (int j = 0; j < s_c; j++)
		{
			if (_costs[i][j].second != -1)
			{
				result += _costs[i][j].second*_costs[i][j].first;
			}
		}
	}
	return result;
}


int main()
	{
	setlocale(LC_ALL, "Rus");

		int p=7;
		int c=8;
		Costs costs(p);
		PCType produs(p);
		PCType cons(c);
		
		fstream fl;
		fl.open("costs.txt");
		for (int i = 0; i < p; i++)
		{
			for (int j = 0; j < c; j++)
			{
				int a;
				fl>> a;
				costs[i].push_back(std::pair<int,int>(a,-1));
			}
		}
		fl.close();
		fl.open("prod.txt");
		for (int i = 0; i < p; i++)
			fl >> produs[i];
		fl.close();
		fl.open("cons.txt");
		for (int i = 0; i < c; i++)
			fl >> cons[i];
		fl.close();
		
		cout << "Истоки: " << endl;
		for (int i = 0; i < p; i++)
			cout << produs[i] << " ";
		cout << endl;
		cout << "Стоки: " << endl;
		for (int i = 0; i < c; i++)
			cout << cons[i] << " ";
		cout << endl;
		
		search_basic_plan(costs, produs, cons);
		opt_plan(costs, produs, cons);

		cout << "Значение целевой функции: " << func_value(costs, p, c) << endl;

	system("pause");
}

