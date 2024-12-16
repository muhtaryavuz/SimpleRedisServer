/*
 * By mmy
 */

#include <exception>

#include "Service/EventExecutor.hpp"
#include "WorkerModule/Worker.hpp"

worker_utility::Worker::Worker(
    int id, std::shared_ptr<worker_utility::Publishable> publisher)
    : m_id(id), m_publisher(publisher) {}

void worker_utility::Worker::init(std::string name) {
  m_thread = std::make_unique<std::thread>(&Worker::process, this);
  if (int result = pthread_setname_np(m_thread->native_handle(), name.c_str());
      result != 0)
    throw std::runtime_error("Couldnt set thread name error->" +
                             std::to_string(result));
}

std::size_t worker_utility::Worker::getQueueSize() {
  std::scoped_lock<std::mutex> lock(m_mutex);
  return m_queue.size();
}

void worker_utility::Worker::insertToQueue(worker_variant_t const &event) {
  {
    std::scoped_lock<std::mutex> lock(m_mutex);
    m_queue.push(std::move(event));
    m_publisher->publishSize(m_id, m_queue.size());
  }
  m_cv.notify_one();
}

void worker_utility::Worker::stopExecution() {
  m_is_running = false;
  m_cv.notify_one();
  if (m_thread->joinable())
    m_thread->join();
}

void worker_utility::Worker::process() {

  worker_utility::worker_variant_t event;
  while (m_is_running) {
    std::unique_lock<std::mutex> lock(m_mutex);
    m_cv.wait(lock);

    if (m_queue.empty())
      continue;

    event = m_queue.front();
    m_queue.pop();
    m_publisher->publishSize(m_id, m_queue.size());
    lock.unlock();

    std::visit(service::EventExecutor{}, event);
  }
  std::cout << "Thread" << m_id << " is terminated" << std::endl;
}