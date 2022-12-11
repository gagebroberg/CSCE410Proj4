#include <mutex>
#include <condition_variable>

template<typename Data>
class concurrent_queue
{
private:
    std::queue<Data> the_queue;
    std::mutex the_mutex;
    std::condition_variable the_condition_variable;
public:
    void push(Data const& data);
    bool empty() const;
    bool try_pop(Data& popped_value);
    Data& front();
    void wait_and_pop(Data& popped_value);
};
