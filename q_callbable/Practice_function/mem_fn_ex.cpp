#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>

using std::vector;

int main()
{
    vector<int> a(1);
    vector<int> b(2);
    vector<int> c(3);
    vector<int> d(4);

    vector<vector<int>> container;
    container.push_back(a);
    container.push_back(b);
    container.push_back(c);
    container.push_back(d);

    vector<int> size_vec(4);
    transform(container.begin(), container.end(), size_vec.begin(),
              std::mem_fn(&vector<int>::size));
    for (auto itr = size_vec.begin(); itr != size_vec.end(); ++itr)
    {
        std::cout << "vector size :: " << *itr << std::endl;
    }
}