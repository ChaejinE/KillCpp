#include <iostream>
#include <set>
#include <string>

template <typename T, typename C>
void print_set(std::set<T, C>& s)
{
    for (const auto& elem : s)
    {
        std::cout << elem << " " << std::endl;
    }
}

class Todo
{
    int priority;
    std::string job_desc;

    public:
        Todo(int priority, std::string job_desc)
            : priority(priority), job_desc(job_desc) {}
        
        friend struct TodoCmp;
        friend std::ostream& operator<<(std::ostream& o, const Todo& td);
};

struct TodoCmp
{
    bool operator()(const Todo& t1, const Todo& t2) const
    {
        if (t1.priority == t2.priority)
        {
            return t1.job_desc < t2.job_desc;
        }
        return t1.priority > t2.priority;
    }
};

std::ostream& operator<<(std::ostream& o, const Todo& td)
{
    o << "[ 중요도: " << td.priority << "] " << td.job_desc;
    return o;
}

int main()
{
    std::set<Todo, TodoCmp> todos;

    todos.insert(Todo(1, "농구하기"));
    todos.insert(Todo(2, "수학숙제"));
    todos.insert(Todo(1, "프로그래밍"));
    todos.insert(Todo(3, "친구"));
    todos.insert(Todo(2, "영화보기"));

    print_set(todos);

    std::cout << "숙제를 끝냈다면 " << std::endl;
    todos.erase(todos.find((Todo(2, "수학숙제"))));
    print_set(todos);
}