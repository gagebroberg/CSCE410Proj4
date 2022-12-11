#include "concurrent_queue.h"

template<typename Data>
class concurrent_queue
{
private:
    std::queue<Data> the_queue;
    std::mutex the_mutex;
    std::condition_variable the_condition_variable;
public:
    void push(Data const& data)
    {
        std::mutex::scoped_lock lock(the_mutex);
        the_queue.push(data);
        lock.unlock();
        the_condition_variable.notify_one();
    }

    bool empty() const
    {
        std::mutex::scoped_lock lock(the_mutex);
        return the_queue.empty();
    }

    bool try_pop(Data& popped_value)
    {
        std::mutex::scoped_lock lock(the_mutex);
        if(the_queue.empty())
        {
            return false;
        }

        popped_value=the_queue.front();
        the_queue.pop();
        return true;
    }

    Data& front()
    {
	std::mutex::scoped_lock lock(the_mutex);
	if(the_queue.empty())
	{
		return NULL;
	}
	return the_queue.front();
    }

    void wait_and_pop(Data& popped_value)
    {
        std::mutex::scoped_lock lock(the_mutex);
        while(the_queue.empty())
        {
            the_condition_variable.wait(lock);
        }

        popped_value=the_queue.front();
        the_queue.pop();
    }

};
