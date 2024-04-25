#pragma once
#include <iterator>
#include <serial_protocol/TaskObject.hpp>
#include <tasks/Task.hpp>

class TaskObjectIterator
{
  public:
    typedef device::TaskCollection::const_iterator Backend;
    typedef std::input_iterator_tag iterator_category;
    typedef task_tracker_systems::TaskObject value_type;
    typedef const value_type reference_type;

    TaskObjectIterator(const Backend &iterator)
        : backend(iterator)
    {
    }

    operator reference_type() const
    {
        return this->operator*();
    }

    reference_type operator*() const
    {
        return {
            .id = backend->first,
            .label = backend->second.getLabel(),
            .duration = backend->second.getLastRecordedDuration().count()};
    }

    TaskObjectIterator &
    operator++()
    {
        ++backend;
        return *this;
    }

    TaskObjectIterator operator++(int)
    {
        const TaskObjectIterator temp = *this;
        ++(*this);
        return temp;
    }

    bool operator==(const TaskObjectIterator &other) const
    {
        return backend == other.backend;
    }

    bool operator!=(const TaskObjectIterator &other) const
    {
        return !(*this == other);
    }

  private:
    Backend backend;
};

TaskObjectIterator begin(const device::TaskCollection &tasks)
{
    return std::cbegin(tasks);
}

TaskObjectIterator end(const device::TaskCollection &tasks)
{
    return std::cend(tasks);
}
