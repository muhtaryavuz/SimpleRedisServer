/*
 * By mmy
 */

#include <exception>

#include "Service/EventExecutor.hpp"
#include "WorkerModule/Worker.hpp"

worker_utility::Worker::Worker(
    int id, std::shared_ptr<worker_utility::Publishable> publisher)
    : id_(id), publisher_(publisher) {}

void worker_utility::Worker::init(std::string name) {
  thread_ = std::make_unique<std::thread>(&Worker::process, this);
  if (int result = pthread_setname_np(thread_->native_handle(), name.c_str());
      result != 0)
    throw std::runtime_error("Couldnt set thread name error->" +
                             std::to_string(result));
}

std::size_t worker_utility::Worker::getQueueSize() {
  std::scoped_lock<std::mutex> lock(mutex_);
  return queue_.size();
}

void worker_utility::Worker::insertToQueue(worker_variant_t &&event) {
  {
    std::scoped_lock<std::mutex> lock(mutex_);
    queue_.push(std::move(event));
    publisher_->publishSize(id_, queue_.size());
  }
  cv_.notify_one();
}

void worker_utility::Worker::stopExecution() {
  is_running_ = false;
  cv_.notify_one();
  if (thread_->joinable())
    thread_->join();
}

void worker_utility::Worker::process() {

  worker_utility::worker_variant_t event;
  while (is_running_) {
    std::unique_lock<std::mutex> lock(mutex_);
    cv_.wait(lock);

    if (queue_.empty())
      continue;

    event = queue_.front();
    queue_.pop();
    publisher_->publishSize(id_, queue_.size());
    lock.unlock();

    std::visit(service::EventExecutor{}, event);
  }
  std::cout << "Thread" << id_ << " is terminated" << std::endl;
}